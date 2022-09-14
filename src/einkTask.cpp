#include "einkTask.h"

#include <GxEPD2_BW.h>
#define GxEPD2_DRIVER_CLASS GxEPD2_154_M09

EinkTask::EinkTask() : TaskThread("disp", 4048, 1, 1)
{
    sensorValueQueue = xQueueCreate(1, 16);
    assert(sensorValueQueue != NULL);
}
EinkTask::~EinkTask()
{
    vQueueDelete(sensorValueQueue);
}

void EinkTask::run()
{
    while (1)
    {
        if (xQueueReceive(sensorValueQueue, &sensorValue, portMAX_DELAY) ==
                pdFALSE /**未更新数据*/
            || busy /**屏幕未完成刷新*/)
        {
            continue;
        }
    }
}
QueueHandle_t EinkTask::getSensorValueQueue()
{
    return sensorValueQueue;
}

bool EinkTask::isBusy()
{
    return busy;
}
void EinkTask::setBusy(bool busyTag)
{
    this->busy = busyTag;
}