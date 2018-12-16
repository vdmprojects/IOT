void interrogate(byte c_Idx){
  
      jsonResult[c_Idx] = getData(API_addr[c_Idx]);
      //if (jsonResult == "") jsonResult = "{Data Not Recieved From Sensor}";

      // Prepare a JSON system status string
      system_status = "";
      
      system_status = "{";  
      system_status += "\"client\":";   system_status += String(c_Idx); 
      system_status += " , ";
      system_status += "\"status\":";   system_status += httpCode; 
      system_status += "}";
}


String getData(String APIstring) {

  String payload = "";
  
  Serial.print("Get data from : ");

  Serial.println(APIstring);
  

  if (WiFi.status() == WL_CONNECTED) {
    
    HTTPClient http;  //Object of class HTTPClient
    
    http.begin(APIstring);
    
    httpCode = http.GET();
    
    Serial.print ("HTTP Code : ");
    Serial.println(httpCode);
    
    //Check the returning code                                                                  
    if (httpCode > 0) {
      digitalWrite(2,1);
      payload = http.getString();  // Get the request response payload
      HTTPerrorcode = "  HTTP GET Status OK";
      failcount = 0;
      Serial.println(payload);
      Serial.println("=================");
     } else {
      HTTPerrorcode = "  HTTP Fetch fail : Error Code " + String(httpCode);
      failcount ++;
      if (failcount > 25) ESP.restart();
     }
        
     http.end();   //Close connection  
    }

   
   return (payload); 
}








byte GetSerData () {

  flow = 0;
  gallons = 0;
  pressure = 0;
  pumpAmps = 0;
  byte valid = 0;
  

  while (Serial.available() > 0) {

    if (Serial.read() == '{') {
        Serial.println("BEGIN");
        
      
        // look for the next valid integer in the incoming serial stream:
        int mantissa = Serial.parseInt();
        // do it again:
        int decimal = Serial.parseInt();
        // do it again:
        flow = float(mantissa + (float(decimal)/100));

        // look for the next valid integer in the incoming serial stream:
        mantissa = Serial.parseInt();
        // do it again:
        decimal = Serial.parseInt();
        // do it again:
        gallons = float(mantissa + (float(decimal)/100));

        // look for the next valid integer in the incoming serial stream:
        mantissa = Serial.parseInt();
        // do it again:
        decimal = Serial.parseInt();
        // do it again:
        pressure = float(mantissa + (float(decimal)/100));

        // look for the next valid integer in the incoming serial stream:
        mantissa = Serial.parseInt();
        // do it again:
        decimal = Serial.parseInt();
        // do it again:
        pumpAmps = float(mantissa + (float(decimal)/100));

        // look for the next valid integer in the incoming serial stream:
        mantissa = Serial.parseInt();
        // do it again:
        decimal = Serial.parseInt();
        // do it again:
        valid = mantissa+decimal;
    }

    if (Serial.read() == '}') {
      
      Serial.println("END");

      while(Serial.available()){delay(0); Serial.read();}
      
      if (valid == 200){
        return (1);
      } else {
        return (0);
      }

      
    }
    
  }
  
}







