#include <ESP8266WiFi.h>
#include "DHT.h"

const char* ssid = "arboreo";
const char* password = "cogumelo";
const char* host = "192.168.0.105";
String postPath = "/garden/v1/parameter";

#define pinHumidity A0
#define DHTTYPE DHT11

// DHT Sensor
const int pinDHT = 4;
// Initialize DHT sensor.
DHT dht(pinDHT, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(20000);

  dht.begin();
  pinMode(pinHumidity, INPUT);

  connectWifi();

  postHumiditySoil();
  postHumidityAir();
  postTempAir();

  Serial.println("ESP8266 in sleep mode");
  ESP.deepSleep(30 * 60 * 1000000);

}

void connectWifi(){
  Serial.print("Connecting to ");
  Serial.println(ssid);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //using deepsleep, dont use looping
}

void postHumidityAir(){
  delay(5000);
  float humidity = dht.readHumidity();
  String pubString = "{\"name\": \"humidityAir\", \"value\": "+ String(humidity) + ", \"measure\": \"percent\"}";
  String pubStringLength = String(pubString.length(), DEC);
  post(pubString, pubStringLength);
}

void postTempAir(){
  delay(5000);
  float temp = dht.readTemperature();
  String pubString = "{\"name\": \"tempAir\", \"value\": "+ String(temp) + ", \"measure\": \"celsius\"}";
  String pubStringLength = String(pubString.length(), DEC);
  post(pubString, pubStringLength);
}

void postHumiditySoil(){
  delay(5000);
  int valueParam = readHumidity();
  String nameParam = "humidity";
  String measureParam = "percent";
  String pubString = "{\"name\": \" "+ nameParam + "\", \"value\": "+ valueParam + ", \"measure\": \" "+ measureParam + "\"}";
  String pubStringLength = String(pubString.length(), DEC);
  post(pubString, pubStringLength);
}

void post(String pubString, String pubStringLength) {
  WiFiClient client;
  const int httpPort = 8080;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print(pubString);
  Serial.print("Requesting POST: ");
  // Send request to the server:
  client.println("POST " + postPath +" HTTP/1.1");
  client.println("Host: garden.com");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(pubStringLength);
  client.println();
  client.print(pubString);
  client.println();
  delay(500); // Can be changed
  
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
}

int readHumidity(){
    int humidity = analogRead(pinHumidity);
    int percent = map(humidity, 1023, 0, 0, 100);
    Serial.print("humidity = ");
    Serial.print(percent);
    return percent;
}

