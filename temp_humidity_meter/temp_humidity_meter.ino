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
  checkLed(humidity, 20.0, pinLedHumd);
  
  Serial.print("Temperature (graus Celsius): ");
  Serial.println(temp, 2);
  checkLed(temp, 20.0, pinLedTemp);
  
  delay(2000);
}

void checkLed(float meter, float limit, int pin){
  if (meter > limit){
    digitalWrite(pin,HIGH);
    delay(1000);
    digitalWrite(pin,LOW);
  }
}

