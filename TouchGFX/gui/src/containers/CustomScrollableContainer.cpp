/*
 * CustomScrollableContainer.cpp
 *
 *  Created on: Dec 18, 2020
 *      Author: andrey
 */

#include <gui/containers/CustomScrollableContainer.hpp>

using namespace touchgfx;

bool CustomScrollableContainer::doCustomScroll(int16_t deltaX, int16_t deltaY) {
	return doScroll(deltaX, deltaY);
}


