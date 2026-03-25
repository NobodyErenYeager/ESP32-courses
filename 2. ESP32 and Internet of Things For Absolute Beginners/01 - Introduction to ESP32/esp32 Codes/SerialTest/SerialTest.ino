int i = 30;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print("Welcome to ESP32 Serial Communication, i = ");
  Serial.write(i);
  Serial.println("");
  i++;
  delay(1000);  // number of mseconds
}
