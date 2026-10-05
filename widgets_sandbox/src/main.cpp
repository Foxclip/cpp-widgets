#include "application.h"
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    bool minimized = false;
    bool screenshot = false;
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

    return 0;
}
