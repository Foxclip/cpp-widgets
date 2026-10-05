#include "application.h"
#include <cstring>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

void enable_no_crt_dialogs() {
    _set_error_mode(_OUT_TO_STDERR);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#ifdef _WIN32
    ::SetErrorMode(::GetErrorMode() | SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS);
#endif
}

[[noreturn]] void report_and_abort(const char* type, const char* what) {
    std::cerr << "terminate called after throwing an instance of '"
            << type << "'\n";
    if (what != nullptr) {
        std::cerr << "  what():  " << what << "\n";
    }
    std::abort();
}

int main(int argc, char* argv[]) {
    bool minimized = false;
    bool screenshot = false;
    bool unattended = false;
    bool no_fps = false;
    bool debug_render = false;
    double fps_duration = 0.0;
    std::string screenshot_path;
    std::string section;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--minimized") == 0) {
            minimized = true;
        } else if (std::strcmp(argv[i], "--screenshot") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "ERROR: --screenshot requires a file path argument" << std::endl;
                return 1;
            }
            screenshot = true;
            screenshot_path = argv[i + 1];
            i++;
            minimized = true;
        } else if (std::strcmp(argv[i], "--no-crt-dialog") == 0) {
            unattended = true;
        } else if (std::strcmp(argv[i], "--no-fps") == 0) {
            no_fps = true;
        } else if (std::strcmp(argv[i], "--debug-render") == 0) {
            debug_render = true;
        } else if (std::strcmp(argv[i], "--fps") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "ERROR: --fps requires a duration (seconds) argument" << std::endl;
                return 1;
            }
            fps_duration = std::atof(argv[i + 1]);
            i++;
            minimized = true;
        } else if (std::strcmp(argv[i], "--section") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "ERROR: --section requires a section name argument" << std::endl;
                return 1;
            }
            section = argv[i + 1];
            i++;
        } else {
            std::cerr << "ERROR: unknown argument: " << argv[i] << std::endl;
            return 1;
        }
    }

    if (unattended) {
        enable_no_crt_dialogs();
    }
    try {
        sandbox::Application application(section, !no_fps, debug_render);
        application.init("Widgets sandbox", sandbox::Application::WINDOW_WIDTH, sandbox::Application::WINDOW_HEIGHT, 0, false, minimized);
        if (screenshot) {
            application.start(true);
            if (!application.saveScreenshot(screenshot_path)) {
                return 1;
            }
        } else if (fps_duration > 0.0) {
            application.start(true);
            application.reportFps(fps_duration);
        } else {
            application.start();
        }
    } catch (const std::exception& e) {
        if (unattended) {
            report_and_abort("std::exception", e.what());
        }
        throw;
    } catch (...) {
        if (unattended) {
            report_and_abort("unknown type", nullptr);
        }
        throw;
    }

    return 0;
}
