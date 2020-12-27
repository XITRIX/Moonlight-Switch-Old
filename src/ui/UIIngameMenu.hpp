#include <borealis.hpp>
#include <functional>
#pragma once

using namespace brls;

class UIIngameMenu: public brls::View {
private:
    TabFrame* tabFrame;
public:
    UIIngameMenu(std::function<void(void)> closeAction);

    std::function<void(void)> closeAction;

    View* getDefaultFocus() override;

    void show(std::function<void(void)> cb, bool animate = true, ViewAnimation animation = ViewAnimation::FADE) override;
    void hide(std::function<void(void)> cb, bool animated = true, ViewAnimation animation = ViewAnimation::FADE) override;
    
    void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, Style* style, FrameContext* ctx) override;
    void layout(NVGcontext* vg, Style* style, FontStash* stash) override;
    
    bool isTranslucent() override
    {
        return true;
    }
};