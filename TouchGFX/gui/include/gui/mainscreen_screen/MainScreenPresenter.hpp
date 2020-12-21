#ifndef MAINSCREENPRESENTER_HPP
#define MAINSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <gui/common/CustomButtons.hpp>

using namespace touchgfx;

class MainScreenView;

class MainScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    MainScreenPresenter(MainScreenView& v);

    void updateStandardECGGraph(int newValue);
    void updateInEarECGGraph(int newValue);
    void updateLeftEarPPGGreenGraph(int newValue);
    void updateLeftEarPPGRedGraph(int newValue);
    void updateLeftEarPPGIRGraph(int newValue);
    void updateRightEarPPGGreenGraph(int newValue);
    void updateRightEarPPGRedGraph(int newValue);
    void updateRightEarPPGIRGraph(int newValue);
    void updateFingerPPGRedGraph(int newValue);
    void updateFingerPPGIRGraph(int newValue);

    void setStandardECGGraphLimits(int minValue, int maxValue);
    void setInEarECGGraphLimits(int minValue, int maxValue);
    void setLeftEarPPGGreenLimits(int minValue, int maxValue);
    void setLeftEarPPGRedLimits(int minValue, int maxValue);
    void setLeftEarPPGIRLimits(int minValue, int maxValue);
    void setRightEarPPGGreenLimits(int minValue, int maxValue);
    void setRightEarPPGRedLimits(int minValue, int maxValue);
    void setRightEarPPGIRLimits(int minValue, int maxValue);
    void setFingerPPGRedLimits(int minValue, int maxValue);
    void setFingerPPGIRLimits(int minValue, int maxValue);

    void updateTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);

    void makeScreenshot();
    void startRecording();
    void stopRecording();
    void connectDevices();
    void disconnectDevices();
    void cleanGraphs();

    void updateButtonsState(uint8_t recordingState, uint8_t connectedState, const char* error);
    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~MainScreenPresenter() {};

private:
    MainScreenPresenter();

    MainScreenView& view;
};

#endif // MAINSCREENPRESENTER_HPP
