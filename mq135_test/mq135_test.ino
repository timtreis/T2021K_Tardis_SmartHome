#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Debug mode
#define debug_mode true

// include WiFi credentials and hardware metadata
#include "wifi_credentials.h"


WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  
  delay(500);

  if (debug_mode == true) {
    
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
  }
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    
      delay(500);

      if (debug_mode == true) { Serial.print("."); }
      
  }
  
  if (debug_mode == true) {
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
  }

}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Reconnecting...");
        if (!client.connect("ESP8266Client")) {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
}
 
void setup() {
 
  if (debug_mode == true) { Serial.begin(115200); }
 
  setup_wifi();
 
  client.setServer(mqttServer, mqttPort);
 
}
 
void loop() {

  if (!client.connected()) {
      reconnect();
  }
  client.loop();

  float sensorVoltage; 
  float sensorValue;

  sensorValue = analogRead(A0);
  sensorVoltage = sensorValue/1024;
    
  if (debug_mode == true) {

    Serial.print("sensor voltage = ");
    Serial.print(sensorVoltage);
    Serial.print("\n");
    
  }
 
  client.publish("balin/airquality", String(sensorVoltage).c_str());
  
  delay(120000);
}
