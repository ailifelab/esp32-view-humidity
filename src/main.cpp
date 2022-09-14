#include "einkTask.h"
#include <Arduino.h>

static EinkTask einkTask = EinkTask();
static QueueHandle_t sensorValueQueue;

void setup() {
  Serial.begin(115200);
  einkTask.runTask();
  sensorValueQueue = xQueueCreate(1, 16);
  vTaskDelete(NULL);
}

void loop() {}