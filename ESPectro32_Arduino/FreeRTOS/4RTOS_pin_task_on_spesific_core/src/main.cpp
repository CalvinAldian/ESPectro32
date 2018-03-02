#include <ESPectro32_board.h>

static int taskCore = 0;  //if set to 1 the task will not working because the core isn't free because there is task working on it (main loop) and the task has the higher priority
 
void coreTask( void * pvParameters ){
 
    String taskMessage = "Task running on core ";
    taskMessage = taskMessage + xPortGetCoreID();
 
    while(true){
        Serial.println(taskMessage);
        delay(1000);
    }
}
 
void setup() {
 
  Serial.begin(112500);
  delay(1000);
 
  Serial.print("Starting to create task on core ");
  Serial.println(taskCore);
 
  xTaskCreatePinnedToCore(
    coreTask,   /* Function to implement the task */
    "coreTask", /* Name of the task */
    10000,      /* Stack size in words */
    NULL,       /* Task input parameter */
    0,          /* Priority of the task */
    NULL,       /* Task handle. */
    taskCore);  /* Core where the task should run */
 
  Serial.println("Task created...");
}
 
void loop() {
 
  Serial.println("Starting main loop...");
  while (true){}
 
}