#ifndef SETTINGSSCREENVIEW_HPP
#define SETTINGSSCREENVIEW_HPP

#include <gui_generated/settingsscreen_screen/SettingsScreenViewBase.hpp>
#include <gui/settingsscreen_screen/SettingsScreenPresenter.hpp>

class SettingsScreenView : public SettingsScreenViewBase
{
public:
    SettingsScreenView();
    virtual void screenBrightnessChanged(int value);
    virtual ~SettingsScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void updateTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
protected:
};

#endif // SETTINGSSCREENVIEW_HPP
