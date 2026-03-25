int sw1 = 12;
int sw2 = 27;
int count = 0;
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
    count++;
    Serial.print("Count = "); Serial.println(count); delay(1000);
  }

  if(digitalRead(sw2) == LOW)
  {
    count--;
    if(count < 0) count = 0;
    Serial.print("Count = "); Serial.println(count); delay(1000);
  }
}
