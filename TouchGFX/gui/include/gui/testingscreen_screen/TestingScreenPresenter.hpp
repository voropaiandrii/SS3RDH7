#ifndef TESTINGSCREENPRESENTER_HPP
#define TESTINGSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <gui/common/CustomButtons.hpp>

using namespace touchgfx;

class TestingScreenView;

class TestingScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    TestingScreenPresenter(TestingScreenView& v);

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

    virtual ~TestingScreenPresenter() {};

    void startTesting();
    void pauseTesting();
    void stopTesting();

    void updateTestingButtonsState(uint8_t testingState);

    void printTestingOutput(const char* string);
    void invalidateTestingScreen();

private:
    TestingScreenPresenter();

    TestingScreenView& view;
};

#endif // TESTINGSCREENPRESENTER_HPP
