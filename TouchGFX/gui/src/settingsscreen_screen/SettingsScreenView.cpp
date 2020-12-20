#include <gui/settingsscreen_screen/SettingsScreenView.hpp>

static uint8_t mSeconds = 0;
static uint8_t mMinutes = 0;
static uint8_t mHours = 0;
static uint8_t mDays = 0;
static uint8_t mMonths = 0;
static uint16_t mYears = 0;

static int8_t secondsOffset = 0;
static int8_t minutesOffset = 0;
static int8_t hoursOffset = 0;
static int8_t daysOffset = 0;
static int8_t monthsOffset = 0;
static int16_t yearsOffset = 0;

static bool isSaveButtonClicked = false;

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
	mSeconds = seconds;
	mMinutes = minutes;
	mHours = hours;
	mDays = days;
	mMonths = months;
	mYears = years;
	updateTimeUI();
}

void SettingsScreenView::saveSettings()
{
	isSaveButtonClicked = true;
}

void SettingsScreenView::increaseSeconds()
{
    if(secondsOffset < 59) {
    	secondsOffset++;
    }
}

void SettingsScreenView::decreaseSeconds()
{
	if(secondsOffset > -59) {
		secondsOffset--;
	}
}

void SettingsScreenView::increaseMinutes()
{
	if(minutesOffset < 59) {
		minutesOffset++;
	}
}

void SettingsScreenView::decreaseMinutes()
{
	if(minutesOffset > -59) {
		minutesOffset--;
	}
}

void SettingsScreenView::increaseHours()
{
    if(hoursOffset < 23) {
    	hoursOffset++;
    }
}

void SettingsScreenView::decreaseHours()
{
	if(hoursOffset > -23) {
		hoursOffset--;
	}
}

void SettingsScreenView::increaseYear()
{
	if(yearsOffset < 2050) {
		yearsOffset++;
	}
}

void SettingsScreenView::decreaseYear()
{
	if(yearsOffset > -2050) {
		yearsOffset--;
	}
}

void SettingsScreenView::increaseMonths()
{
	if(monthsOffset < 12) {
		monthsOffset++;
	}
}

void SettingsScreenView::decreaseMonths()
{
	if(monthsOffset > -12) {
		monthsOffset--;
	}
}

void SettingsScreenView::increaseDays()
{
	if(daysOffset < 31) {
		daysOffset++;
	}
}

void SettingsScreenView::decreaseDays()
{
	if(daysOffset > -31) {
		daysOffset--;
	}
}

void SettingsScreenView::updateTimeUI() {

	uint8_t seconds = mSeconds + secondsOffset;
	if(seconds > 59) {
		seconds = 0;
	}

	uint8_t minutes = mMinutes + minutesOffset;
	if(minutes > 59) {
		minutes = 0;
	}

	uint8_t hours = mHours + hoursOffset;
	if(hours > 23) {
		hours = 0;
	}

	uint8_t days = mDays + daysOffset;
	if(days > 31) {
		days = 1;
	}

	uint8_t months = mMonths + monthsOffset;
	if(months > 12) {
		months = 1;
	}

	uint16_t years = mYears + yearsOffset;
	if(years > 2050) {
		years = 2000;
	}

	Unicode::snprintf(dayTextAreaBuffer, SECONDSTEXTAREA_SIZE, "%02d", days);
	Unicode::snprintf(textMonthAreaBuffer, SECONDSTEXTAREA_SIZE, "%02d", months);
	Unicode::snprintf(yearTextAreaBuffer, SECONDSTEXTAREA_SIZE, "%04d", years);
	Unicode::snprintf(secondsTextAreaBuffer, SECONDSTEXTAREA_SIZE, "%02d", seconds);
	Unicode::snprintf(minutesTextAreaBuffer, SECONDSTEXTAREA_SIZE, "%02d", minutes);
	Unicode::snprintf(hoursTextAreaBuffer, SECONDSTEXTAREA_SIZE, "%02d", hours);

    dayTextArea.invalidate();
    textMonthArea.invalidate();
    yearTextArea.invalidate();
    hoursTextArea.invalidate();
    minutesTextArea.invalidate();
    secondsTextArea.invalidate();

    if(isSaveButtonClicked) {
    	isSaveButtonClicked = false;
    	((SettingsScreenPresenter*)(presenter))->changeTime(years, months, days, hours, minutes, seconds);
    	secondsOffset = 0;
    	minutesOffset = 0;
    	hoursOffset = 0;
    	daysOffset = 0;
    	monthsOffset = 0;
    	yearsOffset = 0;
    }
}
