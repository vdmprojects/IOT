//operates sensors and talks to water monitor json server via Serial
//for ATmega 328P

#define FLOW 3  // D1 Mini D8                          ->  chip pin 2
#define current1Pin A0
#define current2Pin A1

#include <EEPROM.h>


int mVperAmp = 66; // use 185 for 5A Module, 100 for 20A Module and 66 for 30A Module
double RawValue = 0;
int ACSoffset = 2500;


double tempVoltage = 0;
double Amps = 0;
double Pressure = 0;

//Number of pulses, used to measure water
long pulseCount = 0;

//Used to measure flow rate
unsigned long pulseTime, lastTime;

//GPM and TOTGAL
double GPM, elapsedGal, oldGal;

//Number of pulses per wh - found or set on the meter.
int ppg = 10000; //pulses per gallon

String json = "{}";


void setup() {

  pinMode(FLOW, INPUT);
  pinMode(current1Pin, INPUT);
  pinMode(current2Pin, INPUT);

  digitalWrite (FLOW, true); //pullup

  Serial.begin(57600);

  // GPM interrupt attached to IRQ 1 = pin3
  attachInterrupt(1, onPulse, FALLING);



  //init eeprom, one run only
 // elapsedGal = 0;    
 // EEPROM.put(0,elapsedGal);
  //end init eeprom

  EEPROM.get(0,elapsedGal);
  oldGal = elapsedGal;

}




void loop() {

//  Serial.print("Hall sensor in flowmeter = ");
//  Serial.println(digitalRead(FLOW));

  RawValue = get_value(current1Pin);
  Pressure = ((RawValue - 112) / 102.4) * .5;

  //  Serial.print("Pressure = "); // shows the voltage measured
  //  Serial.println(Pressure,3); // the '3' after voltage allows you to display 3 digits after decimal point


  RawValue = get_value(current2Pin);
  tempVoltage = (RawValue / 1024.0) * 5000; // Gets you mV
  Amps = ((tempVoltage - ACSoffset) / mVperAmp);
  Amps = Amps * -1;
  Amps = Amps - 6.9;  //zero offset
  Amps = abs(Amps);
  //Serial.print("Amps = ");
  //Serial.println(Amps);

  //  Serial.print(GPM,2);
  //  Serial.print(" GPM for a total of ");
  //  Serial.println(elapsedGal,2);

  json = "{";
  json += "\"flow\":";   json += String(GPM);
  json += " , ";
  json += "\"gallons\":";   json += String(elapsedGal);
  json += " , ";
  json += "\"pressure\":";   json += String(Pressure);
  json += " , ";
  json += "\"amps\":";   json += String(Amps);
  json += " , ";
  json += "\"valid\": 150.50";
  json += "}";
  
  // json = "{\"flow\":1.21 , \"gallons\":2.22 , \"pressure\":3.23 , \"amps\":4.24}";  //test json
 
  Serial.println(json);




  GPM = 0;

  Serial.println();

  delay(2500);
}
