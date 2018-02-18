#include <ESP8266WiFi.h>
#include "DHT.h"

const char* ssid = "loginhere";
const char* password = "passwordhere";
const char* host = "192.168.0.105";
String postPath = "/garden/v1/parameter/save";

WiFiClient client;

#define pinHumidity A0
#define DHTTYPE DHT11

// DHT Sensor
const int pinDHT = 4;
// Initialize DHT sensor.
DHT dht(pinDHT, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(5000);

  dht.begin();
  pinMode(pinHumidity, INPUT);

  connectWifi();

  //postHumiditySoil();
  //postHumidityAir();
  //postTempAir();

  float temp = dht.readTemperature();
  postDataTago("tempAir", String(temp));
  
  float humidityAir = dht.readHumidity();
  postDataTago("humidityAir", String(humidityAir));
  
  int humiditySoil = readHumidity();
  postDataTago("humiditySoil", String(humiditySoil));

  Serial.println("ESP8266 in sleep mode");
  // first number is hour param
  ESP.deepSleep(1 * 60 * 60000);
  //ESP.deepSleep(60000);

}

void connectWifi() {
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

void postHumidityAir() {
  delay(5000);
  float humidity = dht.readHumidity();
  Serial.println("HUMIDITY AIR= " + String(humidity));
  String pubString = "{\"name\": \"humidityAir\", \"value\": " + String(humidity) + ", \"measure\": \"percent\"}";
  String pubStringLength = String(pubString.length(), DEC);
  post(pubString, pubStringLength);
}

void postTempAir() {
  delay(5000);
  float temp = dht.readTemperature();
  Serial.println("TEMP AIR= " + String(temp));
  String pubString = "{\"name\": \"tempAir\", \"value\": " + String(temp) + ", \"measure\": \"celsius\"}";
  String pubStringLength = String(pubString.length(), DEC);
  post(pubString, pubStringLength);
}

void postHumiditySoil() {
  delay(5000);
  int valueParam = readHumidity();
  Serial.println("HUMIDITY SOIL = " + valueParam);
  String nameParam = "humidity";
  String measureParam = "percent";
  String pubString = "{\"name\": \" " + nameParam + "\", \"value\": " + valueParam + ", \"measure\": \" " + measureParam + "\"}";
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
  client.println("POST " + postPath + " HTTP/1.1");
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

int readHumidity() {
  int humidity = analogRead(pinHumidity);
  int percent = map(humidity, 1023, 0, 0, 100);
  Serial.print("humidity = ");
  Serial.print(percent);
  return percent;
}

String apiKey = "15102988-cf30-4829-a676-3c92632a79af"; //o token vai aqui
const char* server = "api.tago.io";

void postDataTago(String variable, String pubString) {

  const int httpPort = 80;
  if (!client.connect(server, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.println("CONNECTED AT TAGO\n");
  Serial.println("variable = "+variable+ " value = "+ pubString + "\n");

  String postStr = "";

  String postData = "variable="+variable+"&value=" + String(pubString) + "\n";

  postStr = "POST /data HTTP/1.1\n";
  postStr += "Host: api.tago.io\n";
  postStr += "Device-Token: " + apiKey + "\n";
  postStr += "_ssl: false\n";
  postStr += "Content-Type: application/x-www-form-urlencoded\n";
  postStr += "Content-Length: " + String(postData.length()) + "\n";
  postStr += "\n";
  postStr += postData;

  client.print(postStr);
  //Serial.print("%d", postStr);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }


  client.stop();

}

