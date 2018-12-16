



int get_value(int pin){
  double analogvalue = 0;

  for (int t=1;t<101;t++){
    analogvalue = analogvalue + analogRead(pin);
   // Serial.println(analogvalue);
   // Serial.println();
  }

  return int( (analogvalue / 100));
  
}






// The interrupt routine
void onPulse()
{
    //used to measure time between pulses.
    lastTime = pulseTime;
    pulseTime = micros();

    //pulseCounter
    pulseCount++;

    //Calculate flow
    GPM = (3600000000.0 / (pulseTime - lastTime))/ppg;

    //Find total gallons
    elapsedGal=oldGal + (1.0*pulseCount/ppg);

    if ((elapsedGal-oldGal) > 100){
      EEPROM.put(0,elapsedGal);
      oldGal = elapsedGal;
      pulseCount = 0;
     // Serial.println("UPDATED EEPROM");    //Debug only!!!!!!!!!!!!!!!!!!!!!!!!!!! remove for production
    }


}
