#include <WiFi.h>
#include "driver/i2s.h"

void i2s_config() {
  // http://esp-idf.readthedocs.io/en/latest/api/peripherals/i2s.html
  // input
  i2s_config_t i2s_in_config = {
    mode: (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    sample_rate: 44100,
    bits_per_sample: (i2s_bits_per_sample_t)32,
    channel_format: I2S_CHANNEL_FMT_RIGHT_LEFT,
    communication_format: (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    intr_alloc_flags: ESP_INTR_FLAG_LEVEL1,
    dma_buf_count: 32,
    dma_buf_len: 64
  };
  i2s_pin_config_t i2s_in_pin_config = {
    .bck_io_num= 17,
    .ws_io_num= 5,
    .data_out_num= -1, //Not used
    .data_in_num= 16
  };

   //pinMode(17, OUTPUT);
   pinMode(16, INPUT);
   //pinMode(5, OUTPUT);

  i2s_driver_install((i2s_port_t)0, &i2s_in_config, 0, NULL);
  i2s_set_pin((i2s_port_t)0, &i2s_in_pin_config);
}

void read_i2s() {
  i2s_start((i2s_port_t)0);
  uint32_t sample_val[2] = {0, 0};
  uint8_t bytes_read = i2s_pop_sample((i2s_port_t)0, (char *)sample_val, portMAX_DELAY);
  Serial.println(sample_val[0] << 5);
}

