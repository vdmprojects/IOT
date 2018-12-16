void build_json(){

String voltage = String(v);
String current = String(a);

byte relay_State = relayOn;
relay_State = relay_State + 20;
String relayState = String(relay_State);

float relayTarget = relay_Vcutout * cycleOn;
if (relayTarget < relay_Vcutin) relayTarget = relay_Vcutin;
String relayT = String(relayTarget);

  if (heartbeat < 2){
    heartbeat = 2;
  } else {
    heartbeat = 1;
  }

// Prepare a JSON payload string
  payload = "{";
  payload += "\"voltage\":"; payload += voltage; 
  payload += " , ";
  payload += "\"battery_current\":"; payload += current; 
  payload += " , ";
  payload += "\"relay_target\":"; payload += relayT;
  payload += " , ";
  payload += "\"relay_state\":"; payload += relayState;
  payload += " , ";
//  payload += "\"heartbeat\":"; payload += heartbeat;
//  payload += " , ";
  payload += "\"pulse\":"; payload += pulse;
  payload += "}";


 // Just debug messages
//  Serial.print( "New json formed " );
//  Serial.println( payload ); 

// // Send payload
//  char attributes[100];
//  payload.toCharArray( attributes, 100 );
//  client.publish( "v1/devices/me/telemetry", attributes );
//  Serial.println( attributes );
}
