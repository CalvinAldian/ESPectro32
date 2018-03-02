//#ifndef MAIN_MB_TESTI2S_H_
//#define MAIN_MB_TESTI2S_H_

#include "Arduino.h"
#include <SPIFFS.h>
#include <FS.h>
#include "SD.h"

extern "C" {
#include "driver/i2s.h"
#include "esp_log.h"
}

void i2sInit(){

    i2s_config_t i2s_config_rx = {
        mode: (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        sample_rate: 44100,
        bits_per_sample: I2S_BITS_PER_SAMPLE_32BIT,
        channel_format: I2S_CHANNEL_FMT_RIGHT_LEFT,
        communication_format: (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        intr_alloc_flags: ESP_INTR_FLAG_LEVEL1,
        dma_buf_count: 32,
        dma_buf_len: 64
    };

    i2s_config_t i2s_config_tx = {
        mode: (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        sample_rate: 44100,
        bits_per_sample: I2S_BITS_PER_SAMPLE_16BIT,
        channel_format: I2S_CHANNEL_FMT_RIGHT_LEFT,
        communication_format: (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        intr_alloc_flags: ESP_INTR_FLAG_LEVEL1,
        dma_buf_count: 32,
        dma_buf_len: 64
    };

    i2s_pin_config_t pin_config_tx = {
		.bck_io_num = 26, //21,
		.ws_io_num = 25,
		.data_out_num = 22, //I2S_PIN_NO_CHANGE
		.data_in_num = I2S_PIN_NO_CHANGE
	};

	i2s_pin_config_t pin_config_rx = {
		.bck_io_num = 17, //GPIO_NUM_26, //GPIO_NUM_17
		.ws_io_num = 5, //GPIO_NUM_25, //GPIO_NUM_5
		.data_out_num = I2S_PIN_NO_CHANGE,
		.data_in_num = 39//GPIO_NUM_39,//GPIO_NUM_36
	};


 //Somehow it's needed. If not, noise!
  //  pinMode(16, INPUT);

 //for recording
    i2s_driver_install(I2S_NUM_1, &i2s_config_rx, 0, NULL);
    i2s_set_pin(I2S_NUM_1, &pin_config_rx);

 //for playback
    i2s_driver_install(I2S_NUM_0, &i2s_config_tx, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config_tx);
}
    

void echo(){
    uint32_t cnt = 0;
    uint32_t buffer;
    uint32_t buffer_out = 0;
    i2s_start(I2S_NUM_0);
    i2s_start(I2S_NUM_1);

    while(1){
        cnt++;
        buffer = 0;
        int bytes_popped = i2s_pop_sample(I2S_NUM_1, (char*)&buffer, portMAX_DELAY);
    
        buffer_out = buffer << 5;

        if (buffer_out == 0) {
            //For debugging, if out is zero
            Serial.printf("%d -> %x\n", cnt, (int)buffer_out);
            delay(50);
        }
        else {
            //Just playback for now
            i2s_push_sample(I2S_NUM_0, (char*)&buffer_out, portMAX_DELAY);
        }
    }
}