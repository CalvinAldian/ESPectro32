#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

int var = 10;
TaskHandle_t myTask;

void taskOne(void * pvParameters){
    vTaskDelay(10000 / portTICK_PERIOD_MS);
}

void app_main()
{
    printf("Setup: priority = %d\n",uxTaskPriorityGet(NULL));
    xTaskCreate(
        taskOne,
        "TaskOne",
        10000,
        NULL,
        12,
        &myTask
    );
    printf("Setup: created Task priority = %d\n",uxTaskPriorityGet(myTask));
    printf("Main Loop: priority = %d\n",uxTaskPriorityGet(NULL));
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
