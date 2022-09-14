#include "taskThread.h"
#include <Arduino.h>

class EinkTask : public TaskThread<EinkTask>
{
    friend class TaskThread<EinkTask>;

public:
    EinkTask();
    ~EinkTask();
    bool isBusy();
    void setBusy(bool busyTag);
    QueueHandle_t getSensorValueQueue();

private:
    bool busy;
    QueueHandle_t sensorValueQueue;
    uint16_t sensorValue;
}