#pragma once
#include <Arduino.h>
#include <vector>
#include "taskThread.h"

class SensorTask : public TaskThread<SensorTask>
{
    friend class TaskThread<SensorTask>;

public:
    SensorTask();
    ~SensorTask();
    void run();
    void addQueue(QueueHandle_t sensorValueQueue);

private:
    unsigned long lastMillis;
    std::vector<QueueHandle_t> queues;
    void publish(const int &sensorValue);
};