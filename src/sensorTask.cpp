#include "sensorTask.h"

SensorTask::SensorTask() : TaskThread("sensor", 4048, 10, 0) {
  lastMillis = millis();
}

SensorTask::~SensorTask() {}

const int switch_interval = 500;
const int long_press_interval = 3000;

volatile unsigned long last_switch = 0;
volatile unsigned long current = 0;
volatile bool button_acted = false;
volatile bool long_press = false;

void SensorTask::btnChanged() {
  if (digitalRead(BTN_WAKE_PIN) == HIGH) {
    buttonPressed();
  } else {
    buttonReleased();
  }
}

void SensorTask::buttonPressed() {
  button_acted = false;
  current = millis();
}

void SensorTask::buttonReleased() {
  if (!button_acted) {
    button_acted = true; // 标识当前一个按钮完成的按下抬起动作已经完备
    if ((millis() - current) > long_press_interval) {
      long_press = true; //记录按钮按下的总时长
    }
  }
}

void SensorTask::run() {
  while (1) {
    unsigned long currentMillis = millis();
    btnChanged();
    if (!button_acted || (current - last_switch) < switch_interval) {
      delay(200);
      if (currentMillis - lastMillis >= SLEEP_INTERVAL_MILLIS) {
        lastMillis = currentMillis;
        esp_deep_sleep_start();
      } // 在按钮事件不完备或者bounce的时候不做回应
    } else if (button_acted) {
      //读取湿度传感器数据并塞到队列
      int sensorValue = analogRead(SENSOR_A);
      Serial.print("Sensor:");
      Serial.println(sensorValue);
      publish(sensorValue);
      lastMillis = currentMillis;
      long_press = false;
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