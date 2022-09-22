#include "einkTask.h"

#include <SPI.h>
#include <epd1in54.h>
#include <epdpaint.h>

EinkTask::EinkTask() : TaskThread("disp", 4048, 1, 1) {
  sensorValueQueue = xQueueCreate(1, 16);
  assert(sensorValueQueue != NULL);
}
EinkTask::~EinkTask() { vQueueDelete(sensorValueQueue); }

Epd epd;
void EinkTask::run() {
  if (epd.Init(lut_full_update) != 0) {
    assert("e-Paper init failed");
  }
  while (1) {
    if (xQueueReceive(sensorValueQueue, &sensorValue, portMAX_DELAY) ==
            pdFALSE /**未更新数据*/
        || busy /**屏幕未完成刷新*/) {
      continue;
    } else {
      Serial.print("Sensor in eink:");
      Serial.println(sensorValue);
    }
  }
}
QueueHandle_t EinkTask::getSensorValueQueue() { return sensorValueQueue; }

bool EinkTask::isBusy() { return busy; }
void EinkTask::setBusy(bool busyTag) { this->busy = busyTag; }