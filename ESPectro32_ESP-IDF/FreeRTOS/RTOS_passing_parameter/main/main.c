#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

int var = 10;

void taskOne(void * pvParameters){
    for(int i =0 ; i<=5; i++){
    printf("%d\n",var);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void app_main()
{
    xTaskCreate(
        taskOne,
        "TaskOne",
        10000,
        (void*)&var,
        1,
        NULL
    );
}
