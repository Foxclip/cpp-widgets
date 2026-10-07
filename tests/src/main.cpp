#include <cstring>
#include "widget_tests/widget_test.h"
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

void run_tests(const std::string& test_path) {
    test::TestModule root_module("Widget tests", nullptr);
    WidgetTests* widget_module = root_module.addModule<WidgetTests>("Widget", { });
    if (test_path.empty()) {
        root_module.run();
    } else {
        root_module.run(test_path);
    }
    root_module.printSummary();
}

int main(int argc, char* argv[]) {
    bool unattended = false;
    std::string test_path;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--no-crt-dialog") == 0) {
            unattended = true;
        } else if (std::strcmp(argv[i], "--minimized") == 0) {
            minimized = true;
        } else if (test_path.empty() && argv[i][0] != '-') {
            test_path = argv[i];
        }
    }
    if (unattended) {
        enable_no_crt_dialogs();
    }
    try {
        run_tests(test_path);
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

// TODO: TreeView: fix invisible text on the TreeViewEntry drag ghost