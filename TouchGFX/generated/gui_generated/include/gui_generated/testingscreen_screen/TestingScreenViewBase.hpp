/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef TESTINGSCREENVIEWBASE_HPP
#define TESTINGSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/testingscreen_screen/TestingScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TiledImage.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <gui/containers/CustomScrollableContainer.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <gui/containers/NavigationMenu.hpp>

class TestingScreenViewBase : public touchgfx::View<TestingScreenPresenter>
{
public:
    TestingScreenViewBase();
    virtual ~TestingScreenViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void startTesting()
    {
        // Override and implement this function in TestingScreen
    }

    virtual void pauseTesting()
    {
        // Override and implement this function in TestingScreen
    }

    virtual void stopTesting()
    {
        // Override and implement this function in TestingScreen
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::TiledImage backgroundTiledImage;
    touchgfx::TiledImage tiledImage1;
    touchgfx::ButtonWithLabel stopTestingButton;
    touchgfx::ButtonWithLabel pauseTestingButton;
    touchgfx::ButtonWithLabel runTestingButton;
    touchgfx::CustomScrollableContainer textOutputScrollableContainer;
    touchgfx::TextAreaWithOneWildcard textOutput;
    NavigationMenu navigationMenu1;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TEXTOUTPUT_SIZE = 5000;
    touchgfx::Unicode::UnicodeChar textOutputBuffer[TEXTOUTPUT_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<TestingScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // TESTINGSCREENVIEWBASE_HPP
