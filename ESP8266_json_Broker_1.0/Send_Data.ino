void send_data(String json_string) {

// ------------------------------------Just debug messages
  Serial.print( "Sending data : " );
  Serial.println( json_string );
  Serial.println( system_status ); 
  
  if (jsonResult[clientIndex] == ""){
    
    jsonResult[clientIndex] = "{Data Not Recieved From Sensor}";
    
  } else {
    
// ------------------------------------------Send payload
 
  char* body = (char *) malloc(sizeof(char) * 200);
  char* data = (char *) malloc(sizeof(char) * 500);
  /* Space to store values to send */
  char str_val[10];


  /* Builds the body to be send into the request*/ 
  json_string.toCharArray(body,200);
  
  /* Builds the HTTP request to be POST */
  sprintf(data, "POST /api/v1.6/devices/%s", DEVICE_LABEL);
  sprintf(data, "%s HTTP/1.1\r\n", data);
  sprintf(data, "%sHost: things.ubidots.com\r\n", data);
  sprintf(data, "%sUser-Agent: %s/%s\r\n", data, USER_AGENT, VERSION);
  sprintf(data, "%sX-Auth-Token: %s\r\n", data, TOKEN);
  sprintf(data, "%sConnection: close\r\n", data);
  sprintf(data, "%sContent-Type: application/json\r\n", data);
  sprintf(data, "%sContent-Length: %d\r\n\r\n", data, dataLen(body));
  sprintf(data, "%s%s\r\n\r\n", data, body);

  /* Initial connection */
  wifiClient.connect(HTTPSERVER, HTTPPORT);

  /* Verify the client connection */
  if (wifiClient.connect(HTTPSERVER, HTTPPORT)) {
        Serial.println(F("Posting your variables: "));
        Serial.println(data);
        /* Send the HTTP Request */
        wifiClient.print(data);
  }

//  Serial.print("response :");

  /* While the client is available read the response of the server */
  while (wifiClient.available()) {
        char c = wifiClient.read();
        Serial.write(c);
  }


   /* Free memory */
  free(data);
  free(body);
    
  }
  

  // Send system_status
  char* body = (char *) malloc(sizeof(char) * 200);
  char* data = (char *) malloc(sizeof(char) * 500);
  
  /* Space to store values to send */
  char str_val[10];


  /* Builds the body to be send into the request*/ 
  system_status.toCharArray(body,200);
  
  /* Builds the HTTP request to be POST */
  sprintf(data, "POST /api/v1.6/devices/%s", DEVICE_LABEL);
  sprintf(data, "%s HTTP/1.1\r\n", data);
  sprintf(data, "%sHost: things.ubidots.com\r\n", data);
  sprintf(data, "%sUser-Agent: %s/%s\r\n", data, USER_AGENT, VERSION);
  sprintf(data, "%sX-Auth-Token: %s\r\n", data, TOKEN);
  sprintf(data, "%sConnection: close\r\n", data);
  sprintf(data, "%sContent-Type: application/json\r\n", data);
  sprintf(data, "%sContent-Length: %d\r\n\r\n", data, dataLen(body));
  sprintf(data, "%s%s\r\n\r\n", data, body);

  /* Initial connection */
  wifiClient.connect(HTTPSERVER, HTTPPORT);

  /* Verify the client connection */
  if (wifiClient.connect(HTTPSERVER, HTTPPORT)) {
        Serial.println(F("Posting your variables: "));
        Serial.println(data);
        /* Send the HTTP Request */
        wifiClient.print(data);
  }

  /* While the client is available read the response of the server */
  while (wifiClient.available()) {
        char c = wifiClient.read();
        Serial.write(c);
  }
   /* Free memory */
  free(data);
  free(body);
  
  Serial.println("Data Sent");
  system_status = "";
  
}




int dataLen(char* variable) {
  uint8_t dataLen = 0;
  for (int i = 0; i <= 500; i++) {
    if (variable[i] != '\0') {
      dataLen++;
    } else {
      break;
    }
  }
  return dataLen;
}


// Prepare a JSON payload string
//    String system_status = "{";  
//    payload += "\"reported_hashrate\":";   payload += RHR; 
//    payload += " , ";
//    payload += "\"calculated_hashrate\":";   payload += CHR; 
//    payload += " , ";
//    payload += "\"wallet_balance\":";   payload += walletBalance; 
//    payload += "}";

//
//

