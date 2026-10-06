#include "widget_tests/widget_test.h"
#include "widget_tests/widget_tests.h"

bool minimized = false;

WidgetTest::WidgetTest(const std::string& name, test::TestModule* parent, const std::vector<TestNode*>& required_nodes) : TestModule(name, parent, required_nodes) { }

glvx::Window& WidgetTest::getWindow() {
	return dynamic_cast<WidgetTests*>(parent)->window;
}

glvx::Font& WidgetTest::getFont() {
	return dynamic_cast<WidgetTests*>(parent)->textbox_font;
}
