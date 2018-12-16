void screen() {   // Display on OLED


  float coinCenterX (28);
  float coinCenterY (28);
  float coinRadius = (volts * 1.2) - 10;
  float needle_length (35);
  float needleX (80);
  float needleY (32);

  float angle (0);
  int x2 (0);
  int y2 (0);
  int x3 (0);
  int y3 (0);
  

  
//  hash_bar = (hash_bar * 9) + total_hashrate;
//  hash_bar = hash_bar / 10;

  volts  =  (4 * volts) + T_volts; //smoothing
  volts = volts / 5;
  
  
  amps  =  (4 * amps) + T_amps; //smoothing
  amps = amps / 5;


//volts = testVolts;  // ------------------ testing
//testVolts  = testVolts -.2; // ------------------ testing

//volts = 0;

  float pievolts = (volts - (.01*amps)) - 23;
  if (pievolts < 0) pievolts = 0;

  pievolts = pievolts * 60;

  int pie = int(pievolts);

  int needle = int (amps * .7);

  if (needle > 90) needle = 90;
  if (needle < -90) needle = -90;
  if (needle < 0) {
    needle = (needle * -1);
    needle = needle + 90;
  } else {
    needle = 90 - needle;
  }


  String dispVolts = String (volts,1) + "V";
  String dispAmps = String (amps,1) + "A";
  
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
 

  display.clear();
// animations -------------------------------------------------------------------------------
  
    
  for( float z=0; z < pie;z= z + .6 ){  
    angle = z ;
    angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
    x2 = ( coinCenterX + ( sin(angle) * coinRadius ) );
    y2 = ( coinCenterY - ( cos(angle) * coinRadius ) );
    x3 = ( coinCenterX + ( sin(angle) * ( coinRadius - ( coinRadius  ) ) ) );
    y3 = ( coinCenterY - ( cos(angle) * ( coinRadius - ( coinRadius  ) ) ) );
    display.drawLine( x2 , y2  , x3 , y3 );
   
  }

  
  angle = needle ;
  angle = ( angle / 57.29577951 ) ; //Convert degrees to radians
  x2 = ( needleX + ( sin(angle) * needle_length ) );
  y2 = ( needleY - ( cos(angle) * needle_length ) );
  x3 = ( needleX + ( sin(angle) * ( needle_length - ( needle_length  ) ) ) );
  y3 = ( needleY - ( cos(angle) * ( needle_length - ( needle_length  ) ) ) );
  display.drawLine( x2 , y2  , x3 , y3 );

  display.drawCircle(needleX, needleY, 6);
  display.drawCircle(needleX, needleY, 3);
  display.drawCircle(x3 , y3, 2);

  
  display.drawString(127, 0, dispVolts);
  display.drawString(116, 49, dispAmps);
  
  if (message == ""){
    if (volts < 5) {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(5, 53, "Data?");
  } else if (volts < 22) {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(5, 53, "Critical");  
  } else if (volts >27.5) {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(5, 53, "      ;)");
  }
  }else{
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    if (message.length() < 13){
       display.drawString(5, 53, message);
    }else{
      int msgLen = message.length();
      msgScroll ++;
      if (msgScroll > msgLen) msgScroll = 1;

      int dispEnd = msgScroll + 10;
      if (dispEnd > msgLen) dispEnd = msgLen;

      String msgPart = message.substring(msgScroll,dispEnd);
      
      display.drawString(5, 53, msgPart);
    }

   
  }

  if (relay > 20) display.drawCircle(2, 61, 1);
  
  if (failcount < 2) {
    display.drawCircle(4, 4, 1);
    display.drawCircle(4, 4, pulse*2);
    
  }
//  display.setTextAlignment(TEXT_ALIGN_LEFT);
//  String hash = String(total_hashrate,1);
//  hash += "~";
//  display.drawString(48,0, hash);
//  display.drawLine( 0 , 63  , int(hash_bar) , 63 );
//  display.drawLine( 0 , 62  , int(hash_bar) , 62 );
//  display.drawLine( 0 , 61  , int(hash_bar) , 61 );
//  display.drawLine( 0 , 60  , int(hash_bar) , 60 );
  

  

  if (volts < 23.1){
    display.drawLine( 30 , 35  , 35 , 0 );
    display.drawLine( 25 , 0  , 35 , 0 );
    display.drawLine( 30 , 35  , 25 , 0 );
  }

  if (volts < 23.6) {
    display.drawCircle(30, 44, 6);
    if (volts > 23) {
      display.drawCircle(30, 44, 5);
      display.drawCircle(30, 44, 4);
      display.drawCircle(30, 44, 3);
      display.drawCircle(30, 44, 2);
      display.drawCircle(30, 44, 1);
    }
  }

  
 
//Signal Strength
    int8_t quality = getWifiQuality();
    for (int8_t i = 0; i < 4; i++) {
      for (int8_t j = 0; j < 2 * (i + 1); j++) {
        if (quality > i * 25 || j == 0) {
          display.setPixel(118 + 2 * i, 63 - j);
        }
      }
    }


    display.drawCircle(coinCenterX-1,coinCenterY-1,heartbeat);
  
  display.display();
  
}

//float earning_24_hours = 0;
////String last_share_date = "";
//float total_hashrate = 0;
//float total_hashrate_calculated = 0;
//float wallet_balance = 0;

