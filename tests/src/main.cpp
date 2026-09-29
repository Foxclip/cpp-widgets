#include "widget_tests/widget_tests.h"
#include "logger/logger.h"

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

void run_tests() {
    test::TestModule root_module("Widget tests", nullptr);
    WidgetTests* widget_module = root_module.addModule<WidgetTests>("Widget", { });
    root_module.run();
    root_module.printSummary();
}

int main(int argc, char* argv[]) {
    const bool unattended =
        argc > 1 && std::strcmp(argv[1], "--no-crt-dialog") == 0;
    if (unattended) {
        enable_no_crt_dialogs();
    }
    try {
        run_tests();
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
