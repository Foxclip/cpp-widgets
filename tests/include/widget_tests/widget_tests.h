#pragma once

#include "widgets/widgets.h"
#include "test_lib/test.h"

struct GenericWidgetTest;

class WidgetTests : public test::TestModule {
public:
	glvx::Window window;
	glvx::Font textbox_font;

	WidgetTests(const std::string& name, test::TestModule* manager, const std::vector<TestNode*>& required_nodes = { });
	static std::string sfVec2fToStr(const glvx::Vector2f& vec);
	static std::string sfVec2iToStr(const glvx::Vector2i& vec);
	static std::string sfVec2uToStr(const glvx::Vector2u& vec);
	static std::string cursorTypeToStr(fw::CursorType type);
	static std::string floatRectToStr(const glvx::FloatRect& rect);
	static std::string colorToStr(const glvx::Color& color);
	static std::string anchorToStr(fw::Widget::Anchor anchor);
	static bool rectApproxCmp(const glvx::FloatRect& left, const glvx::FloatRect& right);
	static void genericWidgetTest(const GenericWidgetTest& gwt);
	static void mouseDragGesture(fw::Application& application, const glvx::Vector2f& begin_pos, const glvx::Vector2f& offset);
	enum class ResizePoint {
		TOP_LEFT,
		TOP,
		TOP_RIGHT,
		LEFT,
		RIGHT,
		BOTTOM_LEFT,
		BOTTOM,
		BOTTOM_RIGHT
	};
	static glvx::Vector2f getGrabPos(fw::WindowWidget* window, ResizePoint resize_point);
	static void resizeWindow(fw::WindowWidget* window, ResizePoint resize_point, const glvx::Vector2f offset);
	static void dragWindow(fw::Application& application, fw::WindowWidget* window, const glvx::Vector2f& offset);

protected:
	void beforeRunModule() override;
	void afterRunModule() override;

private:

};

struct GenericWidgetTest {
	GenericWidgetTest(fw::Application& application, test::Test& test);
	fw::Application& application;
	test::Test& test;
	fw::Widget* widget = nullptr;
	size_t total_widgets = 0;
	fw::Widget::WidgetType type = fw::Widget::WidgetType::None;
	std::string name;
	std::string fullname;
	bool is_visual_position_quantized = false;
	fw::WidgetVisibility visibility;
	bool is_click_through = false;
	bool is_mouse_over = false;
	fw::Widget::FocusableType focusable_type = fw::Widget::FocusableType::NONE;
	bool is_focused = false;
	bool clip_children = false;
	bool force_custom_cursor = false;
	fw::Widget* parent = nullptr;
	glvx::FloatRect local_bounds;
	glvx::FloatRect global_bounds;
	glvx::FloatRect parent_local_bounds;
	glvx::FloatRect visual_local_bounds;
	glvx::FloatRect visual_global_bounds;
	glvx::FloatRect visual_parent_local_bounds;
};

#define PRESS_MOUSE_LEFT(pos) \
    application.mouseMove(pos); \
    application.mouseLeftPress(); \
    application.advance();

#define RELEASE_MOUSE_LEFT() \
    application.mouseLeftRelease(); \
    application.advance();

#define CLICK_MOUSE(pos) \
    PRESS_MOUSE_LEFT(pos); \
    RELEASE_MOUSE_LEFT();

#define MOVE_MOUSE(pos) \
    application.mouseMove(pos); \
    application.advance();

#define PRESS_KEY(key) \
    application.keyPress(key); \
    application.advance();

#define RELEASE_KEY(key) \
    application.keyRelease(key); \
    application.advance();

#define TAP_KEY(key) \
    PRESS_KEY(key) \
    RELEASE_KEY(key)

#define ENTER_TEXT(key, code) \
    application.keyPress(key); \
    application.textEntered(code); \
    application.advance(); \
    application.keyRelease(key); \
    application.advance();

#define SELECT_ALL() \
    application.keyPress(glvx::Key::LControl); \
    application.keyPress(glvx::Key::A); \
    application.advance(); \
    application.keyRelease(glvx::Key::A); \
    application.keyRelease(glvx::Key::LControl); \
    application.advance();

#define COPY() \
    PRESS_KEY(glvx::Key::LControl); \
    ENTER_TEXT(glvx::Key::C, 'c'); \
    RELEASE_KEY(glvx::Key::LControl);

#define PASTE() \
    PRESS_KEY(glvx::Key::LControl); \
    ENTER_TEXT(glvx::Key::V, 'v'); \
    RELEASE_KEY(glvx::Key::LControl);

#define CUT() \
    PRESS_KEY(glvx::Key::LControl); \
    ENTER_TEXT(glvx::Key::X, 'x'); \
    RELEASE_KEY(glvx::Key::LControl);

#define _CHECK_SELECTION(active, text, cursor_pos, left, right) \
    T_CHECK(textbox_widget->isSelectionActive() == active); \
    T_COMPARE(textbox_widget->getSelectedText(), text); \
    T_COMPARE(textbox_widget->getCursorPos(), cursor_pos); \
    T_COMPARE(textbox_widget->getSelectionLeft(), left); \
    T_COMPARE(textbox_widget->getSelectionRight(), right); \

#define CHECK_SELECTION(active, text, cursor_pos, left, right) \
    T_WRAP_CONTAINER(_CHECK_SELECTION(active, text, cursor_pos, left, right))
