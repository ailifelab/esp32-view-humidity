#include <Arduino.h>

#include "taskThread.h"

class SensorTask : public TaskThread<SensorTask> {
  friend class TaskThread<SensorTask>;

public:
  SensorTask();
  ~SensorTask();

  void addQueue(QueueHandle_t queue);

private:
  QueueHandle_t queue;

  std::vector<QueueHandle_t> queues;
};