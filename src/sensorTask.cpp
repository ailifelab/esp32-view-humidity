#include "sensorTask.h"

SensorTask::SensorTask() : TaskThread("sensor", 4048, 10, 0) {
  lastMillis = millis();
}

SensorTask::~SensorTask() {}

const int switch_interval = 500;
const int long_press_interval = 3000;

volatile bool pressed = false;
volatile unsigned long btnStart = 0L;
volatile bool pressOver = false;
volatile unsigned long pressTime = 0L;

void SensorTask::btnCheck() {
  int pinState = digitalRead(BTN_WAKE_PIN);
  if (pinState == HIGH) {
    if (!pressed) {
      btnStart = millis();
      pressed = true;
    }
  } else {
    if (pressed) {
      pressOver = true;
      unsigned long pressTime = millis() - btnStart;
      pressed = false;
    }
  }
}

void SensorTask::run() {
  while (1) {
    btnCheck();
    unsigned long currentMillis = millis();
    if (pressOver) {
      pressOver = false;
      int sensorValue = analogRead(SENSOR_A);
      Serial.print("Sensor:");
      Serial.println(sensorValue);
      publish(sensorValue);
      lastMillis = currentMillis;
    } else {
      if (currentMillis - lastMillis >= SLEEP_INTERVAL_MILLIS) {
        lastMillis = currentMillis;
        esp_deep_sleep_start();
      }
      delay(200);
    }
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