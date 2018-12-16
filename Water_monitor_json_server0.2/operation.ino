void operate(byte c_Idx){
  
      const size_t bufferSize = JSON_OBJECT_SIZE(4) + 100;

      Volts = 0;
      Amps = 0;

  if (HTTPerrorcode == "  HTTP GET Status OK"){

      if (c_Idx == 1) {

        DynamicJsonBuffer jsonBuffer(bufferSize); 
      
        JsonObject& root = jsonBuffer.parseObject(jsonResult[c_Idx]);
        
        Volts = root["voltage"]; 
        
        Amps = root["battery_current"]; 
      
        byte relay = root["relay_state"]; 
  
        byte pulse = root["pulse"]; 
         
        //Serial.println(jsonResult[c_Idx]); 
      } 

  }
      
        json = "{";
        json += "\"GPH\":";   json += String(flow);
        json += " , ";
        json += "\"Total_gal\":";   json += String(gallons);
        json += " , ";
        json += "\"Tank\":";   json += String(pressure);
        json += " , ";
        json += "\"pump_load\":";   json += String(pumpAmps);
        json += " , ";
        json += "\"B_V\":";   json += String(Volts);
        json += " , ";
        json += "\"B_A\":";   json += String(Amps);
        json += " , ";
        json += "\"Pump_relay\":";   json += String(rly);
        json += "}";

        //Serial.println("^^^^^input^^^^^");
        Serial.println(json);
        //Serial.println("^^^^^output^^^^^");
  
}

