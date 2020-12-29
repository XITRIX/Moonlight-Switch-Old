#include "UIIngameMenu.hpp"

#include <Async.hpp>
#include <InputController.hpp>
#include <string>
#include <unistd.h>

#define RIGHT_OFFSET 280

UIIngameMenu::UIIngameMenu(UIStreamView* streamView, std::function<void(void)> closeAction)
    : View()
    , streamView(streamView)
    , closeAction(closeAction)
{
    registerAction("Close", Key::PLUS, []() { Application::popView(); return true; });
    setActionAvailable(brls::Key::PLUS, false);

    //Create sidebar
    this->tabFrame = new TabFrame();
    this->tabFrame->setTitle(streamView->app->name);

    // Control tab
    auto controlTab = new List();
    auto mouseItem  = new ToggleListItem("Enable mouse", InputController::controller()->mouse_mode_enabled, "Touchscreen - like a touchpad\nZR - Left click, ZL - Right click");
    auto statsItem  = new ToggleListItem("Show stats", streamView->draw_stats);

    controlTab->addView(statsItem);
    controlTab->addView(mouseItem);

    mouseItem->getClickEvent()->subscribe([](auto view) {
        InputController::controller()->mouse_mode_enabled = !InputController::controller()->mouse_mode_enabled;
    });
    statsItem->getClickEvent()->subscribe([streamView](auto view) {
        streamView->draw_stats = !streamView->draw_stats;
    });

    this->tabFrame->addTab("Control", controlTab);

    // Special keys tab

    auto keysTab    = new List();
    auto escapeItem = new ListItem("Escape");
    auto altEnterItem  = new ListItem("Alt + Enter");
    auto altTabItem  = new ListItem("Alt + Tab");
    auto winItem  = new ListItem("Win");

    keysTab->addView(winItem);
    keysTab->addView(new ListItemGroupSpacing());
    keysTab->addView(altEnterItem);
    keysTab->addView(altTabItem);
    keysTab->addView(new ListItemGroupSpacing());
    keysTab->addView(escapeItem);

    escapeItem->getClickEvent()->subscribe([](auto view) {
        Async::run([]() {
            LiSendKeyboardEvent(0x1B, KEY_ACTION_DOWN, 0);
            usleep(500'000);
            LiSendKeyboardEvent(0x1B, KEY_ACTION_UP, 0);
        });
    });
    altEnterItem->getClickEvent()->subscribe([streamView](auto view) {
        Async::run([]() {
            LiSendKeyboardEvent(0x0D, KEY_ACTION_DOWN, MODIFIER_ALT);
            usleep(500'000);
            LiSendKeyboardEvent(0x0D, KEY_ACTION_UP, MODIFIER_ALT);
        });
    });
    altTabItem->getClickEvent()->subscribe([streamView](auto view) {
        Async::run([]() {
            LiSendKeyboardEvent(0x09, KEY_ACTION_DOWN, MODIFIER_ALT);
            usleep(500'000);
            LiSendKeyboardEvent(0x09, KEY_ACTION_UP, MODIFIER_ALT);
        });
    });
    winItem->getClickEvent()->subscribe([streamView](auto view) {
        Async::run([]() {
            LiSendKeyboardEvent(0x5B, KEY_ACTION_DOWN, 0);
            usleep(500'000);
            LiSendKeyboardEvent(0x5B, KEY_ACTION_UP, 0);
        });
    });

    this->tabFrame->addTab("Keys", keysTab);

    // Quit tab
    auto quitTab       = new List();
    auto closeItem     = new ListItem("Close", "Disconnect from host");
    auto terminateItem = new ListItem("Terminate", "Kills the game process on host\nYou could lose your progress");

    quitTab->addView(terminateItem);
    quitTab->addView(closeItem);

    closeItem->getClickEvent()->subscribe([streamView](auto view) {
        brls::Application::popView(brls::ViewAnimation::FADE, [streamView]() {
            streamView->terminate(false);
        });
    });
    terminateItem->getClickEvent()->subscribe([streamView](auto view) {
        brls::Application::popView(brls::ViewAnimation::FADE, [streamView]() {
            streamView->terminate(true);
        });
    });

    this->tabFrame->addSeparator();
    this->tabFrame->addTab("Quit", quitTab);

    alpha = 0.2f;
}

UIIngameMenu::~UIIngameMenu()
{
    delete this->tabFrame;
}

void UIIngameMenu::show(std::function<void(void)> cb, bool animate, ViewAnimation animation)
{
    tabFrame->show(cb, animate, ViewAnimation::SLIDE_RIGHT);
    View::show(cb, animate, animation);
}

void UIIngameMenu::hide(std::function<void(void)> cb, bool animated, ViewAnimation animation)
{
    tabFrame->hide(cb, animated, ViewAnimation::SLIDE_LEFT);
    View::hide(cb, animated, animation);
    closeAction();
}

void UIIngameMenu::draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, Style* style, FrameContext* ctx)
{
    // Backdrop
    nvgFillColor(vg, a(ctx->theme->dropdownBackgroundColor));
    nvgBeginPath(vg);
    nvgRect(vg, x, y, width, height);
    nvgFill(vg);

    // Background
    nvgFillColor(vg, a(ctx->theme->backgroundColorRGB));
    nvgBeginPath(vg);
    nvgRect(vg, x, y, width - RIGHT_OFFSET, height);
    nvgFill(vg);

    // TabFrame
    this->tabFrame->frame(ctx);
}

void UIIngameMenu::layout(NVGcontext* vg, Style* style, FontStash* stash)
{
    // TabFrame
    if (this->tabFrame)
    {
        this->tabFrame->setBoundaries(this->x, this->y, this->width - RIGHT_OFFSET, this->height);
        this->tabFrame->invalidate();
    }
}

View* UIIngameMenu::getDefaultFocus()
{
    return this->tabFrame->getDefaultFocus();
}
