#ifndef FULLGRAPHSCREENPRESENTER_HPP
#define FULLGRAPHSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class FullGraphScreenView;

class FullGraphScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    FullGraphScreenPresenter(FullGraphScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~FullGraphScreenPresenter() {};

private:
    FullGraphScreenPresenter();

    FullGraphScreenView& view;
};

#endif // FULLGRAPHSCREENPRESENTER_HPP
