void checkRelays(){             //ALARMS AND RELAY CONTROL

  float AmpsRelayOn = 0;
  float AmpsRelayOff = 0;







graphTime++;
if (graphTime > graphSec) {

  graphTime = 0;
   
  for (int t=(1); t<100 ; t++){
    Gvolt[t] = Gvolt[t+1];
  } 

  Gvolt[100] = int (v*10);

  if (Gvolt[100] > 300) Gvolt[100] = 300;
  if (Gvolt[100] < 200) Gvolt[100] = 200;

  
  for (int t=(1); t<100 ; t++){
    Gamp[t] = Gamp[t+1];
  } 

  float tempF = a;
  tempF = tempF * .6667;
  tempF = tempF + 100;
  Gamp[100] = int (tempF);

  if (Gamp[100] > 200) Gamp[100] = 200;
  if (Gamp[100] < 0) Gamp[100] = 0;
  
}







  

  
  if (cycleOn == true ){
    tick2 ++;
    Serial.print( "Time to relay test : " );
    Serial.println( (relayCheck - tick2) ); 

    
  }
  
  if (tick2 > relayCheck){
    
    tick2 = 0;

    if (v < relay_Vcutout){ 
        
        if (relayOn == true){
          
          AmpsRelayOn = a;
          relayOn = false;
          digitalWrite(relay,LOW); 
          
          delay(300); //-------------------------500
          
          getData();
          
          AmpsRelayOff = a;
               
        } else {
          
          AmpsRelayOff = a;
          relayOn = true;
          digitalWrite(relay,HIGH); 
          
          delay(300); //-------------------------500
          
          getData();
          
          AmpsRelayOn = a;       
        }
        
      relayOn=false;
      float ampsDiff = AmpsRelayOn - AmpsRelayOff;
      if (ampsDiff > relay_minAmps){
        
        relayOn = true;
      
        if (v > relay_Vcutout) {
             relayOn = false;
             cycleOn = false;
        }
      }    
    }    
  }
   

  if (v < Vcycle_Reset) {
    relay_Vcutout = relay_Vcutout1;   //reset target to high target after reaching "nighttime" floor
  }


  if (v < relay_Vcutin) {
    cycleOn = true;
  }


  if (v > relay_Vcutout) {
    relay_Vcutout = relay_Vcutout2;  //drop down target to low target after reaching high target
    digitalWrite(relay,LOW);  //redundant safety
    relayOn = false;  
    cycleOn = false;  
  }


  if (cycleOn == false) relayOn=false;

  if (relayOn == true) digitalWrite(relay,HIGH);
  if (relayOn == false) digitalWrite(relay,LOW);
  
}

