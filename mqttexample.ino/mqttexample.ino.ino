// LinkIt One sketch for MQTT Demo

#include <LWiFi.h>
#include <LWiFiClient.h>

#include <PubSubClient.h>

/* 
	Modify to your WIFI Access Credentials. 
*/
#define WIFI_AP "X9009"
#define WIFI_PASSWORD "123456789"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.

/*
	Modify to your MQTT broker - Select only one
*/
char mqttBroker[] = "iot.eclipse.org";
// byte mqttBroker[] = {192,168,1,220}; // modify to your local broker

LWiFiClient wifiClient;

PubSubClient client( wifiClient );

unsigned long lastSend;

void InitLWiFi()
{
  LWiFi.begin();
  // Keep retrying until connected to AP
  Serial.println("Connecting to AP");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD))) {
    delay(100);
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker ...");
    // Attempt to connect
    if ( client.connect("a48a1414-60be-463a-94ba-556605053cc8","bleclient1234","bleclient1234") ) {	// Better use some random name
      Serial.println( "[DONE]" );
      // Publish a message on topic "outTopic"
      client.publish( "outTopic","kartographers" );
	  // Subscribe to topic "inTopic"
    /*  client.subscribe( "inTopic" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );*/
    
  }
}

}
void setup()
{
   delay( 10000 );
   Serial.begin( 115200 );
   InitLWiFi();

   client.setServer( mqttBroker, 1883 );
   client.setCallback( callback );

   lastSend = 0;
}

void loop()
{
  
  if( !client.connected() ) {
    reconnect();
  }
  // Send an update only after 3 second

  if( millis()-lastSend > 3000 ) 
  {	
    sendAnalogData();
    lastSend = millis();
  }
  
  client.loop();
}

void callback( char* topic, byte* payload, unsigned int length ) {
  Serial.print( "Recived message on Topic:" );
  Serial.print( topic );
  Serial.print( "    Message:");
  for (int i=0;i<length;i++) {
    Serial.print( (char)payload[i] );
  }
  Serial.println();
}

void sendAnalogData() {
  // Read data to send
  int data_A0 = 123;
  int data_A1 = 456;
  int data_A2 = 789;
  
  // Just debug messages
  Serial.print( "Sending analog data : [" );
  Serial.print( data_A0 ); Serial.print( data_A1 ); Serial.print( data_A2 );
  Serial.print( "]   -> " );

  // Prepare a JSON payload string
  String payload = "{";
  payload += "\"A0\":\""; payload += int(data_A0); payload += "\", ";
  payload += "\"A1\":\""; payload += int(data_A1); payload += "\", ";
  payload += "\"A2\":\""; payload += int(data_A2); payload += "\"";
  payload += "}";
  
  // Send payload
  char analogData[100];
  payload.toCharArray( analogData, 100 );
  client.publish("outTopic",analogData);
  Serial.println( analogData );
}

