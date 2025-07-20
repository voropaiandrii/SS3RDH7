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

    void startTesting();
    void pauseTesting();
    void stopTesting();

    void changeButtonState(ButtonID buttonId, ButtonState state);

    void printTestingOutput(const char* string);
    void invalidateTestingScreen();
protected:
};

#endif // TESTINGSCREENVIEW_HPP
