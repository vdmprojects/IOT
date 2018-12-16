void MODEL_TURBINE(){
  tJet = tJet + ( random (0,12) -6);
  if (tJet < 200) tJet = tJet + 3;
  if (tJet > 1500) tJet = tJet -3;

  tLoad = tLoad + ( random ( 0, (2 * loadRand )) - loadRand);
  if (tLoad < 200) tLoad = tLoad + 5; 
  if (tLoad > 1300) tJet = tLoad -5;

  
  tSpeed = ((5*tSpeed) + tSpeed + tJet - tLoad - brakeLoad)/6;

  if (tSpeed < 900){
    Serial.println("turbine speed low!!");
    if (tSpeed < 100) tSpeed = tJet || (0.2 * tJet - (0.05* (tLoad + brakeLoad)));
  }

  frequency = tSpeed / RPM_mult;

  Serial.println();
  Serial.print("Turbine RPM = ");
  Serial.print(tSpeed);
  Serial.print("    Jet power = ");
  Serial.print(tJet);
  Serial.print("     Load = ");
  Serial.print(tLoad);
  Serial.println();
  Serial.println();
  

  //delay (300);
  
}



//float tSpeed = 1000;
//float tLoad = 1000;
//float tJet=1000;
//float brakeLoad = 0;

