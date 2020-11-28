#include <gui/mainscreen_screen/MainScreenView.hpp>

MainScreenView::MainScreenView()
{

}

void MainScreenView::setupScreen()
{
    MainScreenViewBase::setupScreen();
    standardECGGraphMajorYAxisGrid.setInterval(500);
    earECGGraphMajorYAxisGrid.setInterval(10000);

    standardECGGraph.setGraphRangeY(0, 0x0FFF);
    earECGGraph.setGraphRangeY(65535, -65535);

    fingerPPGRedGraph.setGraphRangeY(0, 0xFFFF);
    fingerPPGIRGraph.setGraphRangeY(0, 0xFFFF);

    leftEarPPGGreenGraph.setGraphRangeY(0, 0x3FFFF);
    leftEarPPGRedGraph.setGraphRangeY(0, 0x3FFFF);
    leftEarPPGIRGraph.setGraphRangeY(0, 0x3FFFF);
    rightEarPPGGreenGraph.setGraphRangeY(0, 0x3FFFF);
    rightEarPPGGreenGraph.setGraphRangeY(0, 0x3FFFF);
    rightEarPPGGreenGraph.setGraphRangeY(0, 0x3FFFF);


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
	digitalClock1.setTime24Hour(hours, minutes, seconds);
}
