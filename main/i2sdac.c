#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "i2sdac.h"

#define SAMPLE_RATE     (44100)
#define I2S_NUM         I2S_NUM_0
#define DMA_BUF_COUNT	14
#define DMA_BUF_LEN	(32*4)

#define TAG "i2sdac"

void i2sdac_init()
{
  i2s_config_t i2s_config;
  
  static const i2s_pin_config_t pin_config = {
    .bck_io_num = 26,
    .ws_io_num = 25,
    .data_out_num = 27,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  
  
  i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER|I2S_MODE_TX);
  i2s_config.sample_rate = SAMPLE_RATE ;
  i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
  i2s_config.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
  i2s_config.communication_format = I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB;
  i2s_config.dma_buf_count = DMA_BUF_COUNT;
  i2s_config.dma_buf_len = DMA_BUF_LEN; 
  i2s_config.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
  
  i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM, &pin_config);
  
}

void i2sdac_start()
{
  i2s_zero_dma_buffer(I2S_NUM);
  i2s_start(I2S_NUM);
}

void i2sdac_stop()
{
  i2s_stop(I2S_NUM);
  i2s_zero_dma_buffer(I2S_NUM);
}

void i2sdac_write(const uint8_t *data, uint32_t len)
{
  TickType_t delay = 50 / portTICK_PERIOD_MS;

  if(len % 8){
    ESP_LOGE(TAG,"unexpected data len:%u",len);
    return;
  }

  for(int i=0; i<len; i+= 4){
    uint16_t d[2];
    
    d[0] = data[i+0]|(data[i+1] << 8);
    d[1] = data[i+2]|(data[i+3] << 8);
    
    int n = i2s_push_sample(I2S_NUM, (const char *)d, delay);
    if(n < 0)
      ESP_LOGE(TAG, "i2s_write_bytes error:%d",n);
  }
}
