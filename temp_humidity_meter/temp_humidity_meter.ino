#include <dht11.h>

// protoboard project https://circuits.io/circuits/4921240-humidity-temp-meter

dht11 sensor;

#define pinLedHumd 5
#define pinLedTemp 4

void setup() {
  Serial.begin(9600); //define serial speed (9600 bps)
  pinMode(pinLedHumd,OUTPUT);
  pinMode(pinLedTemp,OUTPUT);
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
 
  float humidity = (float)sensor.humidity;
  float temp = (float)sensor.temperature;
  
  Serial.print("Humidty (%): ");
  Serial.println(humidity, 2);
  checkLed(humidity, 50.0, pinLedHumd);
  
  Serial.print("Temperature (graus Celsius): ");
  Serial.println(temp, 2);
  checkLed(20.0, temp, pinLedTemp);
  
  delay(2000);
}

void checkLed(float param1, float param2, int pin){
  if (param1 > param2){
    digitalWrite(pin,HIGH);
    delay(1000);
    digitalWrite(pin,LOW);
  }
}

