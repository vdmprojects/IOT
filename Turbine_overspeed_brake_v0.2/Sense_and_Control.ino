void GET_RPM(){
  
    pulseTotal = 0;
    
    for (int t=0;t<2;t++) {   //number of times for sampling
      pulseHigh = pulseIn(pulsePin,HIGH);
      pulseLow = pulseIn(pulsePin,LOW);
      pulseTotal = pulseTotal + pulseHigh + pulseLow; // Time period of the pulse in microseconds
      Serial.println(t);
    }

    pulseTotal = pulseTotal / 5;
    
    frequency=1000000/pulseTotal; // Frequency in Hertz (Hz)
    
    Serial.print("Frequency : ");
    Serial.println(frequency);
}



void TURBINE_CONTROL(){

  if (frequency > maxFreq){
      Serial.println("OVER FREQUENCY!");
      HARD_BRAKE();
      
   } else {

      if (frequency > softThreshold){
        SOFT_BRAKE();
      } else {
        
        freqTrend = frequency - last_frequency;
        
        if (frequency > softThreshold - hysteresis){
          
            if ((frequency + freqTrend) > maxFreq){
              Serial.println("Predictive overrun!");
              SOFT_BRAKE();
            }
         }
      
      }
   }

    if (frequency < (softThreshold - hysteresis)){
      Serial.println("RPM OK");
      BRAKE_OFF();
    } else{
      Serial.println("RPM WARNING");
    }

  
}

