#include <gui/mainscreen_screen/MainScreenView.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>

extern "C" {
	#include "domain/use_cases/recording_use_case.h"
}

MainScreenPresenter::MainScreenPresenter(MainScreenView& v)
    : view(v)
{

}

void MainScreenPresenter::activate()
{
	updateButtonsState(getRecordingStateUseCase(), getDevicesConnectedStateUseCase(), NULL);
}

void MainScreenPresenter::deactivate()
{

}

void MainScreenPresenter::updateStandardECGGraph(int newValue) {
	view.updateStandardECGGraph(newValue);
}

void MainScreenPresenter::updateInEarECGGraph(int newValue) {
	view.updateInEarECGGraph(newValue);
}

void MainScreenPresenter::updateLeftEarPPGGreenGraph(int newValue) {
	view.updateLeftEarPPGGreenGraph(newValue);
}

void MainScreenPresenter::updateLeftEarPPGRedGraph(int newValue) {
	view.updateLeftEarPPGRedGraph(newValue);
}

void MainScreenPresenter::updateLeftEarPPGIRGraph(int newValue) {
	view.updateLeftEarPPGIRGraph(newValue);
}

void MainScreenPresenter::updateRightEarPPGGreenGraph(int newValue) {
	view.updateRightEarPPGGreenGraph(newValue);
}

void MainScreenPresenter::updateRightEarPPGRedGraph(int newValue) {
	view.updateRightEarPPGRedGraph(newValue);
}

void MainScreenPresenter::updateRightEarPPGIRGraph(int newValue) {
	view.updateRightEarPPGIRGraph(newValue);
}

void MainScreenPresenter::updateFingerPPGRedGraph(int newValue) {
	view.updateFingerPPGRedGraph(newValue);
}

void MainScreenPresenter::updateFingerPPGIRGraph(int newValue) {
	view.updateFingerPPGIRGraph(newValue);
}

void MainScreenPresenter::setStandardECGGraphLimits(int minValue, int maxValue) {
	view.setStandardECGGraphLimits(minValue, maxValue);
}

void MainScreenPresenter::setInEarECGGraphLimits(int minValue, int maxValue) {
	view.setInEarECGGraphLimits(minValue, maxValue);
}

void MainScreenPresenter::setLeftEarPPGGreenLimits(int minValue, int maxValue) {
	view.setLeftEarPPGGreenLimits(minValue, maxValue);
}

void MainScreenPresenter::setLeftEarPPGRedLimits(int minValue, int maxValue) {
	view.setLeftEarPPGRedLimits(minValue, maxValue);
}

void MainScreenPresenter::setLeftEarPPGIRLimits(int minValue, int maxValue) {
	view.setLeftEarPPGIRLimits(minValue, maxValue);
}

void MainScreenPresenter::setRightEarPPGGreenLimits(int minValue, int maxValue) {
	view.setRightEarPPGGreenLimits(minValue, maxValue);
}

void MainScreenPresenter::setRightEarPPGRedLimits(int minValue, int maxValue) {
	view.setRightEarPPGRedLimits(minValue, maxValue);
}

void MainScreenPresenter::setRightEarPPGIRLimits(int minValue, int maxValue) {
	view.setRightEarPPGIRLimits(minValue, maxValue);
}

void MainScreenPresenter::setFingerPPGRedLimits(int minValue, int maxValue) {
	view.setFingerPPGRedLimits(minValue, maxValue);
}

void MainScreenPresenter::setFingerPPGIRLimits(int minValue, int maxValue) {
	view.setFingerPPGIRLimits(minValue, maxValue);
}

void MainScreenPresenter::updateTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds) {
	view.updateTime(years, months, days, hours, minutes, seconds);
}

void MainScreenPresenter::makeScreenshot()
{
    // Override and implement this function in MainScreen
	model->makeScreenshot();
}

void MainScreenPresenter::startRecording()
{
    // Override and implement this function in MainScreen
	model->startRecording();
}

void MainScreenPresenter::stopRecording()
{
    // Override and implement this function in MainScreen
	model->stopRecording();
}

void MainScreenPresenter::connectDevices()
{
    // Override and implement this function in MainScreen
	model->connectDevices();
}

void MainScreenPresenter::disconnectDevices()
{
    // Override and implement this function in MainScreen
	model->disconnectDevices();
}

void MainScreenPresenter::cleanGraphs()
{
    // Override and implement this function in MainScreen
	//model->cleanGraphs();
	view.cleanUI();

}

void MainScreenPresenter::updateButtonsState(uint8_t recordingState, uint8_t connectedState, const char* error) {

	if(connectedState == CONNECTING_STATE_CONNECTED) {
		view.changeButtonState(BUTTON_ID_CONNECT_DEVICES, BUTTON_STATE_DISABLED);
		view.changeButtonState(BUTTON_ID_DISCONNECT_DEVICES, BUTTON_STATE_ENABLED);
		if(recordingState == RECORDING_STATE_STARTED) {
			view.changeButtonState(BUTTON_ID_START_RECORDING, BUTTON_STATE_DISABLED);
			view.changeButtonState(BUTTON_ID_STOP_RECORDING, BUTTON_STATE_ENABLED);
			view.showRecordingCounter();
		} else if(recordingState == RECORDING_STATE_STOPPED) {
			view.changeButtonState(BUTTON_ID_START_RECORDING, BUTTON_STATE_ENABLED);
			view.changeButtonState(BUTTON_ID_STOP_RECORDING, BUTTON_STATE_DISABLED);
			view.hideRecordingCounter();
		} else if(recordingState == RECORDING_STATE_ERROR) {
			view.changeButtonState(BUTTON_ID_START_RECORDING, BUTTON_STATE_ENABLED);
			view.changeButtonState(BUTTON_ID_STOP_RECORDING, BUTTON_STATE_DISABLED);
			view.setRecordingError(error);
		}
	} else if(connectedState == CONNECTING_STATE_DISCONNECTED) {
		// Recording is not available if all devices are disconnected
		view.changeButtonState(BUTTON_ID_CONNECT_DEVICES, BUTTON_STATE_ENABLED);
		view.changeButtonState(BUTTON_ID_DISCONNECT_DEVICES, BUTTON_STATE_DISABLED);
		view.changeButtonState(BUTTON_ID_START_RECORDING, BUTTON_STATE_DISABLED);
		view.changeButtonState(BUTTON_ID_STOP_RECORDING, BUTTON_STATE_DISABLED);
		view.hideRecordingCounter();
	}
}
