#ifndef MAINSCREENVIEW_HPP
#define MAINSCREENVIEW_HPP

#include <gui_generated/mainscreen_screen/MainScreenViewBase.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>

class MainScreenView : public MainScreenViewBase
{
public:
    MainScreenView();
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

    virtual ~MainScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // MAINSCREENVIEW_HPP
