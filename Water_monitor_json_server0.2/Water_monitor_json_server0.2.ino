//monitors pressure, flow, amperage and battery current / voltage from other sensors and 
//reports water tank status
//gets sensor data from water_monitor AT328P via serial

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#define WIFI_AP "YOURSSID"
#define WIFI_PASSWORD "WIFIPASSWORD"

const byte device_count = 1; // number of devices to poll




unsigned long api_mtbs = 10000; //mean time between api requests 3 sec
unsigned long api_due_time = 0;
unsigned int localPort = 8888;  // local port to listen for UDP packets


int WFstatus = WL_IDLE_STATUS;
int httpCode = 0;
int HTTPPORT = 80;
int TT = 0;

byte clientIndex = 0;
byte failcount = 0;
byte rly = 0;

int8_t wifiquality = 0;

bool connectivity = false;

String API_addr [10]; 
String errorcode = "Status Initializing";
String HTTPerrorcode = "Status Initializing";
String system_status = "";
String fmtTime = "";
String jsonResult [10];
String message[10];
String json ="";

float flow = 0;
float gallons = 0;
float pressure = 0;
float pumpAmps = 0;
float Volts;
float Amps;

WiFiUDP Udp;

WiFiClient wifiClient;

ESP8266WebServer server(80);


void handleRoot() {
  String messagetext = "Water monitor  - try /api to get data ---- ";
  messagetext = messagetext + errorcode;
  server.send(200,"text/plain",messagetext);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

  
void setup() {

  
  Serial.begin(57600); 

  Serial.println("WATER MON Starting.....");

  WiFi.hostname("WATER_MON");

  InitWiFi();

  for (int i = 1;i<11;i++){
      jsonResult[i] = "-NO DEVICE-"; 
  } 

  API_addr[1]= "http://192.168.42.185/api";
  
 
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }


  server.on("/", handleRoot);


  server.on("/api", [](){

    String package = "";
      package += json;
      package += "\n";
  
  server.send(200, "application/json",package);     
  });


  server.onNotFound(handleNotFound);


  server.begin();
  
  Serial.println("HTTP server started");

  
 }


void loop() {


   WFstatus = WiFi.status();
   if ( WFstatus != WL_CONNECTED) {
      reconnectWIFI();  
   }

    TT++;
    
    if (TT > api_mtbs) { TT = 0;
    
      clientIndex ++;
      if (clientIndex > device_count) clientIndex = 1;
  
      interrogate(clientIndex);  

      if (GetSerData() < 1) Serial.println(" invalid serial data");

      operate(clientIndex);
  
      errorcode = HTTPerrorcode;

      HTTPerrorcode = "";
  
      Serial.println("\n\n");
      Serial.print(errorcode);
      Serial.println("\n\n");

    }

    
  
delay (1);
  
server.handleClient();

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
    connectivity = false;
  }
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    String (ip) = getIpString (WiFi.localIP());
    Serial.println(ip);
    connectivity = true;
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
        connectivity = false;
        t ++;
        if (t>60) ESP.restart();
      }
      Serial.println("WiFi reconnected");
      Serial.println("IP address: ");
      String (ip) = getIpString (WiFi.localIP());
      Serial.println(ip);
      connectivity = true;
    }
}

