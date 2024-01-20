/* connections
TX  TO D3
RX  TO D4
Vcc    3.3V
Gnd    gnd
I2C LCD CONNECTION  
SDA   D2
SCL   D1
vcc   Vin
Gnd   Gnd */
#define BLYNK_TEMPLATE_ID "TMPL3vIyaqBen"                    //change template ID
#define BLYNK_TEMPLATE_NAME "led blink"                     //change template name
#define BLYNK_AUTH_TOKEN "HsoR99kEWwfdW8dSv83wvxk9eokdXjs0"//change token
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WiFi";                                    // change ssid 
char pass[] = "adgj1357";                               //change password
 
#define BLYNK_PRINT Serial
//import liberaries
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004Tv30.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

PZEM004Tv30 pzem(D3, D4);  //assign pins to sensor

float voltage = 0;
float current = 0;
float power = 0;
float energy = 0;
float pf = 0;
float bill = 0;
float unitprice = 7.75;
unsigned long lastMillis = 0;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.init();
  Wire.setClock(10000);
  lcd.backlight();
  delay(100);
  connectToWiFi();
  lcd.clear();
  
  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(0, 0);
    lcd.print("Connected to WiFi");
    Blynk.run();
  } 
  else if (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(0, 0);
    lcd.print("No Connection");
  } 
  else {
    // Do nothing
  }
}

void loop(){
    voltage = pzem.voltage();
    if( !isnan(voltage) ){
        Serial.print("Voltage: "); Serial.print(voltage,1); Serial.println("V");
    } else {
        Serial.println("Error reading voltage");
    }

    current = pzem.current();
    if( !isnan(current) ){
        Serial.print("Current: "); Serial.print(current,3); Serial.println("A");
    } else {
        Serial.println("Error reading current");   
    }

    power = pzem.power();
    if( !isnan(power) ){
        Serial.print("Power: "); Serial.print(power,0); Serial.println("W");
    } else {
        Serial.println("Error reading power");
    }

     energy = pzem.energy();
    if( !isnan(energy) ){
        Serial.print("Energy: "); Serial.print(energy, 3); Serial.println("kWh");
    } else {
        Serial.println("Error reading energy");
    }

    pf = pzem.pf();
    if( !isnan(pf) ){
        Serial.print("PF: "); Serial.println(pf);
    } else {
        Serial.println("Error reading power factor");  
    }
    bill = energy*unitprice;
    if( !isnan(energy) ){
        Serial.print("bill: "); Serial.print(bill, 3); Serial.println("Rs"); 
    } else {
        Serial.println("Error calculating bill");
    }
    Serial.println();
    print();
       
}

void connectToWiFi() {
  if(WiFi.status() == WL_CONNECTED){
    lcd.setCursor(0,0);lcd.print("connected to WiFi");
  } else if(WiFi.status() != WL_CONNECTED){
    lcd.setCursor(0,0);lcd.print("No Connection");
    } else{pass;
      }

  int attempts = 0;
  while (attempts < 2) {
    WiFi.begin(ssid, pass);
    Serial.print("Connecting to WiFi...");
    lcd.setCursor(0,0);
    lcd.print("connecting to wifi ");

    int attemptsCount = 0;
    while (WiFi.status() != WL_CONNECTED && attemptsCount < 10) {
      delay(500);
      Serial.print(".");
      lcd.setCursor(attemptsCount,1);
      lcd.print(".");
      attemptsCount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi!");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CONNECTED TO WiFi!");
      delay(2000);
      lcd.setCursor(0,1);
      lcd.print("CONNECTING TO BLYNK...");
      Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
      delay(2000);
      break;
    } else {
      Serial.println("Connection failed.");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CONNECTION FAILED");
      attempts++;
      delay(2000);
      if (attempts < 2) {
        Serial.println("Retrying...");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("RETRYING...");
        delay(2000);
      } else {
        Serial.println("Failed to connect after 2 attempts, Continuing without WiFi.");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Failed to connect");
        delay(500);
        lcd.setCursor(0,1);
        lcd.print("Continuing without");
        lcd.setCursor(0,2);
        lcd.print("WiFi");
        delay(2000);
        break; 
        // Failed after 2 attempts, exit the loop
      }
    }
  }
}

void print(){

  if (millis() - lastMillis > 3000) {
            lastMillis = millis(); 

            lcd.setCursor(0,1); lcd.print("Voltage:       V");lcd.setCursor(9,1); lcd.print(voltage,1);
              lcd.setCursor(0,2); lcd.print("Current:       A");lcd.setCursor(9,2); lcd.print(current,3); 
                lcd.setCursor(0,3); lcd.print("Power  :       W");lcd.setCursor(9,3); lcd.print(power); 
                  delay(3000);
                lcd.setCursor(0,1); lcd.print("                    ");
              lcd.setCursor(0,2); lcd.print("                    ");
            lcd.setCursor(0,3); lcd.print("                    ");
           delay(200);
            lcd.setCursor(0,1); lcd.print("Energy:        Units");lcd.setCursor(8,1); lcd.print(energy,3);
              lcd.setCursor(0,2); lcd.print("bill  :          Rs");lcd.setCursor(8,2); lcd.print(pf);
                lcd.setCursor(0,3); lcd.print("PF    :            ");lcd.setCursor(8,3); lcd.print(bill);
              delay(2000);
            if(WiFi.status() == WL_CONNECTED){
              delay(500);
                Blynk.virtualWrite(V1, voltage);
                  Blynk.virtualWrite(V2, current);            
                    Blynk.virtualWrite(V3, power);
                  Blynk.virtualWrite(V4, energy);
                Blynk.virtualWrite(V6, pf);  
              Blynk.virtualWrite(V7,bill);
             }else{
              pass;
             }        
          }         
        }


