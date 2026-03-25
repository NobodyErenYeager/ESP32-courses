void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
}

void loop()
{
  int a = touchRead(14);
  if ( a < 40) digitalWrite(LED_BUILTIN,HIGH);
  else digitalWrite(LED_BUILTIN,LOW);
  //Serial.println(a);
  //delay(1000);
}
