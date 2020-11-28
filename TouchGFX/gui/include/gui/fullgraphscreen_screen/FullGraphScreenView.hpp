#ifndef FULLGRAPHSCREENVIEW_HPP
#define FULLGRAPHSCREENVIEW_HPP

#include <gui_generated/fullgraphscreen_screen/FullGraphScreenViewBase.hpp>
#include <gui/fullgraphscreen_screen/FullGraphScreenPresenter.hpp>

class FullGraphScreenView : public FullGraphScreenViewBase
{
public:
    FullGraphScreenView();
    virtual ~FullGraphScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // FULLGRAPHSCREENVIEW_HPP
