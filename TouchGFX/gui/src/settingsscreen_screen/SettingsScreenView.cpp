#include <gui/settingsscreen_screen/SettingsScreenView.hpp>

SettingsScreenView::SettingsScreenView()
{

}

void SettingsScreenView::setupScreen()
{
    SettingsScreenViewBase::setupScreen();
    screenBrightnessSlider.setValue(((SettingsScreenPresenter*)(presenter))->getBrigthness());
}

void SettingsScreenView::tearDownScreen()
{
    SettingsScreenViewBase::tearDownScreen();
}

void SettingsScreenView::screenBrightnessChanged(int value) {
	((SettingsScreenPresenter*)(presenter))->setBrigthness(value);
}

void SettingsScreenView::updateTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds) {
	//hoursTextArea.setTypedText(t)
}
