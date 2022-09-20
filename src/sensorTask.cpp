#include "sensorTask.h"

SensorTask::SensorTask() : TaskThread("sensor", 4048, 10, 0)
{
}

SensorTask::~SensorTask()
{
}

void SensorTask::run()
{
    while (1)
    {
        if (xQueueReceive(btnQueue, &btnValue, portMAX_DELAY) ==
                pdFALSE /**未更新数据*/)
        {
            continue;
        }
    }
}