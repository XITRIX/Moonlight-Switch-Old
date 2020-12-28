#include "UIIngameMenu.hpp"
#include <InputController.hpp>

#include <string>

#define RIGHT_OFFSET 280

UIIngameMenu::UIIngameMenu(UIStreamView* streamView, std::function<void(void)> closeAction)
    : View(), streamView(streamView), closeAction(closeAction)
{
    registerAction("Close", Key::PLUS, []() { Application::popView(); return true; });
    setActionAvailable(brls::Key::PLUS, false);

    //Create sidebar
    this->tabFrame = new TabFrame();
    this->tabFrame->setTitle(streamView->app->name);

    // Control tab
    auto controlTab = new List();
    auto mouseItem = new ToggleListItem("Enable mouse", InputController::controller()->mouse_mode_enabled, "Touchscreen - like a touchpad\nZR - Left click, ZL - Right click");
    auto statsItem = new ToggleListItem("Show stats", streamView->draw_stats);

    controlTab->addView(statsItem);
    controlTab->addView(mouseItem);

    mouseItem->getClickEvent()->subscribe([](auto view) {
        InputController::controller()->mouse_mode_enabled = !InputController::controller()->mouse_mode_enabled;
    });
    statsItem->getClickEvent()->subscribe([streamView](auto view) {
        streamView->draw_stats = !streamView->draw_stats;
    });

    this->tabFrame->addTab("Control", controlTab);

    // Quit tab
    auto quitTab = new List();
    auto closeItem = new ListItem("Close");
    auto terminateItem = new ListItem("Terminate");

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

    alpha = 0.6f;
}

UIIngameMenu::~UIIngameMenu() {
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