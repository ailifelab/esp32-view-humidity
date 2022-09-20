#include <Arduino.h>

#include "taskThread.h"

class SensorTask : public TaskThread<SensorTask>
{
    friend class TaskThread<SensorTask>;

public:
    SensorTask();
    ~SensorTask();
    void run();
    void addQueue(QueueHandle_t queue);

private:
    uint8_t btnValue;
    QueueHandle_t btnQueue;

    std::vector<QueueHandle_t> queues;
};