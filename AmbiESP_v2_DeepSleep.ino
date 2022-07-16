// Digital Pin to GPIO mapping for Wemos D1 mini :

#define D0  16  // D0  => GPIO16
#define D1   5  // D1  => GPIO05 (SCL)
#define D2   4  // D2  => GPIO04 (SDA)
#define D3   0  // D3  => GPIO00
#define D4   2  // D4  => GPIO02 (LED builtin)
#define D5  14  // D5  => GPIO14
#define D6  12  // D6  => GPIO12
#define D7  13  // D7  => GPIO13
#define D8  15  // D8  => GPIO15
#define D9   3  // D9  => GPIO03 (RX)
#define D10  1  // D10 => GPIO01 (TX)

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <stdint.h>
#include <WEMOS_SHT3X.h>
SHT3X sht30(0x45);

#include <Wire.h>
#include <MAX44009.h>
MAX44009 light;

#include "Adafruit_VEML6070.h"
Adafruit_VEML6070 uv = Adafruit_VEML6070();
// Config /////////////////////////////
#define STASSID "SkyNet II"
#define STAPSK  "19820730"
const char* mqtt_server = "10.77.2.10";
const int sleepSeconds = 60;
#define BATT_LEVEL A0
#define ANALOG_READ_SAMPLES 100
#define RAIN_SENSOR D5
///////////////////////////////////////

const char* ssid = STASSID;
const char* password = STAPSK;
float temp, humi, light1, light2, uva, uvb, batterylvl;
int rain;
char str_temp[10], str_humi[10], str_light1[10], str_light2[10], str_uva[10], str_uvb[10], str_batt[10], str_rain[10];
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(D0, WAKEUP_PULLUP);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, HIGH);
  Serial.begin(115200);
  Wire.begin();
/*    if(light.begin())
    {
        Serial.println("Could not find a valid MAX44009 sensor, check wiring!");
    while(1);
  }
  uv.begin(VEML6070_1_T); */
  pinMode(RAIN_SENSOR, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  delay(1000);
  client.setServer(mqtt_server, 1883);

  // Main Code
  //Get vars
  sht30.get();
  temp = sht30.cTemp;
  humi = sht30.humidity;
  batterylvl = gauge_reading();
  rain = digitalRead(RAIN_SENSOR);
  //light1 = light.get_lux();
  //light2 =
  //uva = uv.readUV();
  //uvb =

  //Convert to string
  dtostrf(temp, 1, 2, str_temp);
  dtostrf(humi, 1, 2, str_humi);
  dtostrf(batterylvl, 1, 2, str_batt);
  dtostrf(rain, 1, 0, str_rain);
  delay(500);
  //Send data to mqtt
  client.publish("/home/AmbiESP/temperature", str_temp);
  client.publish("/home/AmbiESP/humidity", str_humi);
  client.publish("/home/AmbiESP/batt_lvl", str_batt);
  client.publish("/home/AmbiESP/rain", str_rain);
  Serial.print("Temp: ");
  Serial.println(str_temp);
  Serial.print("Humi: ");
  Serial.println(str_humi);
  Serial.print("Batt lvl: ");
  Serial.println(str_batt);
  Serial.print("Rain: ");
  Serial.println(str_rain);
  delay(2000);
  digitalWrite(D4, LOW);
  ESP.deepSleep(sleepSeconds * 1000000);
}

void loop() {
}
