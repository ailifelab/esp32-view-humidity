#include "sensorTask.h"

SensorTask::SensorTask() : TaskThread("sensor", 4048, 10, 0)
{
    lastMillis = millis();
}

SensorTask::~SensorTask()
{
}

void SensorTask::run()
{
    while (1)
    {
        unsigned long currentMillis = millis();
        int pinSate = analogRead(BTN_WAKE_PIN);
        if (pinSate)
        {
            //读取湿度传感器数据并塞到队列
            int sensorValue = analogRead(SENSOR_A);
            publish(sensorValue);
            lastMillis = currentMillis;
        }
        else
        {
            delay(200);
            if (currentMillis - lastMillis >= SLEEP_INTERVAL_MILLIS)
            {
                lastMillis = currentMillis;
                esp_deep_sleep_start();
            }
        }
    }
}

void SensorTask::addQueue(QueueHandle_t sensorValueQueue)
{
    queues.push_back(sensorValueQueue);
}

void SensorTask::publish(const int &sensorValue)
{
    for (auto sensorValueQueue : queues)
    {
        xQueueOverwrite(sensorValueQueue, &sensorValue);
    }
}