//Json broker for use with ubidots.com service
//picks up json sentences from HTTP enabled devices and sends them to ubidots

//
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

const char* TOKEN = "TOKEN"; // Put here your TOKEN
const char* DEVICE_LABEL = "JSON_Broker"; // Your device label
const char* VARIABLE_LABEL = "sample_data"; // Your variable label
const char* USER_AGENT = "ESP8266";
const char* VERSION = "1.0";
const char* HTTPSERVER = "things.ubidots.com";


const byte device_count = 3; // number of devices to poll




unsigned long api_mtbs = 10000; //mean time between api requests 3 sec
unsigned long api_due_time = 0;
unsigned int localPort = 8888;  // local port to listen for UDP packets


int status = WL_IDLE_STATUS;
int httpCode = 0;
int HTTPPORT = 80;
int TT = 0;

byte clientIndex = 0;

int8_t wifiquality = 0;

bool connectivity = false;

String API_addr [10]; 
String errorcode = "Status Initializing";
String HTTPerrorcode = "Status Initializing";
String system_status = "";
String fmtTime = "";
String jsonResult [10];


WiFiUDP Udp;

WiFiClient wifiClient;

ESP8266WebServer server(80);


void handleRoot() {
  String messagetext = "HTTP json Broker v0.8  try /api or /api/1 , api/2 for individual sensors to get data ---- ";
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

  Serial.begin(115200); 

  pinMode(2, OUTPUT);

  Serial.println("JSON Broker 1.0 Starting.....");

  WiFi.hostname("HTTP_UBIDOT_BRKR");

  InitWiFi();

  for (int i = 1;i<11;i++){
      jsonResult[i] = "-NO DEVICE-"; 
  } 

  API_addr[1]= "http://192.168.42.185/api";  //addresses for fetching HTTP json
  API_addr[2]= "http://192.168.42.186/api";
  API_addr[3]= "http://192.168.42.188/api"; 
  
 
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }


  server.on("/", handleRoot);


  server.on("/api", [](){

    String package = "";
    
    for (int i = 1;i<11;i++){    
      package += String(i);
      package += " : ";
      package += jsonResult[i];
      package += "\n";
    } 
  server.send(200, "application/json",package);     
  });

  server.on("/api/1", [](){
    String package = jsonResult[1];
    server.send(200, "application/json",package);      
  }); 
  
  server.on("/api/2", [](){
    String package = jsonResult[2];
    server.send(200, "application/json",package);      
  });
  
  server.on("/api/3", [](){
    String package = jsonResult[3];
    server.send(200, "application/json",package);     
  });
  
  server.on("/api/4", [](){
    String package = jsonResult[4];
    server.send(200, "application/json",package);      
  });
  
  server.on("/api/5", [](){
    String package = jsonResult[5];
    server.send(200, "application/json",package);      
  });
  
  server.on("/api/6", [](){
    String package = jsonResult[6];
    server.send(200, "application/json",package);     
  });
  
  server.on("/api/7", [](){
    String package = jsonResult[7];
    server.send(200, "application/json",package);      
  });
  
  server.on("/api/8", [](){
    String package = jsonResult[8];
    server.send(200, "application/json",package);      
  });
  
  server.on("/api/9", [](){
    String package = jsonResult[9];
    server.send(200, "application/json",package);     
  });
  
  server.on("/api/10", [](){
    String package = jsonResult[10];
    server.send(200, "application/json",package);   
  });
    


  server.onNotFound(handleNotFound);


  server.begin();
  
  Serial.println("HTTP server started");

  
 }


void loop() {


   status = WiFi.status();
   if ( status != WL_CONNECTED) {
      reconnectWIFI();  
   }

    TT++;
    
    if (TT > api_mtbs) { TT = 0;
    
      clientIndex ++;
      if (clientIndex > device_count) clientIndex = 1;
  
      interrogate(clientIndex);     
        
      send_data(jsonResult[clientIndex]);
  
      digitalWrite(2,0);
  
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
  
// Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      
      while (WiFi.status() != WL_CONNECTED) {  
        delay(500);
        digitalWrite(2,! digitalRead(2));
        Serial.print("waiting for wifi.........");
        Serial.print(">");        
        connectivity = false;
      }
      Serial.println("WiFi reconnected");
      Serial.println("IP address: ");
      String (ip) = getIpString (WiFi.localIP());
      Serial.println(ip);
      connectivity = true;
    }
}

