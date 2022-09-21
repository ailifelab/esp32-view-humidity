#include <Arduino.h>
#include "driver/rtc_io.h"

#include "einkTask.h"
#include "sensorTask.h"

static EinkTask einkTask = EinkTask();
static SensorTask sensorTask = SensorTask();
static QueueHandle_t sensorValueQueue;
static QueueHandle_t serialDebugQueue;

void setup()
{
    Serial.begin(115200);
    einkTask.runTask();
    sensorValueQueue = einkTask.getSensorValueQueue();
    sensorTask.addQueue(sensorValueQueue);
    serialDebugQueue = xQueueCreate(1, 16);
    sensorTask.addQueue(serialDebugQueue);
    sensorTask.runTask();
    vTaskDelete(NULL);

    pinMode(BTN_WAKE_PIN, INPUT);
    rtc_gpio_pulldown_en(BTN_WAKE_PIN);
    rtc_gpio_pullup_dis(BTN_WAKE_PIN);
    esp_sleep_enable_ext0_wakeup(BTN_WAKE_PIN, 1);
    esp_deep_sleep_start();
}

int sensorValue = 0;
unsigned long last_debug = 0;
void loop()
{
    if (millis() - last_debug > 200 && xQueueReceive(serialDebugQueue, &sensorValue, portMAX_DELAY) == pdTRUE)
    {
        Serial.println("current value:" + sensorValue);
        last_debug = millis();
    }
}