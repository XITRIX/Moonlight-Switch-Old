#pragma once

#include <streaming/MoonlightSession.hpp>
#include <borealis.hpp>

class UIStreamView: public brls::View {
public: 
    UIStreamView(const std::string &address, int app_id);
    ~UIStreamView();
    
    void draw(NVGcontext* ctx, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* fcn) override;
    
    void terminate(bool close_app);
    brls::View* getDefaultFocus() override;
private:
    GLFWgamepadstate glfw_gamepad_state;
    MoonlightSession* m_session;
    bool m_draw_stats = false;
    bool terminated = false;
    void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
};