#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

static int core = 0;

void taskOne(void *pvParameters){
    const char * taskmessage = "task running on core ";
    int _core = xPortGetCoreID();
     while (true){
         printf(taskmessage);
         printf("%d\n",_core);
         vTaskDelay(1000 / portTICK_PERIOD_MS);
     }
}

void app_main()
{
    printf("creating task on core ");
    printf("%d",core);
    xTaskCreatePinnedToCore(
        taskOne,
        "taskOne",
        10000,
        NULL,
        12,
        NULL,
        core
    );
    printf("task created...");

    printf("starting main loop...");
    while(true){
        printf("main loop\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
