#include "widget_tests/widget_tests.h"
#include "logger/logger.h"

void run_tests() {
    test::TestModule root_module("Widget tests", nullptr);
    WidgetTests* widget_module = root_module.addModule<WidgetTests>("Widget", { });
    root_module.run();
    root_module.printSummary();
}

int main() {
    run_tests();
    return 0;
}
