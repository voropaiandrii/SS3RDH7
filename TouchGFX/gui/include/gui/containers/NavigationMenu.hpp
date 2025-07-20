#ifndef NAVIGATIONMENU_HPP
#define NAVIGATIONMENU_HPP

#include <gui_generated/containers/NavigationMenuBase.hpp>

class NavigationMenu : public NavigationMenuBase
{
public:
    NavigationMenu();
    virtual ~NavigationMenu() {}

    virtual void initialize();
protected:
};

#endif // NAVIGATIONMENU_HPP
