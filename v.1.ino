/*

        Project : Irrigation
         Author : Juan Bredenkamp
        Created : 28 May 2019
    Description : Hassio MQTT Irrigation Automation
        Version : v.1
MQTT Device ID  : irrigation

  Arduino Pin                                  Board Pin

	Ground  ----------------------------------   Ground

		  D1  ----------------------------------   zone1
		  D2  ----------------------------------   zone2
		  D5  ----------------------------------   zone3
      D6  ----------------------------------   zone4
  
  Pub
  
	     iotstatus		  ---- 	  Irrigation IOT Online!

  Sub
  
      zone1		  ---- 	  zone1 On or Off "zone1triggeron" or "zone1triggeroff"
      zone2		  ---- 	  zone2 On or Off "zone2triggeron" or "zone2triggeroff"
      zone3		  ---- 	  zone3 On or Off "zone3triggeron" or "zone3triggeroff"
      zone4		  ---- 	  zone4 On or Off "zone4triggeron" or "zone4triggeroff"

*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

char message_buff[100];

IPAddress ip(192, 168, 20, 62);
IPAddress gateway(192, 168, 20, 1);
IPAddress subnet(255, 255, 255, 0); 
IPAddress server(192, 168, 20, 60);

char ssid[] = "SSID"; // your SSID
char pass[] = "Password";  // your SSID Password

WiFiClient espClient;
PubSubClient mqttClient(espClient);

int zone1 = D1;
int zone2 = D2;
int zone3 = D5;
int zone4 = D6;

const long interval = 1000;
unsigned long previousMillis = 0;
int ledState = LOW; 

int builtinled = 2;

void setup() {
  
  Serial.begin(115200);
  pinMode(builtinled, OUTPUT);
  
  pinMode(zone1, OUTPUT);
  digitalWrite(zone1, HIGH);
  
  pinMode(zone2, OUTPUT);
  digitalWrite(zone2, HIGH);
  
  pinMode(zone3, OUTPUT);
  digitalWrite(zone3, HIGH);
  
  pinMode(zone4, OUTPUT);
  digitalWrite(zone4, HIGH);

  
  WiFi.config(ip, gateway, subnet); 
  WiFi.begin(ssid, pass);
 
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
}

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );
    
  delay(1500);
   
  mqttClient.setServer(server, 1883);
  mqttClient.setCallback(callback);
 
  delay(1500);
 
}

void loop() {
  
  if (!mqttClient.connected()) {
    reconnect();
  }
  


unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(builtinled, ledState);
  }



  mqttClient.loop();

  
}

void reconnect() {

  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("irrigation", "username", "Password")) {
      Serial.println("connected");
      
      mqttClient.publish("iotstatus","Irrigation IOT Online!");
	  
      mqttClient.subscribe("zone1");
      mqttClient.subscribe("zone2");
      mqttClient.subscribe("zone3");
	    mqttClient.subscribe("zone4");
	  
    }
    else {
      Serial.print("failed, rc=");
      Serial.println(" try again in 10 seconds");
      delay(10000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  int i = 0;

  for (i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }

  message_buff[i] = '\0';
  String msgString = String(message_buff);
  Serial.print(msgString);
  Serial.println();
  
  if (msgString == "zone1triggeroff"){
	  digitalWrite(zone1,HIGH);
    Serial.println("zone1 off");
  }
	
  if (msgString == "zone1triggeron"){
	  digitalWrite(zone1,LOW);
    Serial.println("zone1 on");
  }
  
  if (msgString == "zone2triggeroff"){
	  digitalWrite(zone2,HIGH);
    Serial.println("zone2 off");
  }
  
  if (msgString == "zone2triggeron"){
	  digitalWrite(zone2,LOW);
    Serial.println("zone2 on");
  }
  
  if (msgString == "zone3triggeroff"){
	  digitalWrite(zone3,HIGH);
    Serial.println("zone3 off");
  }
	
  if (msgString == "zone3triggeron"){
	  digitalWrite(zone3,LOW);
    Serial.println("zone3 on");
  }
  
  if (msgString == "zone4triggeroff"){
	  digitalWrite(zone4,HIGH);
    Serial.println("zone4 off");
  }
	
  if (msgString == "zone4triggeron"){
	  digitalWrite(zone4,LOW);
    Serial.println("zone4 on");
  }
 
}
