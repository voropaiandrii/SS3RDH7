#ifndef MODEL_HPP
#define MODEL_HPP

#include "FreeRTOS.h"
#include "queue.h"

class ModelListener;

typedef struct {
	int minValue = 0;
	int maxValue = 0;
	uint32_t lastUpdateMillis = 0;
	uint8_t counter = 0;
} GraphLimits;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    void setDisplayBrigthness(int percentage);
    int getDisplayBrigthness();
protected:
    ModelListener* modelListener;
private:
    uint32_t getTicks();
    void checkQueue(QueueHandle_t handle, GraphLimits* limits);
    void setGraphLimits(QueueHandle_t handle, GraphLimits* limits, bool resetLimits);
    void updateRTCTime();
};

#endif // MODEL_HPP
