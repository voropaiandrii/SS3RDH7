#include <gui/testingscreen_screen/TestingScreenView.hpp>
#include <gui/testingscreen_screen/TestingScreenPresenter.hpp>

TestingScreenPresenter::TestingScreenPresenter(TestingScreenView& v)
    : view(v)
{

}

void TestingScreenPresenter::activate()
{
	updateTestingButtonsState(getTestingState());
}

void TestingScreenPresenter::deactivate()
{

}

void TestingScreenPresenter::startTesting() {
	model->startTesting();
}

void TestingScreenPresenter::pauseTesting() {
	model->pauseTesting();
}

void TestingScreenPresenter::stopTesting() {
	model->stopTesting();
}

void TestingScreenPresenter::updateTestingButtonsState(uint8_t testingState) {
	if(testingState == TESTING_STATE_STARTED) {
		view.changeButtonState(BUTTON_ID_START_TESTING, BUTTON_STATE_DISABLED);
		view.changeButtonState(BUTTON_ID_PAUSE_TESTING, BUTTON_STATE_ENABLED);
		view.changeButtonState(BUTTON_ID_STOP_TESTING, BUTTON_STATE_ENABLED);
	} else if(testingState == TESTING_STATE_PAUSED) {
		view.changeButtonState(BUTTON_ID_START_TESTING, BUTTON_STATE_ENABLED);
		view.changeButtonState(BUTTON_ID_PAUSE_TESTING, BUTTON_STATE_DISABLED);
		view.changeButtonState(BUTTON_ID_STOP_TESTING, BUTTON_STATE_ENABLED);
	} else if(testingState == TESTING_STATE_STOPPED) {
		view.changeButtonState(BUTTON_ID_START_TESTING, BUTTON_STATE_ENABLED);
		view.changeButtonState(BUTTON_ID_PAUSE_TESTING, BUTTON_STATE_DISABLED);
		view.changeButtonState(BUTTON_ID_STOP_TESTING, BUTTON_STATE_DISABLED);
	} else if(testingState == TESTING_STATE_COMPLETED) {
		view.changeButtonState(BUTTON_ID_START_TESTING, BUTTON_STATE_ENABLED);
		view.changeButtonState(BUTTON_ID_PAUSE_TESTING, BUTTON_STATE_DISABLED);
		view.changeButtonState(BUTTON_ID_STOP_TESTING, BUTTON_STATE_DISABLED);
	}
}

void TestingScreenPresenter::printTestingOutput(const char* string) {
	view.printTestingOutput(string);
}

void TestingScreenPresenter::invalidateTestingScreen() {
	view.invalidateTestingScreen();
}

