
int humidity;

#define pinLedGreen 10
#define pinLedRed 8

void setup() {
  Serial.begin(9600);
  pinMode(pinLedGreen, OUTPUT);
  pinMode(pinLedRed, OUTPUT);

}

void loop() {
  humidity = analogRead(A0);
  int percent = map(humidity, 1023, 0, 0, 100);

  Serial.print(percent);
  Serial.println("%");
  if (percent <= 70)
  {
    digitalWrite(pinLedRed, HIGH);
    //digitalWrite(pinLedGreen, LOW);
  }
  else
  {
    digitalWrite(pinLedRed, LOW);
    //digitalWrite(pinLedGreen, HIGH);
  }
  delay(5000);

}
