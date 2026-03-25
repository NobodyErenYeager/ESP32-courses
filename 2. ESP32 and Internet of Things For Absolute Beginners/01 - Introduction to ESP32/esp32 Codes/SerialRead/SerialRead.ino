void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); delay(3000);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  //Serial.available() it returns the number of bytes ready for reading
  if( Serial.available() > 0) // even if a single char is ready, then this if condition is true and will be executed
  {
    int a = Serial.read();
    delay(3000);
    Serial.write(a+1);
    Serial.println("");
  }
}
