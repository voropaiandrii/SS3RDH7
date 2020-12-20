#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "stdio.h"

extern "C" {
	#include "time/time.h"
	#include "display/display.h"
	#include "data/saved_data_application.h"
	#include "domain/use_cases/recording_use_case.h"
	#include "domain/use_cases/testing_use_case.h"
}

extern "C" QueueHandle_t standartECGQueue;
extern "C" QueueHandle_t earECGQueue;
extern "C" QueueHandle_t earPPGLeftGreenQueue;
extern "C" QueueHandle_t earPPGLeftRedQueue;
extern "C" QueueHandle_t earPPGLeftIRQueue;
extern "C" QueueHandle_t earPPGRightGreenQueue;
extern "C" QueueHandle_t earPPGRightRedQueue;
extern "C" QueueHandle_t earPPGRightIRQueue;
extern "C" QueueHandle_t fingerPPGRedQueue;
extern "C" QueueHandle_t fingerPPGIRQueue;

int counter = 0;
int value = 0;
uint8_t initModel = 0;

GraphLimits standardECGLimits;
GraphLimits inEarECGMLimits;
GraphLimits leftEarPPGGreenLimits;
GraphLimits leftEarPPGRedLimits;
GraphLimits leftEarPPGIRLimits;
GraphLimits rightEarPPGGreenLimits;
GraphLimits rightEarPPGRedLimits;
GraphLimits rightEarPPGIRLimits;
GraphLimits fingerPPGRedLimits;
GraphLimits fingerPPGIIRLimits;

void GraphLimits::clean() {
	this->counter = 0;
	this->minValue = 0;
	this->maxValue = 0;
	this->lastUpdateMillis = 0;
}

Model::Model() : modelListener(0)
{
	/*
	modelListener->setStandardECGGraphLimits(0, 0x0FFF);
	modelListener->setInEarECGGraphLimits(0xFF, 0xFF);
	modelListener->setLeftEarPPGGreenLimits(0, 0xFFFFF);
	modelListener->setLeftEarPPGRedLimits(0, 0xFFFFF);
	modelListener->setLeftEarPPGIRLimits(0, 0xFFFFF);
	modelListener->setRightEarPPGGreenLimits(0, 0xFFFFF);
	modelListener->setRightEarPPGRedLimits(0, 0xFFFFF);
	modelListener->setRightEarPPGIRLimits(0, 0xFFFFF);
	modelListener->setFingerPPGRedLimits(0, 0xFFFFF);
	modelListener->setFingerPPGIRLimits(0, 0xFFFFF);
	*/

	setDisplayBrigthness((int)readDisplayBrigthness());
}

uint32_t Model::getTicks() {
	return HAL_GetTick();
}

void Model::tick()
{
	//checkQueues();

	checkQueue(standartECGQueue, &standardECGLimits);
	checkQueue(earECGQueue, &inEarECGMLimits);
	checkQueue(earPPGLeftGreenQueue, &leftEarPPGGreenLimits);
	checkQueue(earPPGLeftRedQueue, &leftEarPPGRedLimits);
	checkQueue(earPPGLeftIRQueue, &leftEarPPGIRLimits);
	checkQueue(earPPGRightGreenQueue, &rightEarPPGGreenLimits);
	checkQueue(earPPGRightRedQueue, &rightEarPPGRedLimits);
	checkQueue(earPPGRightIRQueue, &rightEarPPGIRLimits);
	checkQueue(fingerPPGRedQueue, &fingerPPGRedLimits);
	checkQueue(fingerPPGIRQueue, &fingerPPGIIRLimits);
	if(counter > 1) {

		counter = 0;
		//modelListener->updateStandardECGGraph(value);
		//modelListener->updateInEarECGGraph(value);
		//modelListener->updateLeftEarPPGGreenGraph(value);
		//modelListener->updateLeftEarPPGRedGraph(value);
		//modelListener->updateLeftEarPPGIRGraph(value);
		//modelListener->updateRightEarPPGGreenGraph(value);
		//modelListener->updateRightEarPPGRedGraph(value);
		//modelListener->updateRightEarPPGIRGraph(value);
		//modelListener->updateFingerPPGRedGraph(value);
		//modelListener->updateFingerPPGIRGraph(value);

		if(value >= 99) {
			value = 0;
			//calculateLimits(true);
		} else {
			value++;
		}
	} else {
		counter++;
	}

	updateRTCTime();

	if(initModel == 0) {
		initModel = 1;
		modelListener->updateButtonsState(isRecordingUseCase(), isDevicesConnectedUseCase());
		//modelListener->updateTestingButtonsState(getTestingState());
	}

	//modelListener->invalidateTestingScreen();

}

void Model::setDisplayBrigthness(int percentage) {
	saveDisplayBrigthness((uint8_t)percentage);
	setBrightness(percentage);
}

int Model::getDisplayBrigthness() {
	return (int)readDisplayBrigthness();
}

void Model::checkQueue(QueueHandle_t queueHandle, GraphLimits* limits) {
	if(queueHandle != 0) {
		int message = 0;
		while(xQueuePeek(queueHandle, (void*)&message, (TickType_t)10)) {
			xQueueReceive(queueHandle, (void*)&message, (TickType_t)10);
			if(modelListener != 0) {
				uint32_t currentMillis = getTicks();
				bool updateLimits = false;
				bool resetLimits = false;

				if(message > limits->maxValue) {
					int newMaxLimit = ((message + limits->maxValue) / 2);
					limits->maxValue = newMaxLimit + 100;
					//limits->maxValue = message + 100;
					updateLimits = currentMillis > limits->lastUpdateMillis + 5000;
				}
				if(message < limits->minValue) {

					if(message > 0) {
						int newMinLimit =  ((message + limits->minValue) / 2);
						limits->minValue = newMinLimit - 100;
						//limits->minValue = newMinLimit;
					} else {
						int newMinLimit =  ((message - limits->minValue) / 2);
						limits->minValue = newMinLimit - 100;
						//limits->minValue = newMinLimit;
					}
					//limits->minValue = message - 100;
					updateLimits = currentMillis > limits->lastUpdateMillis + 5000;
				}


				if(limits->counter < 99) {
					limits->counter++;
				} else {
					limits->counter = 0;
					updateLimits = true;
					resetLimits = true;
				}


				if(queueHandle == standartECGQueue) {
					modelListener->updateStandardECGGraph(message);
					if(updateLimits) {
						setGraphLimits(standartECGQueue, limits, resetLimits);
					}
				} else if(queueHandle == earECGQueue) {
					modelListener->updateInEarECGGraph(message);
					if(updateLimits) {
						setGraphLimits(earECGQueue, limits, resetLimits);
					}
				} else if(queueHandle == earPPGLeftGreenQueue) {
					modelListener->updateLeftEarPPGGreenGraph(message);
					if(updateLimits) {
						setGraphLimits(earPPGLeftGreenQueue, limits, resetLimits);
					}
				} else if(queueHandle == earPPGLeftRedQueue) {
					modelListener->updateLeftEarPPGRedGraph(message);
					if(updateLimits) {
						setGraphLimits(earPPGLeftRedQueue, limits, resetLimits);
					}
				} else if(queueHandle == earPPGLeftIRQueue) {
					modelListener->updateLeftEarPPGIRGraph(message);
					if(updateLimits) {
						setGraphLimits(earPPGLeftIRQueue, limits, resetLimits);
					}
				} else if(queueHandle == earPPGRightGreenQueue) {
					modelListener->updateRightEarPPGGreenGraph(message);
					if(updateLimits) {
						setGraphLimits(earPPGRightGreenQueue, limits, resetLimits);
					}
				} else if(queueHandle == earPPGRightRedQueue) {
					modelListener->updateRightEarPPGRedGraph(message);
					if(updateLimits) {
						setGraphLimits(earPPGRightRedQueue, limits, resetLimits);
					}
				} else if(queueHandle == earPPGRightIRQueue) {
					modelListener->updateRightEarPPGIRGraph(message);
					if(updateLimits) {
						setGraphLimits(earPPGRightIRQueue, limits, resetLimits);
					}
				} else if(queueHandle == fingerPPGRedQueue) {
					modelListener->updateFingerPPGRedGraph(message);
					if(updateLimits) {
						setGraphLimits(fingerPPGRedQueue, limits, resetLimits);
					}
				} else if(queueHandle == fingerPPGIRQueue) {
					modelListener->updateFingerPPGIRGraph(message);
					if(updateLimits) {
						setGraphLimits(fingerPPGIRQueue, limits, resetLimits);
					}
				}


			}
		}
	}
}

void Model::setGraphLimits(QueueHandle_t queueHandle, GraphLimits* limits, bool resetLimits) {
	if(limits->maxValue != 0 && limits->maxValue > limits->minValue) {
		//printf("limits->maxValue: %d, limits->minValue: %d\n", limits->maxValue, limits->minValue);

		if(queueHandle == standartECGQueue) {
			modelListener->setStandardECGGraphLimits(limits->minValue, limits->maxValue);
		} else if(queueHandle == earECGQueue) {
			modelListener->setInEarECGGraphLimits(limits->minValue, limits->maxValue);
		} else if(queueHandle == earPPGLeftGreenQueue) {
			modelListener->setLeftEarPPGGreenLimits(limits->minValue, limits->maxValue);
		} else if(queueHandle == earPPGLeftRedQueue) {
			modelListener->setLeftEarPPGRedLimits(limits->minValue, limits->maxValue);
		} else if(queueHandle == earPPGLeftIRQueue) {
			modelListener->setLeftEarPPGIRLimits(limits->minValue, limits->maxValue);
		} else if(queueHandle == earPPGRightGreenQueue) {
			modelListener->setRightEarPPGGreenLimits(limits->minValue, limits->maxValue);
		} else if(queueHandle == earPPGRightRedQueue) {
			modelListener->setRightEarPPGRedLimits(limits->minValue, limits->maxValue);
		} else if(queueHandle == earPPGRightIRQueue) {
			modelListener->setRightEarPPGIRLimits(limits->minValue, limits->maxValue);
		} else if(queueHandle == fingerPPGRedQueue) {
			modelListener->setFingerPPGRedLimits(limits->minValue, limits->maxValue);
		} else if(queueHandle == fingerPPGIRQueue) {
			modelListener->setFingerPPGIRLimits(limits->minValue, limits->maxValue);
		}

		if(resetLimits) {
			limits->minValue = limits->maxValue;
			limits->maxValue = 0;
		}
	}
	limits->lastUpdateMillis = getTicks();
}

void Model::updateRTCTime() {

	RTCDate currentDate = getCurrentRTCDate();
	//printf("currentDate.seconds: %u\n", currentDate.seconds);
	modelListener->updateTime(currentDate.years,
			currentDate.months,
			currentDate.days,
			currentDate.hours,
			currentDate.minutes,
			currentDate.seconds);

}

void Model::makeScreenshot()
{
    // Override and implement this function in MainScreen
}

void Model::startRecording()
{
	startRecordingUseCase();
	//modelListener->updateButtonsState(isRecordingUseCase(), isDevicesConnectedUseCase());
}

void Model::stopRecording()
{
	stopRecordingUseCase();
	//modelListener->updateButtonsState(isRecordingUseCase(), isDevicesConnectedUseCase());
}

void Model::connectDevices()
{
	connectAllDevicesUseCase();
	//modelListener->updateButtonsState(isRecordingUseCase(), isDevicesConnectedUseCase());
}

void Model::disconnectDevices()
{
	disconnectAllDevicesUseCase();
	//modelListener->updateButtonsState(isRecordingUseCase(), isDevicesConnectedUseCase());
}

void Model::cleanGraphs()
{
	standardECGLimits.clean();
	inEarECGMLimits.clean();
	leftEarPPGGreenLimits.clean();
	leftEarPPGRedLimits.clean();
	leftEarPPGIRLimits.clean();
	rightEarPPGGreenLimits.clean();
	rightEarPPGRedLimits.clean();
	rightEarPPGIRLimits.clean();
	fingerPPGRedLimits.clean();
	fingerPPGIIRLimits.clean();
}

void Model::startTesting() {
	startTestingUseCase();
	//modelListener->updateTestingButtonsState(getTestingState());
}

void Model::pauseTesting() {
	pauseTestingUseCase();
	//modelListener->updateTestingButtonsState(getTestingState());
}

void Model::stopTesting() {
	stopTestingUseCase();
	//modelListener->updateTestingButtonsState(getTestingState());
}

void Model::printTestingOutput(const char* string) {
	modelListener->printTestingOutput(string);
}

void Model::setCPUUsage(uint8_t cpuUsage) {

}

void Model::notifyTestStateChanged() {
	modelListener->updateTestingButtonsState(getTestingState());
}

void Model::notifyMainStateChanged() {
	modelListener->updateButtonsState(isRecordingUseCase(), isDevicesConnectedUseCase());
}

void Model::changeTime(uint16_t years, uint8_t months, uint8_t days, uint8_t hours, uint8_t minutes, uint8_t seconds) {
	RTCDate currentDate;
	currentDate.years = years;
	currentDate.months = months;
	currentDate.days = days;
	currentDate.hours = hours;
	currentDate.minutes = minutes;
	currentDate.seconds = seconds;
	setCurrentRTCDate(&currentDate);
}
