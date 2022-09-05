#include <driver/gpio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define PIN_LED GPIO_NUM_32

uint32_t led_state = 0;

void app_main() {
  gpio_reset_pin(PIN_LED);
  gpio_set_direction(PIN_LED, GPIO_MODE_OUTPUT);

  while(1) {
    gpio_set_level(PIN_LED, led_state);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    led_state = !led_state;
  }
}