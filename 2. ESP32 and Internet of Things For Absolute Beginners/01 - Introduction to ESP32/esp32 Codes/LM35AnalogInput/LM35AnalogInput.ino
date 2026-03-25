void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int a = analogRead(A2);
  Serial.println(a*0.6347); delay(2000);
}
