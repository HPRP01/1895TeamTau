void setup() {
  // put your setup code here, to run once:
  pinMode(9, INPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(9) == HIGH)
  {
    digitalWrite(8, HIGH);
    delay(500);
    digitalWrite(8, LOW);
    delay(500);
  }
  else
  {
    digitalWrite(8, LOW);
  }

}
