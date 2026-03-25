int sw1 = 12;
int sw2 = 27;
void setup()
{
  Serial.begin(9600); delay(10);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP); // default state is HIGH
}

void loop()
{
  if(digitalRead(sw1) == LOW)
  {
    Serial.println("Sw1 is pressed "); delay(600); 
    while(digitalRead(sw1) == LOW);
    delay(20);
  }

  if(digitalRead(sw2) == LOW)
  {
    Serial.println("Sw2 is pressed "); delay(600); 
    while(digitalRead(sw2) == LOW);
    delay(20);
  }
}
