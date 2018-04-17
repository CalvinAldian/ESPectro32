#include <Arduino.h>
#include "BLEDevice.h"
#include <Preferences.h>
#include <ssd1306.h>
#include <OLEDDisplayUi.h>
#include "string.h"
#include <stdio.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "time.h"
#include <esp_system.h>

#define BEAM    33
#define PB      27
#define CW      5
#define CCW     18
#define SW1     14
#define SW2     25
#define SW3     21
#define OP      12
#define ED      13
#define LOCK    23
#define RESET   22

//#define DEMO_DURATION 3000
//typedef void (*Demo)(void);

//int demoMode = 0;
//int counter = 1;

int jarak;
bool flag = false;
bool flag2 = false;
bool _manual = false;
bool deviceFound1 = false;
bool deviceFound2 = false;
bool timeUpdate = false;
bool night = false;
bool morning = false;
bool wifi = false;
bool mqtt = false;
bool _reset = false;
int phase;
int trying;
static BLEAddress *pServerAddress;
const char* userName = "calvin.aldian@gmail.com";
const char* userPass = "9cb80760";
const char* broker   = "mqtt.dioty.co";
const char* outTopic = "/calvin.aldian@gmail.com/out";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 7;
const int   daylightOffset_sec = 0;
char messages[50];
struct tm timeinfo;
int year, month, day, hour, minute, second;

String knownCar[] = { "c4:e0:db:5e:f9:b2"};
String knownBike[] = { "  :  :  :  :  :  "};

BLEScan* pBLEScan;  
BLEClient*  pClient;
Preferences preferences;
//SSD1306  display(0x3c, 4,15);
//OLEDDisplayUi ui (&display);
WiFiClient espClient; 
PubSubClient client(espClient);
time_t now;

/*void drawFrame1() {
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  display.drawStringMaxWidth(0, 0, 50, frame1_Line1);
  display.display();
}

void drawFrame2(){
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 , 0 , frame2_Line1);
  display.drawString(0 , 10 , frame2_Line2);
  display.drawString(0 , 20 , frame2_Line3);
  display.drawString(0 , 30 , frame2_Line4);
  display.drawString(0 , 40 , frame2_Line5);
}

Demo demos[] = {drawFrame1,drawFrame2};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

void oled(int frame){
    display.clear();
    // draw the current demo method
    demos[frame]();
    display.display();
    delay(10);
}*/

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    
    //Called for each advertising BLE server.
    
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE Advertised Device found: ");
      Serial.println(advertisedDevice.toString().c_str());
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());

      bool known = false;
      bool known2 = false;
      for (int i = 0; i < (sizeof(knownCar) / sizeof(knownCar[0])); i++) {
        if (strcmp(pServerAddress->toString().c_str(), knownCar[i].c_str()) == 0) known = true;
      }
      for (int j = 0; j < (sizeof(knownBike) / sizeof(knownBike[0])); j++) {
        if (strcmp(pServerAddress->toString().c_str(), knownBike[j].c_str()) == 0) known2 = true;
      }
      if (known || known2) {
          if (advertisedDevice.getRSSI() > -100) {
            if (known) deviceFound1 = true;
            else if (known2) deviceFound2 = true;
            Serial.print("Device found: ");
            Serial.println(advertisedDevice.getRSSI());
            //display.drawString(0, 30, advertisedDevice.getRSSI());
            //display.display();
            Serial.println(pServerAddress->toString().c_str());
            advertisedDevice.getScan()->stop();
          }
      } 
      else if (flag == false) advertisedDevice.getScan()->stop();
      else {
          Serial.println("no device found");
          deviceFound1 = false;
          deviceFound2 = false;
      }
    }
}; // MyAdvertisedDeviceCallbacks

void reboot(){
    if(!digitalRead(RESET)&&!flag&&!flag2){
        _reset = true;
    }
}

void manual(){
    if (!digitalRead(PB)){
        if (_manual == false) _manual = true;
        else _manual = false;
    }
    feedback();
}

void manuallyOpen(){
    if (_manual && !digitalRead(OP) && digitalRead(ED)){
        if (digitalRead(SW1)){
            digitalWrite(CW,LOW);
            digitalWrite(CCW,HIGH);
        }
        else if (!digitalRead(SW1)){
            digitalWrite(CW,HIGH);
            digitalWrite(CCW,HIGH);    
        }
    }
    else if (_manual && digitalRead(OP) && digitalRead(ED)){
        digitalWrite(CW,HIGH);
        digitalWrite(CCW,HIGH);
    }
}

void manuallyClose(){
    if (_manual && !digitalRead(ED) && digitalRead(OP)){
        if (digitalRead(SW2)){
            digitalWrite(CCW,LOW);
            digitalWrite(CW,HIGH);
        }
        else if (!digitalRead(SW2)){
            digitalWrite(CCW,HIGH);
            digitalWrite(CW,HIGH);    
        }
    }
    else if (_manual && digitalRead(ED) && digitalRead(OP)){
        digitalWrite(CCW,HIGH);
        digitalWrite(CW,HIGH);
    }
}

void gateLock(){
    if (digitalRead(SW3)&&night) digitalWrite(LOCK,LOW);
}

void gateOpen(){
    mqtt = false;
    phase = 1;
}

void stopClose(){
    if (phase == 4 || phase == 2){
        if (digitalRead(SW2)) phase = 5;
    }
}

void stopOpen(){
    WiFi.mode(WIFI_OFF);
    wifi = false;
    digitalWrite(CCW,HIGH);
    digitalWrite(CW,HIGH);
    if (digitalRead(SW1) && phase == 1 && mqtt) phase = 2;
}

void beam(){
    if (phase == 2 || phase == 3 || phase == 4){
        if (!digitalRead(BEAM)) phase = 3;
        else phase = 4;
    }
}

void waktu(){
    now = time(nullptr);
    Serial.println(ctime(&now));
    //frame1_Line1 = " ";
    //frame1_Line1 += ctime(&now);
    hour = (now/3600)%24;
    hour = hour + 7;
    if (hour>24) hour = hour - 24;
    minute = (now/60)%60;
    second = now % 60;
    if (hour>=9&&minute>=0&&second>=0){
        if (hour<=18&&minute<=59&&second<=59){
            night = false;
            morning = true;
            digitalWrite(LOCK,HIGH);
        }
    }
    if ((hour>=19&&minute>=0&&second>=0)||(hour<=8&&minute<=59&&second<=59)) {
        night = true;
        morning = false;
        digitalWrite(LOCK,LOW);
    }
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.println(second);
    Serial.print("nigth : ");
    Serial.println(night);
    Serial.print("morning : ");
    Serial.println(morning);
    Serial.print("manual : ");
    Serial.println(_manual);
    //delay(1000);
}

void printLocalTime()
{
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void updateTime(){
    //update time
    //btStop();
    delay(10);
    Serial.println("update time....");
    WiFi.mode(WIFI_STA);
    Serial.println(preferences.getString("SSID","not found"));
    Serial.println(preferences.getString("PASS","not found"));
    WiFi.begin(preferences.getString("SSID","not found").c_str(), preferences.getString("PASS","not found").c_str()); 
    trying = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        trying++;
        Serial.println(trying);
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
        //init and get the time
    }
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();
    //disconnect WiFi as it's no longer needed
    //WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    //btStart();
}

void feedback(){
    delay(10);
    while((phase==1&&!mqtt)||(_manual&&!mqtt)){
        WiFi.mode(WIFI_STA);
        wifi = true;
        WiFi.begin(preferences.getString("SSID","not found").c_str(), preferences.getString("PASS","not found").c_str());     
        while(!client.connected()&&phase==1){
            Serial.print("\nconnected to ");
            Serial.println(broker);
            if(client.connect("client_id",userName, userPass)){
                Serial.print("\nconnected to ");
                Serial.println(broker);
            }
            else {
                Serial.println("Trying to connect again ");
                for (int k=0; k<=200; k++){
                    delay(10);
                    if (phase!=1) break;
                }
            }
            Serial.println(client.connected());
            Serial.println("loop");
        }
        //messages
        Serial.println("sending messages");
        if (!_manual) snprintf(messages, 50, "Gate Opened");
        else snprintf (messages, 50, "Gate operated manually");
        client.publish(outTopic, messages);
        Serial.println("Messages Published!!");
        client.disconnect();
        WiFi.mode(WIFI_OFF);
        wifi = false;
        btStart();
        mqtt = true;
    }
}

void setup() {
    preferences.begin("WIFI", false);   
    Serial.begin(115200);
    //pinMode(16,  OUTPUT);
    pinMode(LOCK, OUTPUT);
    pinMode(CW,  OUTPUT);
    pinMode(CCW, OUTPUT);
    //manual
    pinMode(PB,  INPUT_PULLUP);
    pinMode(OP, INPUT_PULLUP);
    pinMode(ED, INPUT_PULLUP);
    pinMode(RESET, INPUT_PULLUP);
    //sensor
    pinMode(BEAM, INPUT_PULLUP);
    pinMode(SW1,  INPUT_PULLUP);
    pinMode(SW2,  INPUT_PULLUP);
    pinMode(SW3,  INPUT_PULLUP);
    digitalWrite(CW,HIGH);
    digitalWrite(CCW,HIGH);
    attachInterrupt(digitalPinToInterrupt(SW1),stopOpen,CHANGE);
    attachInterrupt(digitalPinToInterrupt(SW2),stopClose,CHANGE);
    attachInterrupt(digitalPinToInterrupt(SW3),gateLock,CHANGE);
    attachInterrupt(digitalPinToInterrupt(BEAM),beam,CHANGE);
    attachInterrupt(digitalPinToInterrupt(OP),manuallyOpen,CHANGE);
    attachInterrupt(digitalPinToInterrupt(ED),manuallyClose,CHANGE);
    attachInterrupt(digitalPinToInterrupt(PB),manual,CHANGE);
    attachInterrupt(digitalPinToInterrupt(RESET),reboot,CHANGE);

    updateTime();//get time
    
    //reset OLED
    //digitalWrite(16,LOW);
    //delay(50);
    //digitalWrite(16,HIGH);

    //setup OLED
    //ui.init();
    //display.init();
    //display.flipScreenVertically();
    //display.setFont(ArialMT_Plain_10);
    //display.setTextAlignment(TEXT_ALIGN_LEFT);

    //setup     BLE
    BLEDevice::init(""); 
    pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true);
    client.setServer(broker,1883);
}

void loop() {
    if (!flag&&!flag2) waktu();
    if (!_manual&&!wifi){
        deviceFound1 = false;
        deviceFound2 = false;
        Serial.print("start scanning...");
        BLEScanResults scanResults = pBLEScan->start(1);
        if (deviceFound2&&night&&!_manual){
            flag2 = true;
            digitalWrite(LOCK,HIGH);
            Serial.println("motor");
        }
        else if (deviceFound1&&night&&!_manual){
            flag = true;        
            digitalWrite(LOCK,HIGH);        
            gateOpen();
            Serial.println("mobil");
        }
        else if (deviceFound1&&morning&&!_manual){
            flag = true;              
            gateOpen();
            Serial.println("mobil");
        }
    }
    if (_reset) {
        preferences.clear();
        esp_restart();
    }
    while(flag&&!_manual){
        switch(phase){
            case 1 :
                digitalWrite(CCW,HIGH);
                digitalWrite(CW,LOW);
                Serial.println("opening gate");
                if (!mqtt) feedback();
            break;

            case 2 :
                digitalWrite(CW,HIGH);
                digitalWrite(CCW,LOW);
                Serial.println("closing gate");
            break;

            case 3 :
                digitalWrite(CCW,HIGH);
                digitalWrite(CW,HIGH);
                Serial.println("pause close");
            break;

            case 4 :
                digitalWrite(CCW,LOW);
                digitalWrite(CW,HIGH);
                Serial.println("continue close");
            break;

            case 5:
                digitalWrite(CW,HIGH);
                digitalWrite(CCW,HIGH);
                Serial.println("stand by");
                flag = false;
            break;
        }
        if (phase != 1){
            Serial.println("Scanning....");
            deviceFound1 = false;
            deviceFound2 = false;
            if (!wifi){
                BLEScanResults scanResults = pBLEScan->start(1);
                if (deviceFound2&&night&&!_manual){
                    flag2 = true;
                    digitalWrite(LOCK,HIGH);
                    Serial.println("motor");
                }
                else if (deviceFound1&&night&&!_manual){
                    flag = true;        
                    digitalWrite(LOCK,HIGH);        
                    gateOpen();
                    Serial.println("mobil");
                }
                else if (deviceFound1&&morning&&!_manual){
                    flag = true;              
                    gateOpen();
                    Serial.println("mobil");
                }
            }
        }
    }
}
