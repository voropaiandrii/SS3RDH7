#ifndef MODEL_HPP
#define MODEL_HPP

#include "FreeRTOS.h"
#include "queue.h"

class ModelListener;

class GraphLimits {
	public:
		void clean();

		int minValue = 0;
		int maxValue = 0;
		uint32_t lastUpdateMillis = 0;
		uint8_t counter = 0;
};

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

    void makeScreenshot();
    void startRecording();
    void stopRecording();
    void connectDevices();
    void disconnectDevices();
    void cleanGraphs();

    void startTesting();
    void pauseTesting();
    void stopTesting();

    void printTestingOutput(const char* string);

    void setCPUUsage(uint8_t cpuUsage);
    void notifyTestStateChanged();
    void notifyMainStateChanged(const char* error);

    void changeTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
protected:
    ModelListener* modelListener;
private:
    uint32_t getTicks();
    void checkQueue(QueueHandle_t handle, GraphLimits* limits);
    void setGraphLimits(QueueHandle_t handle, GraphLimits* limits, bool resetLimits);
    void updateRTCTime();
};

#endif // MODEL_HPP
