void getData()
{
  Serial.println("Collecting Analog data.");

  V=0;
  A=0;

  int16_t adc0, adc1, adc2, adc3;
  for (int count = 0; count < 10; count++) {
      a=0;
      v=0;
    
      ads1115.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
      adc0 = ads1115.readADC_SingleEnded(3);
    
       ads1115.setGain(GAIN_SIXTEEN);     // 16x gain  +/- 0.256V  1 bit = 0.125mV
       delay (10);
       adc1 = ads1115.readADC_Differential_0_1();
       
    //  adc2 = ads1115.readADC_SingleEnded(2);
    //  adc3 = ads1115.readADC_SingleEnded(3);
    //  Serial.print("AIN0: "); Serial.println(adc0);
    //  Serial.print("AIN1: "); Serial.println(adc1);
    //  Serial.print("AIN2: "); Serial.println(adc2);
    //  Serial.print("AIN3: "); Serial.println(adc3);
    //  Serial.println(" ");
      
      v = adc0;
      v = v / 7979;  //direct read in mv
      v = v * 7.5;  //7.5:1 voltage divider
      V=V+v;
    
      a = adc1;
      a = a / 12.5;
      A=A+a;
    }
    
  a = A / 10;
  v = V / 10;

  if (v > 5) check_time2 = 1;

  Serial.print("Amperage ");
  Serial.println(a);


  Serial.print("Voltage ");
  Serial.println(v);

  //ALARMS 

  if (v > alarm_voltage) alarm = 1;
  if (v < (alarm_voltage -.1)) alarm = 0;


  if (alarm == 1){
    digitalWrite(buzzer, !digitalRead(buzzer)); // make buzzer pin output oscillate
  } else {
    digitalWrite(buzzer,0);
  }
  


  if (v>30) relayOn=false;

  if (relayOn == true) digitalWrite(relay,HIGH);
  if (relayOn == false) digitalWrite(relay,LOW);
  


}



// ------------------------ON Board ADC Example:
//  for (int count = 0; count < 10; count++) {
//    volt = volt  + analogRead(0) ;
//    delay (100);
//  }
//  
//  r = volt / 10;
//
//  Serial.print("Raw");
//  Serial.println(r);
//  r = r - 7;
//  float v = ((r * 30) /1024);







// ------------------------ADS1x15 ADC Example:
//
//  int16_t adc0, adc1, adc2, adc3;
// 
//  adc0 = ads1015.readADC_SingleEnded(0);
//  adc1 = ads1015.readADC_SingleEnded(1);
//  adc2 = ads1015.readADC_SingleEnded(2);
//  adc3 = ads1015.readADC_SingleEnded(3);
//  Serial.print("AIN0: "); Serial.println(adc0);
//  Serial.print("AIN1: "); Serial.println(adc1);
//  Serial.print("AIN2: "); Serial.println(adc2);
//  Serial.print("AIN3: "); Serial.println(adc3);
//  Serial.println(" ");

  
//  Serial.println("Getting differential reading from AIN0 (P) and AIN1 (N)");
//  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV)");
//  ads1015.begin();
//
// 
//  int16_t results;
// 
//  results = ads1015.readADC_Differential_0_1();
//  Serial.print("Differential: "); Serial.print(results); Serial.print("("); Serial.print(results * 3); Serial.println("mV)");


//Adjusting Gain
//To boost small signals, the gain can be adjusted on the ADS1x15 chips in the following steps:
//
// ads1015.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV (default)
// ads1015.setGain(GAIN_ONE);     // 1x gain   +/- 4.096V  1 bit = 2mV
// ads1015.setGain(GAIN_TWO);     // 2x gain   +/- 2.048V  1 bit = 1mV
// ads1015.setGain(GAIN_FOUR);    // 4x gain   +/- 1.024V  1 bit = 0.5mV
// ads1015.setGain(GAIN_EIGHT);   // 8x gain   +/- 0.512V  1 bit = 0.25mV
// ads1015.setGain(GAIN_SIXTEEN); // 16x gain  +/- 0.256V  1 bit = 0.125mV
//
//adsGain_t getGain(void)
//
//Reads the current gain value (default = 2/3x)


