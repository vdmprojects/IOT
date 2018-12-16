
String getData(String APIstring) {

  String payload = "";
  
  Serial.print("Get data from : ");

  Serial.println(APIstring);
  

  if (WiFi.status() == WL_CONNECTED) {
    
    HTTPClient http;  //----------------------------------Object of class HTTPClient
    
    http.begin(APIstring);
    
    int httpCode = http.GET();
    
    Serial.print ("HTTP Code : ");
    Serial.println(httpCode);
    
    //Check the returning code                                                                  
    if (httpCode > 0) {

      failcount = 1;
      
      // -------------------------------------------Get the request response payload
      
      payload = http.getString();
      
      // --------------------------------------------Parsing

      const size_t bufferSize = JSON_OBJECT_SIZE(4) + 100;
      
      DynamicJsonBuffer jsonBuffer(bufferSize); 
    
      JsonObject& root = jsonBuffer.parseObject(payload);
      

      T_volts = root["voltage"]; // 24.98)
      volts  =  (4 * volts) + T_volts; //smoothing
      volts = volts / 5;
      
      T_amps = (root["battery_current"]); // -4.63
      amps  =  (4 * amps) + T_amps; //smoothing
      amps = amps / 5;
      
      relay = root["relay_state"]; // 20

      pulse = root["pulse"]; // 1
     
     } else {
      
     failcount++;

     if (failcount > 9) ESP.restart();
 
    }
     
   http.end();   //---------------------------------------Close connection
      
 }

    screen();

   return (payload); 
   
}






String getMessage(String A) {

  String M = "";

  message = "";

  Serial.print("Get message from : ");

  Serial.println(A);
  
   if (WiFi.status() == WL_CONNECTED) {
      
      HTTPClient http;  //-------------------------------------Object of class HTTPClient
      
      http.begin(A);
      
      int httpCode = http.GET();
      
      Serial.print ("HTTP Code : ");
      Serial.println(httpCode);
      
                                                             
      if (httpCode > 0) {
        M = http.getString();
      }
         
     http.end();   //------------------------------------------Close connection
        
   }

  return(M);

}











void Fetch() {
    TT++;

    if (TT > api_mtbs) {

      TT = 0;
  
      String jsonResult = getData(addr[failcount]);   
    
    }
}


