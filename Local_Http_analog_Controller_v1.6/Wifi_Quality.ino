// converts the dBm to a range between 0 and 100%
int8_t getWifiQuality() {
  int32_t dbm = WiFi.RSSI();
  Serial.print("db :");
  Serial.println(dbm);
  if(dbm <= -100) {
      return 0;
  } else if (dbm > 0 ){ 
      return 0;
  } else if(dbm >= -50) {
      return 100;
  } else {
      return 2 * (dbm + 100);
  }
}
