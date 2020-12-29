#pragma once

#include <streaming/GameStreamClient.hpp>
#include <streaming/MoonlightSession.hpp>
#include <borealis.hpp>

class UIStreamView: public brls::View {
public: 
    Host* host;
    PAPP_LIST app;

    bool draw_stats = false;

    UIStreamView(const Host* host, PAPP_LIST app);
    ~UIStreamView();
    
    void draw(NVGcontext* ctx, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* fcn) override;
    
    void terminate(bool close_app);
    brls::View* getDefaultFocus() override;
private:
    GLFWgamepadstate glfw_gamepad_state;
    MoonlightSession* m_session;
    bool terminated = false;
    void restart_connection();
    void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
};