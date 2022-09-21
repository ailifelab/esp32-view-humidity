#pragma once
#include <Arduino.h>

template<class T>
class TaskThread {
public:
    TaskThread(const char *_name, uint32_t stackDepth, UBaseType_t priority,
               const BaseType_t coreId = tskNO_AFFINITY)
        : _name{_name}, stackDepth{stackDepth}, priority{priority}, coreId{coreId} {}

    TaskHandle_t getHandle() { return this->taskHandle; }
    void run() { static_cast<T &>(*this).run(); }

    void runTask()
    {
        BaseType_t result = xTaskCreatePinnedToCore(task, _name, stackDepth, this, priority,
                                                    &this->taskHandle, coreId);
        assert("Failed to create task" && result == pdPASS);
    }

private:
    TaskHandle_t taskHandle;
    const char *_name;
    uint32_t stackDepth;
    UBaseType_t priority;
    const BaseType_t coreId;
    static void task(void *_params)
    {
        TaskThread *p = static_cast<TaskThread *>(_params);
        p->run();
    }
};