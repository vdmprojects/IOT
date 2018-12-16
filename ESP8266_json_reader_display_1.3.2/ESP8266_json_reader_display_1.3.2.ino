
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

//#include "OLEDDisplayUi.h"
#include "images.h"

#include "SSD1306Wire.h"
//#include "SH1106Wire.h"


#define WIFI_AP "SSID"
#define WIFI_PASSWORD "WIFI PASSWORD"


// Pins
const int I2C_DISPLAY_ADDRESS = 0x3c;
//const int SDA_PIN = 0;
//const int SDC_PIN = 2;

const int SDA_PIN = 4; //yellow
const int SDC_PIN = 0;

//const int SDA_PIN = 13; 
//const int SDC_PIN = 12;

 
unsigned long api_mtbs = 3; //mean time between api requests 3 sec

float testVolts = 26;
float volts = 30;
float amps = -100;

float T_volts = 30;
float T_amps = -100;

String errorcode = "";
String message = "          ";


int WFstatus = WL_IDLE_STATUS;
int TT = 2;
int relay = 20;

byte failcount = 1;
byte pulse = 0;
byte heartbeat = 13;
byte msgctr = 11;
byte msgScroll = 0;

byte n = 59;

float earning_24_hours = 0;
float total_hashrate = 0;
float total_hashrate_calculated = 0;
float wallet_balance = 0;
float hash_bar = 0;

char* addr[] = {
  "http://192.168.42.187/msg",
  "http://192.168.42.187/api/1",
  "http://192.168.42.187/api/1",
  "http://192.168.42.220/api/1",
  "http://192.168.42.185/api",
  "http://192.168.42.187/api/1",
  "http://192.168.42.220/api/1",
  "http://192.168.42.187/api/1",
  "http://192.168.42.220/api/1",
  "http://192.168.42.185/api",
  "http://192.168.42.187/api/1"
 
};


  // Create display
 
  SSD1306Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
  //SH1106Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
  
  WiFiClient wifiClient;
  
void setup() {
  
  Serial.begin(115200);

  WiFi.hostname("LOCAL_DISP1");
  
  InitWiFi();

  pinMode (2,OUTPUT); //for some configurations
  digitalWrite (2,HIGH); //for some configurations



  // initialize dispaly
  display.init();
  display.clear();
  display.display();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(255);
  
  screen();
}


void loop() {

  WFstatus = WiFi.status();
  if ( WFstatus != WL_CONNECTED) {
     reconnectWIFI();  
  }
    
  if (amps > 0){
    heartbeat ++;
    if (heartbeat > 26) {
      heartbeat = 1;
      Fetch();
    }
  }else{
    heartbeat --;
     if (heartbeat < 1) {
       heartbeat = 26;
       Fetch();
     }
  }

  if (heartbeat == 2){
    msgctr ++;
    if (msgctr > 13){
      message = getMessage(addr[0]);
      msgctr = 0;
    }

  }

  screen();
  delay(100);
}
    
  



void InitWiFi()
{
    Serial.println("Connecting to AP =");
    // attempt to connect to WiFi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    digitalWrite(2,! digitalRead(2));
    Serial.print(">"); 
  }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    String (ip) = getIpString (WiFi.localIP());
    Serial.println(ip);
}




void reconnectWIFI() {

byte t = 0;
  
// Loop until we're reconnected

  WFstatus = WiFi.status();
  
  if ( WFstatus != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      
      while (WiFi.status() != WL_CONNECTED) {  
        delay(500);
        digitalWrite(2,! digitalRead(2));
        Serial.print("waiting for wifi.........");
        Serial.print(">");        
        t ++;
        if (t>60) ESP.restart();
      }
      Serial.println("WiFi reconnected");
      Serial.println("IP address: ");
      String (ip) = getIpString (WiFi.localIP());
      Serial.println(ip);
  }
}



