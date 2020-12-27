#include "UIIngameMenu.hpp"

#include <string>

#define RIGHT_OFFSET 280

UIIngameMenu::UIIngameMenu(std::function<void(void)> closeAction)
    : View(), closeAction(closeAction)
{
    registerAction("Close", Key::PLUS, []() { Application::popView(); return true; });
    setActionAvailable(brls::Key::PLUS, false);

    //Create sidebar
    this->tabFrame = new TabFrame();
    this->tabFrame->setTitle("Test Menu");

    auto list = new List();
    list->addView(new ListItem("Test item"));
    this->tabFrame->addTab("Test tab", list);

    alpha = 0.6f;
}

void UIIngameMenu::show(std::function<void(void)> cb, bool animate, ViewAnimation animation) 
{
    tabFrame->show(cb, animate, ViewAnimation::SLIDE_RIGHT);
    View::show(cb, animate, animation);
}

void UIIngameMenu::hide(std::function<void(void)> cb, bool animated, ViewAnimation animation)
{
    closeAction();
    tabFrame->hide(cb, animated, ViewAnimation::SLIDE_LEFT);
    View::hide(cb, animated, animation);
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