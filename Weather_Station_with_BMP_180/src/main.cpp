#include <Arduino.h>
#include <Preferences.h>
#include <ESPectro32_Board.h>
#include <ESPectro32_RGBLED_Animation.h>
#include <Wifi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <string.h>
#include <Wire.h> //Including wire library
#include <SFE_BMP180.h> //Including BMP180 library

#define ALTITUDE 768 //Altitude where I live (change this to your altitude)

SFE_BMP180 pressure; //Creating an object for BMP180
WiFiClient espClient; 
Preferences preferences; 
PubSubClient client(espClient);

const char* userName = "user.name";
const char* userPass = "user.password";
const char* broker   = "mqtt.dioty.co";
const char* outTopic = "/topic/out";
const char* outTopicT = "/topic/outT";
const char* outTopicA = "/topic/outA";

int threshold = 40; // threshold for capasitive touch
char messages[50];  // buff to save phototransistor value
char messagesT[50]; // buff to save temperature value
char messagesA[50]; // buff to save humidity value
int trying, touch, i;
float temp, alt;

//reconecting to broker
void reconnect(){
  while(!client.connected()){
    Serial.print("\nconnected to ");
    Serial.println(broker);
    if(client.connect("client_id",userName, userPass)){
      Serial.print("\nconnected to ");
      Serial.println(broker);
    }
    else {
      Serial.println("Trying to connect again ");
      delay(5000);
    }
  }
}

//setting up wif
void wifiSetup(){
  //trying to connect to wifi using SSID and Password that have been saved
  WiFi.begin(preferences.getString("SSID","not found").c_str(), preferences.getString("PASS","not found").c_str()); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    trying++;
    if (trying >=10) break;
  }
  
  //Init WiFi as Station, start SmartConfig
  if (WiFi.status() != WL_CONNECTED){
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();

    //Wait for SmartConfig packet from mobile
    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.println("SmartConfig received.");

    Serial.println("Connecting to wifi.");
    while(WiFi.status() != WL_CONNECTED){
      delay(100);
      Serial.print("-");
    }
    preferences.putString("SSID", WiFi.SSID()); // save SSID to preferences
    preferences.putString("PASS", WiFi.psk());  // save Password to preferences
  }
}

void setup() {
    // put your setup code here, to run once:    
    Serial.begin(9600); //Starting serial communication

    Serial.println("Program started");
    if (pressure.begin()) //If initialization was successful, continue
      Serial.println("BMP180 init success");
    else //Else, stop code forever
    {
      Serial.println("BMP180 init fail");
      while (1);
    }
    preferences.begin("WIFI", false);
    ESPectro32.begin();    
    wifiSetup();
    client.setServer(broker,1883);
}

void loop() {
    // put your main code here, to run repeatedly:
  char status;
  double T, P, p0; //Creating variables for temp, pressure and relative pressure

  Serial.print("You provided altitude: ");
  Serial.print(ALTITUDE);
  Serial.println(" meters");
  // Getting temperature
  status = pressure.startTemperature();
  if (status != 0) {
    delay(status);
    
    status = pressure.getTemperature(T);
    if (status != 0) {
      Serial.print("Temp: ");
      Serial.print(T, 1);
      temp = T;
      Serial.println(" deg C");
      // Getting pressure
      status = pressure.startPressure(3);

      if (status != 0) {
        delay(status);

        status = pressure.getPressure(P, T);
        if (status != 0) {
          Serial.print("Pressure measurement: ");
          Serial.print(P);
          alt = P;
          Serial.println(" hPa");

          p0 = pressure.sealevel(P, ALTITUDE);
          Serial.print("Relative (sea-level) pressure: ");
          Serial.print(p0);
          Serial.println("hPa");
        }
      }
    }
    }  
    if (!client.connected()){
      reconnect();
    }
    client.loop();
        snprintf(messages, 75, "value = %ld voltage = %f", ESPectro32.readPhotoTransistorValue(),ESPectro32.readPhotoTransistorVoltage());
        snprintf(messagesT, 75, "temperature = %f C", temp);
        snprintf(messagesA, 75, "pressure = %f hPa", alt);
        Serial.print("Photo Transistor : ");
        Serial.println(messages);
        client.publish(outTopic, messages);   // publishing phototransistor data to MQTT
        client.publish(outTopicT, messagesT); // publishing temperature data to MQTT   
        client.publish(outTopicA, messagesA); // publishing humidity data to MQTT
        touch = touchRead(T4);                // read capasitive touch value
        // clean preferences and restart ESP32 if the condition meet
        while(touch<=threshold){
            delay(100);
            i++;
            if (i>10) {
                preferences.clear();
                ESP.restart();
            }
        }
        i = 0;
        delay(1000);
}




