#include <borealis.hpp>
#include <functional>
#include "UIStreamView.hpp"
#pragma once

using namespace brls;

class UIIngameMenu: public brls::View {
private:
    TabFrame* tabFrame;
    UIStreamView* streamView;
public:
    UIIngameMenu(UIStreamView* streamView, std::function<void(void)> closeAction);
    ~UIIngameMenu();

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