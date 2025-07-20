/*
 * ViewUtils.hpp
 *
 *  Created on: Dec 17, 2020
 *      Author: andrey
 */

#ifndef GUI_INCLUDE_GUI_COMMON_VIEWUTILS_HPP_
#define GUI_INCLUDE_GUI_COMMON_VIEWUTILS_HPP_

#include <touchgfx/widgets/Button.hpp>

namespace touchgfx
{
	class ViewUtils {
		public:
			static void disableButton(Button* button);
			static void enableButton(Button* button);
	};
}


#endif /* GUI_INCLUDE_GUI_COMMON_VIEWUTILS_HPP_ */
