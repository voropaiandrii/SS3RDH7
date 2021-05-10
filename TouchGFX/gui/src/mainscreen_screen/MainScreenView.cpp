#include <gui/mainscreen_screen/MainScreenView.hpp>
#include <gui/common/ViewUtils.hpp>

MainScreenView::MainScreenView()
{

}

void MainScreenView::setupScreen()
{
    MainScreenViewBase::setupScreen();

    standardECGGraph.setGraphRangeY(65535, -65535);
    standardECGGraphMajorYAxisGrid.setInterval(5000);

    earECGGraph.setGraphRangeY(0, 0xFFFF);
    earECGGraphMajorYAxisGrid.setInterval(5000);

    fingerPPGRedGraph.setGraphRangeY(0, 0xFFFFF);
    fingerPPGRedGraphMajorYAxisGrid.setInterval(40000);

    fingerPPGIRGraph.setGraphRangeY(0, 0xFFFFF);
    fingerPPGIRGraphMajorYAxisGrid.setInterval(40000);

    leftEarPPGGreenGraph.setGraphRangeY(0, 0x1FFFFF);
    leftEarPPGGreenGraphMajorYAxisGrid.setInterval(80000);

    leftEarPPGRedGraph.setGraphRangeY(0, 0x1FFFFF);
    leftEarPPGRedGraphMajorYAxisGrid.setInterval(80000);

    leftEarPPGIRGraph.setGraphRangeY(0, 0x1FFFFF);
    leftEarPPGIRGraphMajorYAxisGrid.setInterval(80000);

    rightEarPPGGreenGraph.setGraphRangeY(0, 0x1FFFFF);
    rightEarPPGGreenGraphMajorYAxisGrid.setInterval(80000);

    rightEarPPGRedGraph.setGraphRangeY(0, 0x1FFFFF);
    rightEarPPGRedGraphMajorYAxisGrid.setInterval(80000);

    rightEarPPGIRGraph.setGraphRangeY(0, 0x1FFFFF);
    rightEarPPGIRGraphMajorYAxisGrid.setInterval(80000);

    recordingDigitalClock.setVisible(false);

    /*
    fingerPPGRedGraph.setVisible(false);
    fingerPPGIRGraph.setVisible(false);

    leftEarPPGGreenGraph.setVisible(false);
    leftEarPPGRedGraph.setVisible(false);
    leftEarPPGIRGraph.setVisible(false);

    rightEarPPGGreenGraph.setVisible(false);
    rightEarPPGRedGraph.setVisible(false);
    rightEarPPGIRGraph.setVisible(false);

    cpuUsageTextProgress.setVisible(false);

    standardECGGraph.setPosition(50, 30, 740, 225);
    earECGGraph.setPosition(50, 240, 740, 225);
    */

}

void MainScreenView::tearDownScreen()
{
    MainScreenViewBase::tearDownScreen();
}

void MainScreenView::updateStandardECGGraph(int newValue) {
	standardECGGraph.addDataPoint(newValue);
}

void MainScreenView::updateInEarECGGraph(int newValue) {
	earECGGraph.addDataPoint(newValue);
}

void MainScreenView::updateLeftEarPPGGreenGraph(int newValue) {
	leftEarPPGGreenGraph.addDataPoint(newValue);
}

void MainScreenView::updateLeftEarPPGRedGraph(int newValue) {
	leftEarPPGRedGraph.addDataPoint(newValue);
}

void MainScreenView::updateLeftEarPPGIRGraph(int newValue) {
	leftEarPPGIRGraph.addDataPoint(newValue);
}

void MainScreenView::updateRightEarPPGGreenGraph(int newValue) {
	rightEarPPGGreenGraph.addDataPoint(newValue);
}

void MainScreenView::updateRightEarPPGRedGraph(int newValue) {
	rightEarPPGRedGraph.addDataPoint(newValue);
}

void MainScreenView::updateRightEarPPGIRGraph(int newValue) {
	rightEarPPGIRGraph.addDataPoint(newValue);
}

void MainScreenView::updateFingerPPGRedGraph(int newValue) {
	fingerPPGRedGraph.addDataPoint(newValue);
}

void MainScreenView::updateFingerPPGIRGraph(int newValue) {
	fingerPPGIRGraph.addDataPoint(newValue);
}

void MainScreenView::setStandardECGGraphLimits(int minValue, int maxValue) {
	standardECGGraph.setGraphRange(0, 100, minValue, maxValue);
}

void MainScreenView::setInEarECGGraphLimits(int minValue, int maxValue) {
	earECGGraph.setGraphRange(0, 100, minValue, maxValue);
}

void MainScreenView::setLeftEarPPGGreenLimits(int minValue, int maxValue) {
	leftEarPPGGreenGraph.setGraphRange(0, 100, minValue, maxValue);
}

void MainScreenView::setLeftEarPPGRedLimits(int minValue, int maxValue) {
	leftEarPPGRedGraph.setGraphRange(0, 100, minValue, maxValue);
}

void MainScreenView::setLeftEarPPGIRLimits(int minValue, int maxValue) {
	leftEarPPGIRGraph.setGraphRange(0, 100, minValue, maxValue);
}

void MainScreenView::setRightEarPPGGreenLimits(int minValue, int maxValue) {
	rightEarPPGGreenGraph.setGraphRange(0, 100, minValue, maxValue);
}

void MainScreenView::setRightEarPPGRedLimits(int minValue, int maxValue) {
	rightEarPPGRedGraph.setGraphRange(0, 100, minValue, maxValue);
}

void MainScreenView::setRightEarPPGIRLimits(int minValue, int maxValue) {
	rightEarPPGIRGraph.setGraphRange(0, 100, minValue, maxValue);
}

void MainScreenView::setFingerPPGRedLimits(int minValue, int maxValue) {
	fingerPPGRedGraph.setGraphRange(0, 100, minValue, maxValue);
}

void MainScreenView::setFingerPPGIRLimits(int minValue, int maxValue) {
	fingerPPGIRGraph.setGraphRange(0, 100, minValue, maxValue);
}

void MainScreenView::updateTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds) {
	if(currentSeconds != seconds) {
		currentSeconds = seconds;
		realTimeDigitalClock.setTime24Hour(hours, minutes, seconds);
		if(presenter->isRecording() && recordingDigitalClock.isVisible()) {
			recorderCounter++;
			uint32_t counterValue = recorderCounter;
			uint32_t recordedHours = (counterValue / 3600);
			counterValue -= recordedHours * 3600;
			uint32_t recordedMinutes = (counterValue / 60);
			counterValue -= recordedMinutes * 60;
			uint32_t recordedSeconds = counterValue;
			recordingDigitalClock.setTime24Hour(recordedHours, recordedMinutes, recordedSeconds);

		}
	}
}

void MainScreenView::makeScreenshot()
{
	presenter->makeScreenshot();
}

void MainScreenView::startRecording()
{
	recorderCounter = 0;
	presenter->startRecording();
	hideRecordingError();
}

void MainScreenView::stopRecording()
{
	presenter->stopRecording();
}

void MainScreenView::connectDevices()
{
	presenter->connectDevices();
}

void MainScreenView::disconnectDevices()
{
	presenter->disconnectDevices();
}

void MainScreenView::cleanGraphs()
{
	presenter->cleanGraphs();
}

void MainScreenView::changeButtonState(ButtonID buttonId, ButtonState state) {
	Button* button = NULL;
	switch(buttonId) {
		case BUTTON_ID_MAKE_SCREENSHOT:
			button = &makeScreenshotButton;
			break;
		case BUTTON_ID_START_RECORDING:
			button = &startRecordingButton;
			break;
		case BUTTON_ID_STOP_RECORDING:
			button = &stopRecordingButton;
			break;
		case BUTTON_ID_CONNECT_DEVICES:
			button = &connectDevicesButton;
			break;
		case BUTTON_ID_DISCONNECT_DEVICES:
			button = &disconnectDevicesButton;
			break;
		case BUTTON_ID_CLEAN:
			button = &cleanButton;
			break;
		default:
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

void MainScreenView::showRecordingCounter() {
	recordingDigitalClock.setVisible(true);
	recordingDigitalClock.setTime24Hour(0, 0, 0);
	recordingDigitalClock.invalidate();
}

void MainScreenView::hideRecordingCounter() {
	recordingDigitalClock.setVisible(false);
	recordingDigitalClock.invalidate();
}

void MainScreenView::showRecordingError() {
	errorTextArea.setVisible(true);
	errorTextArea.invalidate();
}

void MainScreenView::hideRecordingError() {
	errorTextArea.setVisible(false);
	errorTextArea.invalidate();
}

void MainScreenView::cleanUI() {
	standardECGGraph.clear();
	earECGGraph.clear();
	fingerPPGRedGraph.clear();
	fingerPPGIRGraph.clear();
	leftEarPPGGreenGraph.clear();
	leftEarPPGRedGraph.clear();
	leftEarPPGIRGraph.clear();
	rightEarPPGGreenGraph.clear();
	rightEarPPGRedGraph.clear();
	rightEarPPGIRGraph.clear();

	standardECGGraph.invalidate();
	earECGGraph.invalidate();
	fingerPPGRedGraph.invalidate();
	fingerPPGIRGraph.invalidate();
	leftEarPPGGreenGraph.invalidate();
	leftEarPPGRedGraph.invalidate();
	leftEarPPGIRGraph.invalidate();
	rightEarPPGGreenGraph.invalidate();
	rightEarPPGRedGraph.invalidate();
	rightEarPPGIRGraph.invalidate();
}

void MainScreenView::setRecordingError(const char* error) {
	if(error != NULL) {
		//presenter->stopRecording();
		Unicode::fromUTF8((const uint8_t*)error, errorTextAreaBuffer, ERRORTEXTAREA_SIZE);
		showRecordingError();
	}
}
