#include "driver/rtc_io.h"
#include <Arduino.h>

#include "einkTask.h"
#include "sensorTask.h"

static EinkTask einkTask = EinkTask();
static SensorTask sensorTask = SensorTask();
static QueueHandle_t sensorValueQueue;
static QueueHandle_t serialDebugQueue;

void setup() {
  Serial.begin(115200);
  einkTask.runTask();
  sensorValueQueue = einkTask.getSensorValueQueue();
  sensorTask.addQueue(sensorValueQueue);
  serialDebugQueue = xQueueCreate(1, 16);
  sensorTask.addQueue(serialDebugQueue);
  sensorTask.runTask();
  vTaskDelete(NULL);
  rtc_gpio_pulldown_en(BTN_WAKE_PIN);
  rtc_gpio_pullup_dis(BTN_WAKE_PIN);
  
  pinMode(BTN_WAKE_PIN, INPUT);
}

int sensorValue = 0;
void loop() {
  if (xQueueReceive(serialDebugQueue, &sensorValue, portMAX_DELAY) ==
      pdFALSE /**未更新数据*/) {
    Serial.print("current value:");
    Serial.println(sensorValue);
  }
  Serial.println("loop");
  delay(1000);
}