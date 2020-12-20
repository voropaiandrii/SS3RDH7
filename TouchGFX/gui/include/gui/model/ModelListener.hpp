#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    virtual void updateStandardECGGraph(int newValue) {}
    virtual void updateInEarECGGraph(int newValue) {}
    virtual void updateLeftEarPPGGreenGraph(int newValue) {}
    virtual void updateLeftEarPPGRedGraph(int newValue) {}
    virtual void updateLeftEarPPGIRGraph(int newValue) {}
    virtual void updateRightEarPPGGreenGraph(int newValue) {}
    virtual void updateRightEarPPGRedGraph(int newValue) {}
    virtual void updateRightEarPPGIRGraph(int newValue) {}
    virtual void updateFingerPPGRedGraph(int newValue) {}
    virtual void updateFingerPPGIRGraph(int newValue) {}

    virtual void setStandardECGGraphLimits(int minValue, int maxValue) {}
    virtual void setInEarECGGraphLimits(int minValue, int maxValue) {}
    virtual void setLeftEarPPGGreenLimits(int minValue, int maxValue) {}
    virtual void setLeftEarPPGRedLimits(int minValue, int maxValue) {}
    virtual void setLeftEarPPGIRLimits(int minValue, int maxValue) {}
    virtual void setRightEarPPGGreenLimits(int minValue, int maxValue) {}
    virtual void setRightEarPPGRedLimits(int minValue, int maxValue) {}
    virtual void setRightEarPPGIRLimits(int minValue, int maxValue) {}
    virtual void setFingerPPGRedLimits(int minValue, int maxValue) {}
    virtual void setFingerPPGIRLimits(int minValue, int maxValue) {}

    virtual void updateTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds) {}

    virtual void makeScreenshot() {}
    virtual void startRecording() {}
    virtual void stopRecording() {}
    virtual void connectDevices() {}
    virtual void disconnectDevices() {}
    virtual void cleanGraphs() {}

    virtual void updateButtonsState(uint8_t isRecording, uint8_t isConnected) {}
    virtual void updateTestingButtonsState(uint8_t testingState) {}

    virtual void printTestingOutput(const char* string) {}

    virtual void invalidateTestingScreen() {}

    //virtual void changeTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds) {}

    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
