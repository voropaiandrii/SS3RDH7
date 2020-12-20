#include <gui/settingsscreen_screen/SettingsScreenView.hpp>
#include <gui/settingsscreen_screen/SettingsScreenPresenter.hpp>

SettingsScreenPresenter::SettingsScreenPresenter(SettingsScreenView& v)
    : view(v)
{

}

void SettingsScreenPresenter::activate()
{

}

void SettingsScreenPresenter::deactivate()
{

}

void SettingsScreenPresenter::setBrigthness(int brigthnessPercentage)
{
	model->setDisplayBrigthness(brigthnessPercentage);
}

int SettingsScreenPresenter::getBrigthness()
{
	return model->getDisplayBrigthness();
}

void SettingsScreenPresenter::updateTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds) {
	view.updateTime(years, months, days, hours, minutes, seconds);
}


void SettingsScreenPresenter::changeTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds) {
	model->changeTime(years, months, days, hours, minutes, seconds);
}
