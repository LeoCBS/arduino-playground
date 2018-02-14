#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 14

OneWire oneWire(ONE_WIRE_BUS);


DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;

void setup(void)
{
  Serial.begin(115200);
  sensors.begin();
  Serial.println("searching sensores DS18B20...");
  Serial.print("sensors found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" sensores.");
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("sensors not found !"); 
  Serial.print("address sensor: ");
  show_sensor_address(sensor1);
  Serial.println();
  Serial.println();
  
}

void show_sensor_address(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void loop()
{
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensor1);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  
  delay(3000);
}
