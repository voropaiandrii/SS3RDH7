#include <gui/mainscreen_screen/MainScreenView.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>

MainScreenPresenter::MainScreenPresenter(MainScreenView& v)
    : view(v)
{

}

void MainScreenPresenter::activate()
{

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
