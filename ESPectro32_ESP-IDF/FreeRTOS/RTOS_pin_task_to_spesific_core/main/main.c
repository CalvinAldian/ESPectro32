#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "String.h"

static int core = 0;

void taskOne(void * pvParameters){
    /*const char* taskMessage = "Task running on core ";
    taskMessage = taskMessage + xPortGetCoreID();
 
    while(1){
        printf(taskMessage);
        printf("\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }*/
    vTaskDelete(NULL);
}

void app_main()
{
    TaskHandle_t mytask;
    
    xTaskCreate(
        taskOne,
        "TaskOne",
        10000,
        NULL,
        12,
        &mytask
    );
    printf(xPortGetCoreID());
    /*printf("Task created...\n");
    printf("Starting main loop...\n");
    while (1){
        printf("main loop\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }*/
}
