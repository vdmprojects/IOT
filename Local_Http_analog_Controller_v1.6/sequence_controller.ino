void tock(){
  TT = 0;
  tick++;
  
  if (tick >= (10 * update_interval)){
    
    tick = 0;
    
    getData();

    checkRelays();

    check_time1++;

    if (check_time1 == 30) ESP.restart();

    check_time2++;

    if (check_time2 == 15) {
      server.stop(); 
      server.begin();
    }


    if (check_time2 == 30) ESP.restart();


    Serial.print("Time since API access :");
    Serial.println(check_time1);
    
    build_json();

    screen();

  }
}

