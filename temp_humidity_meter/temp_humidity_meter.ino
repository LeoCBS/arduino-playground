#include <dht11.h>

// protoboard project https://circuits.io/circuits/4921240-humidity-temp-meter

dht11 sensor;

#define pinLed 5

void setup() {
  Serial.begin(9600); //define serial speed (9600 bps)
  pinMode(pinLed,OUTPUT);
}

void loop() {
  Serial.print("reading sendor pin 2: ");
  int chk = sensor.read(2);
  switch (chk) {
    case DHTLIB_OK:
      Serial.println("OK");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("checksum error");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("Tempo esgotado");
      break;
    default:
      Serial.println("Erro desconhecido");
  }
  Serial.print("Humidty (%): ");

  float humidity = (float)sensor.humidity;
  Serial.println(humidity, 2);
  if (humidity > 20.0){
    digitalWrite(pinLed,HIGH);
    delay(1000);
    digitalWrite(pinLed,LOW);
  }
  Serial.print("Temperature (graus Celsius): ");
  Serial.println((float)sensor.temperature, 2);
  delay(2000);
}
