#include <driver/gpio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <dht11.h>

#define DHT_PIN GPIO_NUM_25

void app_main() {
  printf("Initializing DHT sensor... ");
  DHT11_init(DHT_PIN);
  printf("done!\n");

  while(1) {
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    struct dht11_reading dinfo = DHT11_read();

    if(dinfo.status != DHT11_OK) {
      printf("Cannot read the sensor's data!\n");

      return;
    }
    
    printf("T=%d°C\n\n", dinfo.temperature);
    printf("H=%d%%\n", dinfo.humidity);
    printf("======================\n");
  }
}