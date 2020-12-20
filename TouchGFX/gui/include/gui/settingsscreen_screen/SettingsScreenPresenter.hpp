#ifndef SETTINGSSCREENPRESENTER_HPP
#define SETTINGSSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class SettingsScreenView;

class SettingsScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    SettingsScreenPresenter(SettingsScreenView& v);

    void setBrigthness(int brigthnessPercentage);

    int getBrigthness();
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

    virtual ~SettingsScreenPresenter() {};

    void updateTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);

    void changeTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);

private:
    SettingsScreenPresenter();

    SettingsScreenView& view;
};

#endif // SETTINGSSCREENPRESENTER_HPP
