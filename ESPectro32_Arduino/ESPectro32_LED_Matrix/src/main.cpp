#include <Arduino.h>
#include <ESPectro32_Board.h>
#include <ESPectro32_LedMatrix.h>
#define Threshold 40

Adafruit_IS31FL3731 matrix;

void setup() {
ESPectro32.begin();
  Serial.println("ISSI manual animation test");
  if (! matrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }
  Serial.println("IS31 Found!");    
}

void loop() {
        matrix.drawCircle(3,3,3,220);
}
