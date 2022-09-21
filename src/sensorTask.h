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
  void btnChanged();

private:
  unsigned long lastMillis;
  std::vector<QueueHandle_t> queues;
  void publish(const int &sensorValue);

  void buttonPressed();
  void buttonReleased();
};