/*
TX  TO D3
RX  TO D4
Vcc    3.3V
Gnd    gnd */
#define BLYNK_TEMPLATE_ID "---------"                    //change template ID
#define BLYNK_TEMPLATE_NAME "---------"                     //change template name
#define BLYNK_AUTH_TOKEN "-----------------"//change token

// WiFi credentials
char auth[] = BLYNK_AUTH_TOKEN;
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004Tv30.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// NTP settings
const char* ntpServerName = "pool.ntp.org";
const int timeZone = 5.5;  // Adjust according to your timezone

// PZEM-004T settings
PZEM004Tv30 pzem(D3, D4);  //assign pins to sensor

// Reset interval in seconds (60 days)
const unsigned long resetInterval = 60UL * 24UL * 60UL * 60UL;

// Variables to store last reset time
unsigned long lastResetTime = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServerName, timeZone * 3600);

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Initialize NTP client
  timeClient.begin();
  Blynk.begin(auth, ssid, password);
}

void loop() {
  timeClient.update();

  // Check if it's time to reset PZEM-004T
  if (millis() - lastResetTime >= resetInterval * 1000UL) {
    pzem.resetEnergy();
    lastResetTime = millis();
  }

  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();

  Serial.print("Voltage: ");
  Serial.print(voltage);Blynk.virtualWrite(V1, voltage);
  Serial.print(" V, Current: ");
  Serial.print(current);Blynk.virtualWrite(V2, current); 
  Serial.print(" A, Power: ");
  Serial.print(power);Blynk.virtualWrite(V3, power);
  Serial.print(" W, Energy: ");
  Serial.print(energy);Blynk.virtualWrite(V4, energy);
  Serial.println(" kWh");

  delay(4000);
}

