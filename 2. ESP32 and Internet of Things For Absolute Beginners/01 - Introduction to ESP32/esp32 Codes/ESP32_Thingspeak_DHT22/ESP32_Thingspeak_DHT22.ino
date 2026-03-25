/******************************************************************/
/*****************************************************************/
/******************************************************************/
// Import nessesary libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#define DHTPIN 27 // dht connection
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);


// Set our wifi name and password

const char* ssid = "Robotics Lab";   // your network SSID (name) 
const char* password = "Robotics@321";   // your network password

// Your thingspeak channel url with api_key query
String serverName = "https://api.thingspeak.com/update?api_key=XXNUJDNUCHY4FTYU"; // please replace it with your write API KEy


void setup() 
{
  Serial.begin(9600); // Opens up the serial port with a baudrate of 9600 bits per second

  
  WiFi.begin(ssid, password); // Attempt to connect to wifi with our password
  Serial.println("Connecting"); // Print our status to the serial monitor
  // Wait for wifi to connect
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  delay(2000);
  dht.begin();
}


void loop() 
{
    if(WiFi.status()== WL_CONNECTED)
    { 
      // Check to make sure wifi is still connected
      HTTPClient http; // Initialize our HTTP client
      float h = dht.readHumidity(); 
      float t = dht.readTemperature();
      Serial.print("Temperature = "); Serial.println(t);
      Serial.print("Humidity = "); Serial.println(h);
      Serial.println("*******************************************************************************************");
      String url = serverName + "&field1=" + t + "&field2=" + h ; // Define our entire url
      http.begin(url.c_str()); // Initialize our HTTP request
      int httpResponseCode = http.GET(); // Send HTTP request
      if (httpResponseCode > 0)// Check for good HTTP status code
      { 
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      }
      else
      {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
      delay(15000);  
    }
    else 
    {
      Serial.println("WiFi Disconnected");
      delay(1000);
    }
    
}
