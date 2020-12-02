/*
 * saved_data_application.h
 *
 *  Created on: Nov 23, 2020
 *      Author: andrey
 */

#ifndef INC_DATA_SAVED_DATA_APPLICATION_H_
#define INC_DATA_SAVED_DATA_APPLICATION_H_

#include "data/saved_data.h"

void setTimeSet(uint8_t);
uint8_t isTimeSet();
void saveDisplayBrigthness(uint8_t);
uint8_t readDisplayBrigthness();

#endif /* INC_DATA_SAVED_DATA_APPLICATION_H_ */
