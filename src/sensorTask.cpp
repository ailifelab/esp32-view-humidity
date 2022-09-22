#include "sensorTask.h"

SensorTask::SensorTask() : TaskThread("sensor", 4048, 10, 0) {
  lastMillis = millis();
}

SensorTask::~SensorTask() {}

const int switch_interval = 500;
const int long_press_interval = 3000;

void SensorTask::btnCheck() {
  if (digitalRead(BTN_WAKE_PIN) == HIGH) {
    if (!pressed) {
      btnStart = millis();
      pressed = true;
    }
  } else {
    if (pressed) {
      pressOver = true;
      // unsigned long pressTime = millis() - btnStart;
      pressed = false;
    }
  }
}

void SensorTask::run() {
  pressed = false;
  btnStart = 0L;
  pressOver = false;
  pressTime = 0L;
  esp_sleep_enable_ext0_wakeup(BTN_WAKE_PIN, 0);
  while (1) {
    btnCheck();
    unsigned long currentMillis = millis();
    if (pressOver) {
      pressOver = false;
      int sensorValue = analogRead(SENSOR_A);
      // 4095=完全干燥，1300基本泡在水里
      publish(sensorValue);
      lastMillis = currentMillis;
    } else {
      if (currentMillis - lastMillis >= SLEEP_INTERVAL_MILLIS) {
        lastMillis = currentMillis;
        esp_light_sleep_start();
      }
    }
    delay(200);
  }
}

void SensorTask::addQueue(QueueHandle_t sensorValueQueue) {
  queues.push_back(sensorValueQueue);
}

void SensorTask::publish(const int &sensorValue) {
  for (auto sensorValueQueue : queues) {
    xQueueOverwrite(sensorValueQueue, &sensorValue);
  }
}