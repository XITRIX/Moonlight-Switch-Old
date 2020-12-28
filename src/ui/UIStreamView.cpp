#include "UIStreamView.hpp"

#include <Async.hpp>
#include <Settings.hpp>
#include <UIError.hpp>
#include <cstring>
#include <streaming/MoonlightSession.hpp>

#ifdef __SWITCH__
#include "AudrenAudioRenderer.hpp"
#else
#include "DebugFileRecorderAudioRenderer.hpp"
#endif

#include "FFmpegVideoDecoder.hpp"
#include "GLVideoRenderer.hpp"
#include "InputController.hpp"
#include "UIIngameMenu.hpp"

UIStreamView::UIStreamView(const Host* host, PAPP_LIST app)
    : host((Host*)host)
    , app(app)
{
    this->terminateByPlusButton = false;
    setActionAvailable(brls::Key::PLUS, false);

    m_session = new MoonlightSession(host->address, app->id);

    m_session->set_video_decoder(new FFmpegVideoDecoder());
    m_session->set_video_renderer(new GLVideoRenderer());

#ifdef __SWITCH__
    m_session->set_audio_renderer(new AudrenAudioRenderer());
#else
    m_session->set_audio_renderer(new DebugFileRecorderAudioRenderer(false));
#endif

    m_session->start([this](auto result) {
        // if (m_loader) {
        //     m_loader->dispose();
        //     m_loader = NULL;
        // }

        if (result.isSuccess())
        {
            //
        }
        else
        {
            showError(result.error(), [this]() {
                this->terminate(false);
            });
        }
    });
}

UIStreamView::~UIStreamView()
{
    if (!terminated) { terminate(false); }
    brls::Application::notify("Destroy session");
    delete m_session;
}

brls::View* UIStreamView::getDefaultFocus()
{
    return this;
}

int dm_width = 0, dm_height = 0;
double dx = 0, dy = 0;
void UIStreamView::draw(NVGcontext* ctx, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* fcn)
{
    if (terminated)
        return;

    if (!m_session->is_active())
    {
        brls::Application::notify("Terminate");
        terminate(false);
        terminated = true;
        return;
    }

    nvgSave(ctx);

    m_session->draw();

    nvgRestore(ctx);

    if (m_session->connection_status_is_poor())
    {
        nvgFontSize(ctx, 20);
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

        nvgFontBlur(ctx, 3);
        nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
        // nvgFontFace(ctx, "icons");
        // nvgText(ctx, 20, height - 30, utf8(FA_EXCLAMATION_TRIANGLE).data(), NULL);
        nvgFontFaceId(ctx, fcn->fontStash->regular);
        nvgText(ctx, 50, height - 28, "Bad connection...", NULL);

        nvgFontBlur(ctx, 0);
        nvgFillColor(ctx, nvgRGBA(255, 255, 255, 255));
        // nvgFontFace(ctx, "icons");
        // nvgText(ctx, 20, height - 30, utf8(FA_EXCLAMATION_TRIANGLE).data(), NULL);
        nvgFontFaceId(ctx, fcn->fontStash->regular);
        nvgText(ctx, 50, height - 28, "Bad connection...", NULL);
    }

    if (draw_stats)
    {
        static char output[1024];

        int offset = 0;

        auto stats = m_session->session_stats();

        offset += sprintf(&output[offset],
            "Estimated host PC frame rate: %.2f FPS\n"
            "Incoming frame rate from network: %.2f FPS\n"
            "Decoding frame rate: %.2f FPS\n"
            "Rendering frame rate: %.2f FPS\n",
            stats->video_decode_stats.total_fps,
            stats->video_decode_stats.received_fps,
            stats->video_decode_stats.decoded_fps,
            stats->video_render_stats.rendered_fps);

        offset += sprintf(&output[offset],
            "Frames dropped by your network connection: %.2f%% (Total: %u)\n"
            "Average receive time: %.2f ms\n"
            "Average decoding time: %.2f ms\n"
            "Average rendering time: %.2f ms\n",
            (float)stats->video_decode_stats.network_dropped_frames / stats->video_decode_stats.total_frames * 100,
            stats->video_decode_stats.network_dropped_frames,
            (float)stats->video_decode_stats.total_reassembly_time / stats->video_decode_stats.received_frames,
            (float)stats->video_decode_stats.total_decode_time / stats->video_decode_stats.decoded_frames,
            (float)stats->video_render_stats.total_render_time / stats->video_render_stats.rendered_frames);

        nvgFontFaceId(ctx, fcn->fontStash->regular);
        nvgFontSize(ctx, 20);
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_BOTTOM);

        nvgFontBlur(ctx, 1);
        nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
        nvgTextBox(ctx, 20, 30, width, output, NULL);

        nvgFontBlur(ctx, 0);
        nvgFillColor(ctx, nvgRGBA(0, 255, 0, 255));
        nvgTextBox(ctx, 20, 30, width, output, NULL);
    }

    // Mouse
    glfwSetCursorPosCallback(brls::Application::window, [](GLFWwindow* w, double x, double y) {
        int m_width, m_height;
        glfwGetWindowSize(w, &m_width, &m_height);
        x = x * 1920 / m_width;
        y = y * 1080 / m_height;
        InputController::controller()->handle_cursor_event(1920, 1080, x, y);
    });

    if (Settings::settings()->click_by_tap())
    {
        glfwSetMouseButtonCallback(brls::Application::window, [](GLFWwindow* w, int button, int action, int modifiers) {
            InputController::controller()->handle_mouse_event(button, action, modifiers);
        });
    }

    // Gamepad
    glfwGetGamepadState(GLFW_JOYSTICK_1, &glfw_gamepad_state);
    InputController::controller()->handle_gamepad_event(glfw_gamepad_state);

    static bool menuOverlay = false;
    InputController::controller()->menu_key_combo([this]() {
        if (!menuOverlay)
        {
            menuOverlay = true;
            auto menu   = new UIIngameMenu(this, []() {
                Async::run([](){
                    usleep(100000);
                    menuOverlay = false;
                });
            });
            brls::Application::pushView(menu);
        }
    });

    if (!menuOverlay)
        InputController::controller()->send_to_stream();
}

void UIStreamView::terminate(bool close_app)
{
    terminated = true;
    InputController::controller()->stop_rumple();

    // if (m_loader) {
    //     m_loader->dispose();
    //     m_loader = NULL;
    // }

    m_session->stop(close_app);

    brls::Application::popView();
}