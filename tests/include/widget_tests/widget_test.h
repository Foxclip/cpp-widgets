#pragma once

#include <glvx/window.h>
#include <glvx/event.h>
#include <glvx/text.h>
#include <glvx/font.h>
#include <glvx/vertex.h>
#include <glvx/vertex_array.h>
#include <glvx/render_texture.h>
#include <glvx/render_states.h>
#include <glvx/shape.h>
#include <glvx/rectangle.h>
#include <glvx/image.h>
#include <glvx/cursor.h>
#include <glvx/keyboard.h>
#include <glvx/mouse.h>
#include "test_lib/test.h"

// set by the --minimized command-line parameter;
// when true, the test window is created minimized
extern bool minimized;

class WidgetTest : public test::TestModule {
public:
	WidgetTest(const std::string& name, test::TestModule* parent, const std::vector<TestNode*>& required_nodes = { });

protected:
	glvx::Window& getWindow();
	glvx::Font& getFont();

};
