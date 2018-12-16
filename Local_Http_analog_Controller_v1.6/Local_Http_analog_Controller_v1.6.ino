//#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <Adafruit_ADS1015.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


// CHOOSE ONE
//#include "SSD1306Wire.h"
#include "SH1106Wire.h"


#define WIFI_AP "SSID"
#define WIFI_PASSWORD "WIFI PASSWORD"


// Pins
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 5;
const int SDC_PIN = 4;
//const int SDA_PIN = 4;
//const int SDC_PIN = 0;

const byte buzzer = 0;
const byte relay = 14;
const byte update_interval = 2; //seconds between data samples
const byte graphSec = 2; //this times two is the nuber of seconds to move the graph

const float alarm_voltage = 31;
const float relay_minAmps = .5;

const float relay_Vcutout1 = 29;
const float relay_Vcutout2 = 28;

const float Vcycle_Reset = 25;

const float relay_Vcutin = 26;

const int relaySeconds = 300; //check for relay conditions at this approximate interval under normal operation


const String deviceName = "HTTP Power Monitor v1.5";




//Global Vars------------------------------------------------------


int status = WL_IDLE_STATUS;

unsigned long lastSend;

bool alarm = 0;
bool connectivity = false;
bool relayOn = false;
bool cycleOn = true;
bool pulse = true;

//byte graph[120];
byte tick = 0;
int relayCheck = (relaySeconds / 2);

byte tick2 = (relayCheck - 5);
byte heartbeat = 0;
byte TT= 0;

float r = 0;
float v;
float a;
float V = 0;
float A = 0;
float relay_Vcutout = relay_Vcutout1;

int Gvolt[101];
int Gamp[101];

byte check_time1 = 0;
byte check_time2 = 0;
byte graphTime = 0;


String payload = "No Data Yet";



/////Initializations

// Create display
//SSD1306Wire      display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
SH1106Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);


Adafruit_ADS1115 ads1115(0x4A); 


WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets


WiFiClient wifiClient;


//PubSubClient client(wifiClient);

ESP8266WebServer server(80);




void handleRoot() {
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  delay(0);
  
  snprintf ( temp, 600,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>HTTP PowerMon</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>%s</h1>\
    <p>Uptime: %2d:%02d:%02d</p>\
     <p>Battery Voltage:%f ..... Battery Amperage:%f </p>\   
  </body>\
</html>",

   "HTTP Power Monitor 1.6", hr, min % 60, sec % 60, v, a
  );
  
  server.send ( 200, "text/html", temp );
  check_time1 = 1;
}

void handleGraph() {
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  
  snprintf ( temp, 600,

"<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>HTTP PowerMon</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>%s</h1>\ 
    <p>Uptime: %2d:%02d:%02d</p>\ 
    <p>Voltage:</p>\   
    <img src=\"/volts.svg\" />\
    <p>Battery Amperage:</p>\ 
    <img src=\"/amps.svg\" />\ 
  </body>\
</html>",

   "HTTP Power Monitor Graphing", hr, min % 60, sec % 60
  );
  
  server.send ( 200, "text/html", temp );
  check_time1 = 1;
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
  check_time1 = 1;
}



void setup()
{
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW); // make buzzer pin output low

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW); // make buzzer pin output low
  
  Serial.begin(115200);

  Serial.println(deviceName);
  Serial.println("Operational.");
  
  // Initialize display
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.display();
  
  InitWiFi();

  lastSend = 0;

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());


  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/graph", handleGraph);

  server.on ( "/volts.svg", drawVolts );

  server.on ( "/amps.svg", drawAmps );

  server.on("/api", [](){
    
    server.send(200, "application/json", payload);
    pulse = (!pulse);
    screen();
    
    check_time1 = 1;
    
    Serial.print("API Access!    ");
    Serial.println( payload ); 

  });

  server.onNotFound(handleNotFound);

  server.begin();
  
  Serial.println("HTTP server started");

  for (int t=(1); t<100 ; t++){
    Gvolt[t] = 200 + t;
    Gamp[t] = t*2;
  }
  
}


void loop() {

  status = WiFi.status();
  if ( status != WL_CONNECTED) {
     reconnect();  
  }
  
  TT++;
  if ( TT > 100 ) {
    tock(); 
  }
  
  server.handleClient();
  
  delay(1);
  
}



void InitWiFi()
{
    Serial.println("Connecting to AP ...");
    // attempt to connect to WiFi network
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
    
    delay(500);
    Serial.print("."); 
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 10, deviceName);
    display.drawString(0, 20, "Connecting to ");
    display.drawString(0, 30, WIFI_AP);        
    display.display();

    delay (1000);
    connectivity = false;

    getData();
    screen();
    delay(2000);
    
  }
  
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    String (ip) = getIpString (WiFi.localIP());
    Serial.println(ip);
    connectivity = true;
    display.clear();  
    display.setFont(ArialMT_Plain_10);
    String IP="DHCP " + ip;
    display.drawString(0, 30, IP);     
    display.display();
}


void reconnect() {
 // Loop until we're reconnected
    status = WiFi.status();
    
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        
        delay(1500);
        
        Serial.print(".");       
        display.clear();
        display.setFont(ArialMT_Plain_10);
        display.drawString(0, 10, "Reconnecting to ");
        display.drawString(0, 30, WIFI_AP);        
        display.display();
        connectivity = false;
        delay (1000);
        getData();
        screen();
        delay(2000);
        
      }
      
      Serial.println("Connected to AP");
          connectivity = true;
          display.clear(); 
          display.setFont(ArialMT_Plain_10);
          String (ip) = getIpString (WiFi.localIP());
          String IP="DHCP " + ip;
          display.drawString(0, 30, IP);       
          display.display();
          
      }

 
}


void drawVolts() {

  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"510\" height=\"220\">\n";
  out += "<rect width=\"510\" height=\"220\" fill=\"rgb(0, 0, 0)\" stroke-width=\"2\" stroke=\"blue\" />\n";
  out += "<g stroke=\"blue\">\n";

  sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"2\" />\n", 1, 110, 510, 110);
  out += temp;
  out += "</g>\n";  

  out += "<g stroke=\"cyan\">\n";
  
  for (int x = 1; x < 99; x++) {
    delay(0);
    
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"2\" />\n", x*5, 210 - (2*(Gvolt[x] - 200)), (x*5) + 5, 210 - (2*(Gvolt[x+1] - 200)));
    out += temp;

  }
  out += "</g>\n</svg>\n";

  server.send ( 200, "image/svg+xml", out);
}


void drawAmps() {

  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"510\" height=\"220\">\n";
  out += "<rect width=\"510\" height=\"220\" fill=\"rgb(0, 0, 0)\" stroke-width=\"2\" stroke=\"blue\" />\n";
  out += "<g stroke=\"blue\">\n";
  
  sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"2\" />\n", 1, 110, 510, 110);
  out += temp;
  out += "</g>\n";

  out += "<g stroke=\"yellow\">\n";
  
  for (int x = 1; x < 99; x++) {
      delay(0);
      sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"2\" />\n", x*5, 210 - Gamp[x], (x*5) + 5, 210 - Gamp[x+1]);
      out += temp;
  }
   
  out += "</g>\n</svg>\n";

  server.send ( 200, "image/svg+xml", out);
  //Serial.println(out);
  
}
