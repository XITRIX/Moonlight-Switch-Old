#include "UIStreamView.hpp"

#include <Async.hpp>
#include <Settings.hpp>
#include <UIError.hpp>
#include <cstring>
#include <streaming/MoonlightSession.hpp>

#include "AudrenAudioRenderer.hpp"
#include "FFmpegVideoDecoder.hpp"
#include "GLVideoRenderer.hpp"
#include "InputController.hpp"

UIStreamView::UIStreamView(const std::string& address, int app_id)
{
    this->terminateByPlusButton = false;
    registerAction("", brls::Key::MINUS, [this] { this->terminate(false); return true; });

    m_session = new MoonlightSession(address, app_id);

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
            terminate(false);
            showError(result.error());
        }
    });
}

UIStreamView::~UIStreamView()
{
    brls::Application::notify("Destroy session");
    delete m_session;
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
        Async::instance()->run([this]() {
            this->terminate(false);
        });
        terminated = true;
        return;
    }

    nvgSave(ctx);

    m_session->draw();

    nvgRestore(ctx);

    // nvgFontSize(ctx, 20);
    // nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

    // nvgFontBlur(ctx, 3);
    // nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
    // // nvgFontFace(ctx, "icons");
    // // nvgText(ctx, 20, height - 30, utf8(FA_EXCLAMATION_TRIANGLE).data(), NULL);
    // nvgFontFaceId(ctx, fcn->fontStash->regular);
    // auto text = ("x: " + std::to_string(dx) + ", y: " + std::to_string(dy) + ", width: " + std::to_string(dm_width) + ", height: " + std::to_string(dm_height)).c_str();
    // nvgText(ctx, 50, height - 50, text, NULL);

    // nvgFontBlur(ctx, 0);
    // nvgFillColor(ctx, nvgRGBA(255, 255, 255, 255));
    // // nvgFontFace(ctx, "icons");
    // // nvgText(ctx, 20, height - 30, utf8(FA_EXCLAMATION_TRIANGLE).data(), NULL);
    // nvgFontFaceId(ctx, fcn->fontStash->regular);
    // nvgText(ctx, 50, height - 50, text, NULL);

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

    if (m_draw_stats)
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
        // auto config = m_active_session->config();
        x = x * 1920 / m_width;
        y = y * 1080 / m_height;
        InputController::controller()->handle_cursor_event(1920, 1080, x, y);
        // dm_width  = m_width;
        // dm_height = m_height;
        // dx        = x;
        // dy        = y;
    });

    if (Settings::settings()->click_by_tap())
    {
        glfwSetMouseButtonCallback(brls::Application::window, [](GLFWwindow* w, int button, int action, int modifiers) {
            InputController::controller()->handle_mouse_event(button, action, modifiers);
        });
    }
    else
    {
        
    }

    // Gamepad
    glfwGetGamepadState(GLFW_JOYSTICK_1, &glfw_gamepad_state);
    InputController::controller()->handle_gamepad_event(glfw_gamepad_state);

    // TODO: Get out of here...
    // if (InputController::controller()->gamepad_combo_is_enabled(GamepadComboExitAndClose))
    // {
    //     this->terminate(true);
    // }
    // else if (InputController::controller()->gamepad_combo_is_enabled(GamepadComboExit))
    // {
    //     this->terminate(false);
    // }

    // if (!m_draw_stats && InputController::controller()->gamepad_combo_is_enabled(GamepadComboShowStats))
    // {
    //     m_draw_stats = true;
    // }
    // else if (m_draw_stats && InputController::controller()->gamepad_combo_is_enabled(GamepadComboHideStats))
    // {
    //     m_draw_stats = false;
    // }
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