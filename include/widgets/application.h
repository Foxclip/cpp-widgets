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
#include "widgets/widget_list.h"
#include <queue>

namespace fw {

	struct MouseGesture {
		enum MouseGestureType {
			NORMAL,
			MOVE,
		};
		bool active = false;
		Widget* source = nullptr;
		MouseGestureType type;
		glvx::Vector2f startPos;
		glvx::Mouse::Button button;
		MouseGesture();
		MouseGesture(
			Widget* source,
			MouseGestureType type,
			glvx::Vector2f startPos,
			glvx::Mouse::Button button
		);
	};

	enum class Stage {
		NONE,
		PROCESS,
		BEFORE_INPUT,
		INPUT,
		AFTER_INPUT,
		WORLD,
		UPDATE,
		RENDER,
	};

	class Application {
	public:
		Application();
		Application(glvx::Window& window);
		virtual void init(
			const std::string& window_title,
			unsigned int window_width,
			unsigned int window_height,
			unsigned int antialiasing,
			bool vsync,
			bool minimized = false
		);
		virtual void start(bool external_control = false);
		void advance();
		void maximizeWindow() const;
		glvx::Vector2u getWindowSize() const;
		const fw::Font& getDefaultFont() const;
		Stage getStage() const;
		Widget* getLeftGestureSource() const;
		Widget* getRightGestureSource() const;
		const glvx::RenderTexture& getRenderTexture() const;
		glvx::Image getRenderedImage() const;
		void setWindowSize(unsigned int width, unsigned int height);
		void setWindowSize(const glvx::Vector2u& size);
		void addExternalEvent(const glvx::Event& event);
		void mouseMove(int x, int y);
		void mouseMove(unsigned int x, unsigned int y);
		void mouseMove(float x, float y);
		void mouseMove(const glvx::Vector2i& pos);
		void mouseMove(const glvx::Vector2f& pos);
		void mouseLeftPress();
		void mouseRightPress();
		void mouseLeftRelease();
		void mouseRightRelease();
		void mouseLeftClick();
		void mouseRightClick();
		void mouseLeftClick(const glvx::Vector2f& pos);
		void mouseRightClick(const glvx::Vector2f& pos);
		void mouseScrollX(float delta);
		void mouseScrollY(float delta);
		void keyPress(glvx::Key key);
		void keyRelease(glvx::Key key);
		void textEntered(uint32_t code);
		bool isLeftButtonPressed() const;
		bool isRightButtonPressed() const;
		bool isMiddleButtonPressed() const;
		bool isLCtrlPressed() const;
		bool isLAltPressed() const;
		bool isLShiftPressed() const;
		glvx::Vector2i getMousePos() const;
		glvx::Vector2f getMousePosf() const;
		const glvx::Vector2f& getMousePressPosf() const;
		glvx::Vector2f getWindowCenter() const;
		WidgetList& getWidgets();
		void setBackgroundColor(const glvx::Color& color);
		void setDefaultFont(const fw::Font& font);
		void setVerticalSyncEnabled(bool value);
		void close();

	protected:
		glvx::Window& window;
		glvx::Window internal_window;
		bool external_window = false;
		bool running = true;
		glvx::View window_view;
		WidgetList widgets = WidgetList(*this);
		glvx::Vector2i internal_mouse_pos;
		glvx::Vector2i mousePrevPos;
		glvx::Vector2f mousePrevPosf;
		glvx::Vector2f mousePressPosf;
		bool leftButtonPressed = false;
		bool rightButtonPressed = false;
		bool middleButtonPressed = false;
		MouseGesture mouse_gesture_left;
		MouseGesture mouse_gesture_right;
		glvx::Cursor arrow_cursor;
		glvx::Cursor text_cursor;
		glvx::Cursor size_top_left_cursor;
		glvx::Cursor size_top_cursor;
		glvx::Cursor size_top_right_cursor;
		glvx::Cursor size_left_cursor;
		glvx::Cursor size_right_cursor;
		glvx::Cursor size_bottom_left_cursor;
		glvx::Cursor size_bottom_cursor;
		glvx::Cursor size_bottom_right_cursor;
		bool external_control = false;
		glvx::Vector2i external_mouse_pos;
		bool external_lctrl_pressed = false;
		bool external_lalt_pressed = false;
		bool external_lshift_pressed = false;
		std::queue<glvx::Event> external_event_queue;
		// parallel axis hints (0 = X, 1 = Y) for simulated MouseWheelScrolled events
		std::queue<int> external_wheel_axis_queue;
		glvx::Color background_color = glvx::Color::Black;
		CursorType current_cursor_type = CursorType::Arrow;

		virtual void onInit();
		virtual void onStart();
		virtual void onFrameBegin();
		virtual void onFrameEnd();
		virtual void onProcessWidgets();
		virtual void onProcessWindowEvent(const glvx::Event& event);
		virtual void onProcessKeyboardEvent(const glvx::Event& event);
		virtual void onProcessLeftPress();
		virtual void onProcessRightPress();
		virtual void onProcessLeftRelease();
		virtual void onProcessRightRelease();
		virtual void onProcessMouseMove();
		virtual void onProcessMouseScrollX(float delta);
		virtual void onProcessMouseScrollY(float delta);
		virtual void onProcessKeyboard();
		virtual void onProcessMouse();
		virtual void onBeforeProcessInput();
		virtual void onAfterProcessInput();
		virtual void onProcessWorld();
		virtual void onRender();
		virtual void onClose();
		void startMoveGesture(Widget* source);
		void endGestureLeft();
		void endGestureRight();

	private:
		friend class Widget;
		friend class CanvasWidget;
		Stage stage = Stage::NONE;
		glvx::RenderTexture render_texture;

		void mainLoop();
		void processWidgets();
		void processBeforeInput();
		void processInput();
		void processAfterInput();
		void processEvent(const glvx::Event& event);
		void processWindowEvent(const glvx::Event& event);
		void processKeyboardEvent(const glvx::Event& event);
		void processMouseEvent(const glvx::Event& event);
		void startNormalGesture(Widget* source, glvx::Mouse::Button button);
		void processLeftPress();
		void processRightPress();
		void processMiddlePress();
		void processLeftRelease();
		void processRightRelease();
		void processMiddleRelease();
		void processMouseMove();
		void processScrollX(float delta);
		void processScrollY(float delta);
		void processKeyboard();
		void processMouse();
		void processWorld();
		void render();
		void setCursorType(CursorType type);

	};

}
