/*
 * ViewUtils.cpp
 *
 *  Created on: Dec 17, 2020
 *      Author: andrey
 */


#include <gui/common/ViewUtils.hpp>

namespace touchgfx
{
	void ViewUtils::disableButton(Button* button) {
		button->setTouchable(false);
		button->setAlpha(127);
		button->invalidate();
	}

	void ViewUtils::enableButton(Button* button) {
		button->setTouchable(true);
		button->setAlpha(255);
		button->invalidate();
	}
}
