#ifndef TESTINGSCREENVIEW_HPP
#define TESTINGSCREENVIEW_HPP

#include <gui_generated/testingscreen_screen/TestingScreenViewBase.hpp>
#include <gui/testingscreen_screen/TestingScreenPresenter.hpp>

class TestingScreenView : public TestingScreenViewBase
{
public:
    TestingScreenView();
    virtual ~TestingScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // TESTINGSCREENVIEW_HPP
