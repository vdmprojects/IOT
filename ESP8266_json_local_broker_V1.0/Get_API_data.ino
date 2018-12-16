void interrogate(byte c_Idx){
  
      jsonResult[c_Idx] = getData(API_addr[c_Idx]);
      //if (jsonResult[c_Idx] == "") jsonResult[c_Idx] = "{Data Not Recieved From Sensor}";

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










