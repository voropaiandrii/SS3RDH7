/*
 * CustomScrollableContainer.hpp
 *
 *  Created on: Dec 18, 2020
 *      Author: andrey
 */

#ifndef GUI_INCLUDE_GUI_CONTAINERS_CUSTOMSCROLLABLECONTAINER_HPP_
#define GUI_INCLUDE_GUI_CONTAINERS_CUSTOMSCROLLABLECONTAINER_HPP_

#include <touchgfx/containers/ScrollableContainer.hpp>

namespace touchgfx
{
	class CustomScrollableContainer: public ScrollableContainer {
		public:
			bool doCustomScroll(int16_t deltaX, int16_t deltaY);
	};
}



#endif /* GUI_INCLUDE_GUI_CONTAINERS_CUSTOMSCROLLABLECONTAINER_HPP_ */
