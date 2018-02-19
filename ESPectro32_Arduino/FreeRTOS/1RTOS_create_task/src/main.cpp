#include <Arduino.h>
#include <ESPectro32_board.h>

void taskOne( void * parameter )
{
    for( int i = 0;i<10;i++ ){
        Serial.println("Task 1");
        delay(1000);
    } 
    Serial.println("Ending task 1");
    vTaskDelete( NULL );
}
 
void taskTwo( void * parameter)
{
    for( int i = 0;i<10;i++ ){
        Serial.println("Task 2");
        delay(1000);
    }
    Serial.println("Ending task 2");
    vTaskDelete( NULL ); 
}

void setup() {
  Serial.begin(112500);
 
  xTaskCreate(
    taskOne,          /* Task function. */
    "TaskOne",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
 
  xTaskCreate(
    taskTwo,          /* Task function. */
    "TaskTwo",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
 
}
 
void loop() {
  delay(1000);
}