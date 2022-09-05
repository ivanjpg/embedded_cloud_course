#include <driver/gpio.h>
#include <driver/adc.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// PIN 34 -> ADC1, channel 6
// https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
#define PIN_LM ADC1_CHANNEL_6

// The standard reference value for ESP32 ADC is 1.1V
// but can be obtained calibrated if desired
// https://docs.espressif.com/projects/esp-idf/en/v4.4.2/esp32/api-reference/peripherals/adc.html

// Must be adjusted depending on the attenuation.
// For 0db  -> 1100.0
// For 6db  -> 2200.0
// For 11db -> 3300.00
// https://esp32.com/viewtopic.php?f=12&t=1045#p4636
#define ADC_REF 1100.0

// The maximum resolution is 12-bit
#define ADC_RES 4096.0

void app_main() {
  adc1_config_width(ADC_WIDTH_12Bit);
  adc1_config_channel_atten(PIN_LM, ADC_ATTEN_0db);

  while(1) {
    int adc = adc1_get_raw(PIN_LM);
    float mV = adc * (ADC_REF / ADC_RES);
    float temp = mV / 10.0;

    printf("T=%f°C\n", temp);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}