#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

void taskOne(void * pvParameters){
    for (int i = 0; i<=5; i++){
        printf("%d\n",i);    
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
        NULL,
        1,
        NULL
    );
}
