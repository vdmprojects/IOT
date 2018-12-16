void prepareJson(byte c_Idx){
  
      const size_t bufferSize = JSON_OBJECT_SIZE(4) + 100;

  if (HTTPerrorcode == "  HTTP GET Status OK"){

    

      if (c_Idx == 1) {

        DynamicJsonBuffer jsonBuffer(bufferSize); 
      
        JsonObject& root = jsonBuffer.parseObject(jsonResult[c_Idx]);
        
        float volts = root["voltage"]; 
        
        float amps = (root["battery_current"]); 
      
        byte relay = root["relay_state"]; 
  
        byte pulse = root["pulse"]; 
         
        Serial.println(jsonResult[c_Idx]);
        
        jsonResult[c_Idx] = "{";  
        jsonResult[c_Idx] += "\"voltage\":";   jsonResult[c_Idx] += String(volts); 
        jsonResult[c_Idx] += " , ";
        jsonResult[c_Idx] += "\"battery_current\":";   jsonResult[c_Idx] += String(amps); 
        jsonResult[c_Idx] += " , ";
        jsonResult[c_Idx] += "\"relay_state\":";   jsonResult[c_Idx] += String(relay); 
        jsonResult[c_Idx] += " , ";
        jsonResult[c_Idx] += "\"pulse\":";   jsonResult[c_Idx] += String(pulse); 
        jsonResult[c_Idx] += "}";

        Serial.println("^^^^^input^^^^^");
        Serial.println(jsonResult[c_Idx]);
        Serial.println("^^^^^output^^^^^");

      }else if (c_Idx == 2){

        Serial.println(jsonResult[c_Idx]);   
    
      }else if (c_Idx == 3){

        Serial.println(jsonResult[c_Idx]);   
    
      }else if (c_Idx == 4){

        Serial.println(jsonResult[c_Idx]);   
    
      }else if (c_Idx == 5){

        Serial.println(jsonResult[c_Idx]);   
   
      }

      
  
  }
  
  
}

