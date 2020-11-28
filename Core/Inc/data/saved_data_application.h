/*
 * saved_data_application.h
 *
 *  Created on: Nov 23, 2020
 *      Author: andrey
 */

#ifndef INC_DATA_SAVED_DATA_APPLICATION_H_
#define INC_DATA_SAVED_DATA_APPLICATION_H_

#include "data/saved_data.h"

#ifdef __cplusplus
extern "C" {
#endif
	void setTimeSet(uint8_t);
	uint8_t isTimeSet();
	void saveDisplayBrigthness(uint8_t);
	uint8_t readDisplayBrigthness();
#ifdef __cplusplus
}
#endif

#endif /* INC_DATA_SAVED_DATA_APPLICATION_H_ */
