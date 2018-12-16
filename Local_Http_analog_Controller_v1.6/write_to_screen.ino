void screen() {

  String out = "";

//// Display on OLED
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);



  String disp = "";

  String a$ = "Current : " + String (a);
  display.drawString(1, 3, a$);
  display.setFont(ArialMT_Plain_24);
  String v$ = "V:" + String (v);
  
  if (connectivity == false){
    
    display.setFont(ArialMT_Plain_16);
    display.drawString(2, 35, v$);
    display.setFont(ArialMT_Plain_10);  
    display.drawString(5, 50, "Connectivity Error");
    
  } else{
    
    display.drawString(33, 35, v$);
    display.setFont(ArialMT_Plain_10);
    display.drawString(4, 38, "Vt");
    float vt = relay_Vcutout;
    if (cycleOn == false) vt = 0;
    String Vtarget = String(vt);
    display.drawString(4, 49, Vtarget);
    
  }
  

  if (check_time1 > 5) {
    disp = String(check_time1);
    
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(126, 34, disp);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    
  }

  if (check_time2 > 2) {
    disp = String(check_time2);
    
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(126, 44, disp);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    
  }



//////LINES  
  display.drawLine(40,32,127,32);
  display.drawLine(0,32,7,32);
  display.drawLine(0,0,127,0);
  display.drawLine(127,0,127,63);  
  display.drawLine(127,63,0,63);
  display.drawLine(0,63,0,0);

////////TIME
   display.drawCircle(24 , 32, heartbeat);
   display.drawCircle(24 , 32, (pulse * 7));


/////Signal Strength
  int8_t quality = getWifiQuality();
  for (int8_t i = 0; i < 4; i++) {
    for (int8_t j = 0; j < 2 * (i + 1); j++) {
      if (quality > i * 25 || j == 0) {
        display.setPixel(118 + 2 * i, 60 - j);
      }
    }
  }


/////IP Address
  display.setFont(ArialMT_Plain_10);
  String (ip) = getIpString (WiFi.localIP());
  String IP=ip;
  display.drawString(43, 20, IP);       


  
  display.display();

  
}

