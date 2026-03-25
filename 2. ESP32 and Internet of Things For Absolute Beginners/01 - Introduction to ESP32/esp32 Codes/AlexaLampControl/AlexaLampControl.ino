/***************************************************
Complete IoT Project with DHT22, Relay and ESP32 Huzzah Board
THE SAME CODE TO BE USED TO CONTROL LAMP FROM ALEXA

DHT Sensor connected to PIn 27
Relay Connected to Pin 12
 ****************************************************/

#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Robotics Lab"  // replace it with your Wifi Name
#define WLAN_PASS       "Robotics@321"  // Replace it with your wifi password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "amitrana3348"  // Change this to your username
#define AIO_KEY         "3e359482c74e59509c3f50ab76e3a80958f55033"  // Change this to your key

#include "DHT.h"
#define DHTPIN 27 // dht connection
#define DHTTYPE DHT22   // DHT 22  connected to pin 27
DHT dht(DHTPIN, DHTTYPE);

int rel = 12;


/************ Global State (you don't need to change this!) ******************/

WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Subscribe relay = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay");
Adafruit_MQTT_Subscribe led = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() 
{
  Serial.begin(9600);
  delay(10);
  pinMode(rel,OUTPUT); digitalWrite(rel,LOW);
  pinMode(LED_BUILTIN,OUTPUT); digitalWrite(LED_BUILTIN,LOW);
  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&relay);
  mqtt.subscribe(&led);
  dht.begin();
}


void loop() 
{
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) 
  {
    if (subscription == &led) 
    {
      Serial.print(F("Got: "));
      Serial.println((char *)led.lastread);
      if(strcmp((char *)led.lastread, "ON") == 0) 
      {
        digitalWrite(LED_BUILTIN,HIGH);
        Serial.println("LED Turned ON");
      }
      if(strcmp((char *)led.lastread, "OFF") == 0)
      {
        digitalWrite(LED_BUILTIN,LOW);
        Serial.println("LED Turned OFF");
      }
    }
    
    if (subscription == &relay) 
    {
      Serial.print(F("Got: "));
      Serial.println((char *)relay.lastread);
      if(strcmp((char *)relay.lastread, "ON") == 0) 
      {
        digitalWrite(rel,HIGH);
        Serial.println("Relay Turned ON");
      }
      if(strcmp((char *)relay.lastread, "OFF") == 0)
      {
        digitalWrite(rel,LOW);
        Serial.println("Relay Turned OFF");
      }
    }
  }

  // Now we can publish stuff!
  float h = dht.readHumidity(); 
  float t = dht.readTemperature();
 
  Serial.print(F("\nSending Temperature value "));
  Serial.print(t);
  Serial.print("...");
  if (! temperature.publish(t)) 
  {
    Serial.println(F("Failed"));
  } 
  else 
  {
    Serial.println(F("OK!"));
  }
  delay(1500);
  Serial.print(F("\nSending Humidity value "));
  Serial.print(h);
  Serial.print("...");
  if (! humidity.publish(h)) 
  {
    Serial.println(F("Failed"));
  } 
  else 
  {
    Serial.println(F("OK!"));
  }
  Serial.println("*******************************************************************************************");
  
  delay(1500);

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() 
{
  unsigned char ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) 
  { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
