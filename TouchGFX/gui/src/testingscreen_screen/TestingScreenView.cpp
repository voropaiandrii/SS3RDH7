#include <gui/testingscreen_screen/TestingScreenView.hpp>
#include <gui/common/ViewUtils.hpp>

TestingScreenView::TestingScreenView()
{

}

void TestingScreenView::setupScreen()
{
    TestingScreenViewBase::setupScreen();
}

void TestingScreenView::tearDownScreen()
{
    TestingScreenViewBase::tearDownScreen();
}

void TestingScreenView::startTesting() {
	presenter->startTesting();
}

void TestingScreenView::pauseTesting() {
	presenter->pauseTesting();
}

void TestingScreenView::stopTesting() {
	presenter->stopTesting();
}

void TestingScreenView::changeButtonState(ButtonID buttonId, ButtonState state) {
	Button* button = NULL;
	switch(buttonId) {
		case BUTTON_ID_START_TESTING:
			button = &runTestingButton;
			break;
		case BUTTON_ID_PAUSE_TESTING:
			button = &pauseTestingButton;
			break;
		case BUTTON_ID_STOP_TESTING:
			button = &stopTestingButton;
			break;
	}

	if(button != NULL) {
		if(state == BUTTON_STATE_ENABLED) {
			ViewUtils::enableButton(button);
		} else {
			ViewUtils::disableButton(button);
		}
	}
}

void TestingScreenView::printTestingOutput(char* string) {
	//Unicode::snprintf(textOutputBuffer, TEXTOUTPUT_SIZE, "%s", string);
	Unicode::fromUTF8((uint8_t*)string, textOutputBuffer, TEXTOUTPUT_SIZE);
	textOutput.invalidate();
}
