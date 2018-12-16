void SOFT_BRAKE(){
  Serial.print("SOFT BRAKING  brakeLoad = ");

 float BL = maxFreq - frequency;
 BL =  1-(BL / brakeRange);
 int loadCycle = int (100*BL);
 if (loadCycle < 50) loadCycle = 50;
 
 for (int t = 0; t<10;t++){
    digitalWrite (brake1Pin, 1);
    delay(loadCycle);
    digitalWrite (brake1Pin, 0);
    delay(100-loadCycle);
 } 

  Serial.print(BL);
 

    
// /* turbine modeling code - Remark out for production 
brakeLoad =  loadCycle * 10;
Serial.print("  : ");
Serial.println(loadCycle);
// */

  
}




void HARD_BRAKE(){
  Serial.println("*******HARD BRAKING********");
  digitalWrite (brake1Pin,1);

// /* turbine modeling code - Remark out for production 
  brakeLoad = 1200;
// */

}




void BRAKE_OFF(){
  Serial.println("Brake off.");
  digitalWrite (brake1Pin,0);
  
// /* turbine modeling code - Remark out for production 
  brakeLoad = 0;
// */
  
}



