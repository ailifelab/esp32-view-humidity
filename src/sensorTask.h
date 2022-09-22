#pragma once
#include "taskThread.h"
#include <Arduino.h>
#include <vector>

class SensorTask : public TaskThread<SensorTask> {
  friend class TaskThread<SensorTask>;

public:
  SensorTask();
  ~SensorTask();
  void run();
  void addQueue(QueueHandle_t sensorValueQueue);

private:
  unsigned long lastMillis;
  bool pressed;
  unsigned long btnStart;
  bool pressOver;
  unsigned long pressTime;
  std::vector<QueueHandle_t> queues;
  void publish(const int &sensorValue);
  void btnCheck();
};