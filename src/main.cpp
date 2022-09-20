#include "einkTask.h"
#include <Arduino.h>
#include "driver/rtc_io.h"

static EinkTask einkTask = EinkTask();
static SensorTask sensorTask = SensorTask();
static QueueHandle_t sensorValueQueue;

void setup()
{
    Serial.begin(115200);
    einkTask.runTask();
    sensorTask.runTask();
    sensorValueQueue = einkTask.getSensorValueQueue();
    vTaskDelete(NULL);

    pinMode(BTN_WAKE_PIN, INPUT);
    rtc_gpio_pulldown_en(BTN_WAKE_PIN);
    rtc_gpio_pullup_dis(BTN_WAKE_PIN);
    esp_sleep_enable_ext0_wakeup(BTN_WAKE_PIN, 1);
    esp_deep_sleep_start();
}

void loop() {}