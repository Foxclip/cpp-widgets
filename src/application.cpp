#define GLFW_EXPOSE_NATIVE_WIN32
#include "widgets/application.h"
#include <GLFW/glfw3native.h>
#include <Windows.h>
#include <winuser.h>

namespace fw {

    Application::Application() : window(internal_window) {
        arrow_cursor.loadFromSystem(glvx::Cursor::Type::Arrow);
        text_cursor.loadFromSystem(glvx::Cursor::Type::Text);
        size_top_left_cursor.loadFromSystem(glvx::Cursor::Type::SizeTopLeftBottomRight);
        size_top_cursor.loadFromSystem(glvx::Cursor::Type::SizeVertical);
        size_top_right_cursor.loadFromSystem(glvx::Cursor::Type::SizeBottomLeftTopRight);
        size_left_cursor.loadFromSystem(glvx::Cursor::Type::SizeHorizontal);
        size_right_cursor.loadFromSystem(glvx::Cursor::Type::SizeHorizontal);
        size_bottom_left_cursor.loadFromSystem(glvx::Cursor::Type::SizeBottomLeftTopRight);
        size_bottom_cursor.loadFromSystem(glvx::Cursor::Type::SizeVertical);
        size_bottom_right_cursor.loadFromSystem(glvx::Cursor::Type::SizeTopLeftBottomRight);
    }

    Application::Application(glvx::Window& window) : window(window) {
        arrow_cursor.loadFromSystem(glvx::Cursor::Type::Arrow);
        text_cursor.loadFromSystem(glvx::Cursor::Type::Text);
        size_top_left_cursor.loadFromSystem(glvx::Cursor::Type::SizeTopLeftBottomRight);
        size_top_cursor.loadFromSystem(glvx::Cursor::Type::SizeVertical);
        size_top_right_cursor.loadFromSystem(glvx::Cursor::Type::SizeBottomLeftTopRight);
        size_left_cursor.loadFromSystem(glvx::Cursor::Type::SizeHorizontal);
        size_right_cursor.loadFromSystem(glvx::Cursor::Type::SizeHorizontal);
        size_bottom_left_cursor.loadFromSystem(glvx::Cursor::Type::SizeBottomLeftTopRight);
        size_bottom_cursor.loadFromSystem(glvx::Cursor::Type::SizeVertical);
        size_bottom_right_cursor.loadFromSystem(glvx::Cursor::Type::SizeTopLeftBottomRight);
        external_window = true;
    }

    void Application::init(
        const std::string& window_title,
        unsigned int window_width,
        unsigned int window_height,
        unsigned int antialiasing,
        bool vsync
    ) {
        if (external_window) {
            window.setSize((int)window_width, (int)window_height);
            window.setTitle(window_title);
        } else {
            window.create((int)window_width, (int)window_height, window_title.c_str(), (int)antialiasing);
        }
        window.setVerticalSyncEnabled(vsync);
        onInit();
    }

    void Application::start(bool external_control) {
        this->external_control = external_control;
        running = true;
        onStart();
        if (!external_control) {
            mainLoop();
        }
    }

    void Application::advance() {
        stage = Stage::NONE;
        onFrameBegin();
        stage = Stage::PROCESS;
        processWidgets();
        stage = Stage::BEFORE_INPUT;
        processBeforeInput();
        stage = Stage::INPUT;
        processInput();
        stage = Stage::AFTER_INPUT;
        processAfterInput();
        stage = Stage::WORLD;
        processWorld();
        stage = Stage::UPDATE;
        widgets.updateWidgets();
        stage = Stage::RENDER;
        render();
        stage = Stage::NONE;
        onFrameEnd();
    }

    void Application::maximizeWindow() const {
        HWND windowHandle = glfwGetWin32Window(window.getWindowHandle());
        ShowWindow(windowHandle, SW_MAXIMIZE);
    }

    glvx::Vector2u Application::getWindowSize() const {
        return glvx::Vector2u(window.getSize());
    }

    const fw::Font& Application::getDefaultFont() const {
        return widgets.getDefaultFont();
    }

    Stage Application::getStage() const {
        return stage;
    }

    Widget* Application::getLeftGestureSource() const {
        if (mouse_gesture_left.active) {
            return mouse_gesture_left.source;
        } else {
            return nullptr;
        }
    }

    Widget* Application::getRightGestureSource() const {
        if (mouse_gesture_right.active) {
            return mouse_gesture_right.source;
        } else {
            return nullptr;
        }
    }

    const glvx::RenderTexture& Application::getRenderTexture() const {
        return render_texture;
    }

    glvx::Image Application::getRenderedImage() const {
        return render_texture.readPixels();
    }

    void Application::setWindowSize(unsigned int width, unsigned int height) {
        window.setSize((int)width, (int)height);
        if (external_control) {
            glvx::Event event;
            event.type = glvx::EventType::Resized;
            event.size.width = width;
            event.size.height = height;
            addExternalEvent(event);
        }
    }

    void Application::setWindowSize(const glvx::Vector2u& size) {
        window.setSize((int)size.x, (int)size.y);
    }

    void Application::addExternalEvent(const glvx::Event& event) {
        wAssert(external_control);
        external_event_queue.push(event);
    }

    void Application::mouseMove(int x, int y) {
        wAssert(external_control);
        external_mouse_pos = glvx::Vector2i(x, y);
        glvx::Event event;
        event.type = glvx::EventType::MouseMoved;
        event.mouseMove.x = x;
        event.mouseMove.y = y;
        addExternalEvent(event);
    }

    void Application::mouseMove(unsigned int x, unsigned int y) {
		mouseMove((int)x, (int)y);
    }

    void Application::mouseMove(float x, float y) {
        mouseMove((int)x, (int)y);
    }

    void Application::mouseMove(const glvx::Vector2i& pos) {
        mouseMove(pos.x, pos.y);
    }

    void Application::mouseMove(const glvx::Vector2f& pos) {
        mouseMove(pos.x, pos.y);
    }

    void Application::mouseLeftPress() {
        wAssert(external_control);
        glvx::Event event;
        event.type = glvx::EventType::MouseButtonPressed;
        event.mouseButton.button = glvx::Mouse::Button::Left;
        event.mouseButton.x = external_mouse_pos.x;
        event.mouseButton.y = external_mouse_pos.y;
        addExternalEvent(event);
    }

    void Application::mouseRightPress() {
        wAssert(external_control);
        glvx::Event event;
        event.type = glvx::EventType::MouseButtonPressed;
        event.mouseButton.button = glvx::Mouse::Button::Right;
        event.mouseButton.x = external_mouse_pos.x;
        event.mouseButton.y = external_mouse_pos.y;
        addExternalEvent(event);
    }

    void Application::mouseLeftRelease() {
        wAssert(external_control);
        glvx::Event event;
        event.type = glvx::EventType::MouseButtonReleased;
        event.mouseButton.button = glvx::Mouse::Button::Left;
        event.mouseButton.x = external_mouse_pos.x;
        event.mouseButton.y = external_mouse_pos.y;
        addExternalEvent(event);
    }

    void Application::mouseRightRelease() {
        wAssert(external_control);
        glvx::Event event;
        event.type = glvx::EventType::MouseButtonReleased;
        event.mouseButton.button = glvx::Mouse::Button::Right;
        event.mouseButton.x = external_mouse_pos.x;
        event.mouseButton.y = external_mouse_pos.y;
        addExternalEvent(event);
    }

    void Application::mouseLeftClick() {
        mouseLeftPress();
        advance();
        mouseLeftRelease();
        advance();
    }

    void Application::mouseRightClick() {
        mouseRightPress();
        advance();
        mouseRightRelease();
        advance();
    }

    void Application::mouseLeftClick(const glvx::Vector2f& pos) {
        mouseMove(pos);
        mouseLeftClick();
    }

    void Application::mouseRightClick(const glvx::Vector2f& pos) {
        mouseMove(pos);
        mouseRightClick();
    }

    void Application::mouseScrollX(float delta) {
        wAssert(external_control);
        glvx::Event event;
        event.type = glvx::EventType::MouseWheelScrolled;
        event.mouseWheel.delta = delta;
        event.mouseWheel.x = external_mouse_pos.x;
        event.mouseWheel.y = external_mouse_pos.y;
        addExternalEvent(event);
        external_wheel_axis_queue.push(0);
    }

    void Application::mouseScrollY(float delta) {
        wAssert(external_control);
        glvx::Event event;
        event.type = glvx::EventType::MouseWheelScrolled;
        event.mouseWheel.delta = delta;
        event.mouseWheel.x = external_mouse_pos.x;
        event.mouseWheel.y = external_mouse_pos.y;
        addExternalEvent(event);
        external_wheel_axis_queue.push(1);
    }

    void Application::keyPress(glvx::Key key) {
        wAssert(external_control);
        if (key == glvx::Key::LControl) {
            external_lctrl_pressed = true;
        } else if (key == glvx::Key::LAlt) {
            external_lalt_pressed = true;
        } else if (key == glvx::Key::LShift) {
            external_lshift_pressed = true;
        }
        glvx::Event event;
        event.type = glvx::EventType::KeyPressed;
        event.key.code = key;
        addExternalEvent(event);
    }

    void Application::keyRelease(glvx::Key key) {
        wAssert(external_control);
        if (key == glvx::Key::LControl) {
            external_lctrl_pressed = false;
        } else if (key == glvx::Key::LAlt) {
            external_lalt_pressed = false;
        } else if (key == glvx::Key::LShift) {
            external_lshift_pressed = false;
        }
        glvx::Event event;
        event.type = glvx::EventType::KeyReleased;
        event.key.code = key;
        addExternalEvent(event);
    }

    void Application::textEntered(uint32_t code) {
        wAssert(external_control);
        glvx::Event event;
        event.type = glvx::EventType::TextEntered;
        event.text.unicode = code;
        addExternalEvent(event);
    }

    bool Application::isLeftButtonPressed() const {
        return leftButtonPressed;
    }

    bool Application::isRightButtonPressed() const {
        return rightButtonPressed;
    }

    bool Application::isMiddleButtonPressed() const {
        return middleButtonPressed;
    }

    bool Application::isLCtrlPressed() const {
        if (external_control) {
            return external_lctrl_pressed;
        } else {
            return glvx::Keyboard::isKeyPressed(glvx::Key::LControl);
        }
    }

    bool Application::isLAltPressed() const {
        if (external_control) {
            return external_lalt_pressed;
        } else {
            return glvx::Keyboard::isKeyPressed(glvx::Key::LAlt);
        }
    }

    bool Application::isLShiftPressed() const {
        if (external_control) {
            return external_lshift_pressed;
        } else {
            return glvx::Keyboard::isKeyPressed(glvx::Key::LShift);
        }
    }

    glvx::Vector2i Application::getMousePos() const {
        if (external_control) {
            return external_mouse_pos;
        } else {
            return internal_mouse_pos;
        }
    }

    glvx::Vector2f Application::getMousePosf() const {
        return to2f(getMousePos());
    }

    const glvx::Vector2f& Application::getMousePressPosf() const {
        return mousePressPosf;
    }

    glvx::Vector2f Application::getWindowCenter() const {
        glvx::Vector2f center = to2f(window.getSize()) / 2.0f;
		return center;
    }

    WidgetList& Application::getWidgets() {
        return widgets;
    }

    void Application::setBackgroundColor(const glvx::Color& color) {
        background_color = color;
    }

    void Application::setDefaultFont(const fw::Font& font) {
        widgets.setDefaultFont(font);
    }

    void Application::setVerticalSyncEnabled(bool value) {
        window.setVerticalSyncEnabled(value);
    }

    void Application::close() {
        running = false;
        onClose();
    }

    void Application::onInit() { }

    void Application::onStart() { }

    void Application::onFrameBegin() { }

    void Application::onFrameEnd() { }

    void Application::onProcessWidgets() { }

    void Application::onProcessWindowEvent(const glvx::Event& event) { }

    void Application::onProcessKeyboardEvent(const glvx::Event& event) { }

    void Application::onProcessLeftPress() { }

    void Application::onProcessRightPress() { }

    void Application::onProcessLeftRelease() { }

    void Application::onProcessRightRelease() { }

    void Application::onProcessMouseMove() { }

    void Application::onProcessMouseScrollX(float delta) { }

    void Application::onProcessMouseScrollY(float delta) { }

    void Application::onProcessKeyboard() { }

    void Application::onProcessMouse() { }

    void Application::onBeforeProcessInput() { }

    void Application::onAfterProcessInput() { }

    void Application::onProcessWorld() { }

    void Application::onRender() { }

    void Application::onClose() { }

    void Application::startMoveGesture(Widget* source) {
        LoggerTag tag_mouse_gesture("mouseGesture");
        mouse_gesture_left = MouseGesture(source, MouseGesture::MOVE, getMousePosf(), glvx::Mouse::Button::Left);
        std::string source_str = source->getFullName();
        std::string type_str = "move";
        logger << "Start gesture: \n";
        LoggerIndent indent;
        logger << source_str << "\n";
        logger << type_str << "\n";
        logger << getMousePosf() << "\n";
    }

    void Application::endGestureLeft() {
        LoggerTag tag_mouse_gesture("mouseGesture");
        mouse_gesture_left.active = false;
        logger << "End gesture\n";
    }

    void Application::endGestureRight() {
        LoggerTag tag_mouse_gesture("mouseGesture");
        mouse_gesture_right.active = false;
        logger << "End gesture\n";
    }

    void Application::mainLoop() {
        wAssert(!external_window);
        while (window.isOpen() && running) {
            advance();
        }
        window.close();
    }

    void Application::processWidgets() {
        widgets.reset(glvx::Vector2f((float)window.getSize().x, (float)window.getSize().y), getMousePosf());
        onProcessWidgets();
    }

    void Application::processBeforeInput() {
        internal_mouse_pos = glvx::Mouse::getPosition(window);
        widgets.processBeforeInput();
    }

    void Application::processInput() {
        if (external_control) {
            while (!external_event_queue.empty()) {
                glvx::Event event = external_event_queue.front();
                external_event_queue.pop();
                processEvent(event);
            }
        } else {
            glvx::Event event;
            while (window.pollEvent(event)) {
                processEvent(event);
            }
        }
        processKeyboard();
        processMouse();
    }

    void Application::processAfterInput() {
        onAfterProcessInput();
        widgets.processAfterInput();
    }

    void Application::processEvent(const glvx::Event& event) {
        processWindowEvent(event);
        processKeyboardEvent(event);
        processMouseEvent(event);
    }

    void Application::processWindowEvent(const glvx::Event& event) {
        widgets.processWindowEvent(event);
        onProcessWindowEvent(event);
    }

    void Application::processKeyboardEvent(const glvx::Event& event) {
        widgets.processKeyboardEvent(event);
        if (!widgets.getFocusedWidget()) {
            onProcessKeyboardEvent(event);
        }
    }

    void Application::processMouseEvent(const glvx::Event& event) {
        if (event.type == glvx::EventType::MouseButtonPressed) {
            switch (event.mouseButton.button) {
                case glvx::Mouse::Button::Left:
                    leftButtonPressed = true;
                    mousePressPosf = getMousePosf();
                    processLeftPress();
                    break;
                case glvx::Mouse::Button::Right:
                    rightButtonPressed = true;
                    mousePrevPos = glvx::Vector2i(event.mouseButton.x, event.mouseButton.y);
                    processRightPress();
                    break;
                case glvx::Mouse::Button::Middle:
                    middleButtonPressed = true;
					processMiddlePress();
					break;
            }
        } else if (event.type == glvx::EventType::MouseButtonReleased) {
            switch (event.mouseButton.button) {
                case glvx::Mouse::Button::Left:
                    leftButtonPressed = false;
                    processLeftRelease();
                    break;
                case glvx::Mouse::Button::Right:
                    rightButtonPressed = false;
                    processRightRelease();
                    break;
				case glvx::Mouse::Button::Middle:
					middleButtonPressed = false;
					processMiddleRelease();
					break;
            }
        } else if (event.type == glvx::EventType::MouseMoved) {
            processMouseMove();
        } else if (event.type == glvx::EventType::MouseWheelScrolled) {
            // GLVX wheel events are Y-only; simulated X scrolls carry an axis hint
            if (external_control && !external_wheel_axis_queue.empty()) {
                int axis = external_wheel_axis_queue.front();
                external_wheel_axis_queue.pop();
                if (axis == 0) {
                    processScrollX(event.mouseWheel.delta);
                } else {
                    processScrollY(event.mouseWheel.delta);
                }
            } else {
                processScrollY(event.mouseWheel.delta);
            }
        }
    }

    void Application::startNormalGesture(Widget* source, glvx::Mouse::Button button) {
        if (!source) {
            return;
        }
        LoggerTag tag_mouse_gesture("mouseGesture");
        if (button == glvx::Mouse::Button::Left) {
            mouse_gesture_left = MouseGesture(source, MouseGesture::NORMAL, getMousePosf(), button);
        } else if (button == glvx::Mouse::Button::Right) {
            mouse_gesture_right = MouseGesture(source, MouseGesture::NORMAL, getMousePosf(), button);
        }
        std::string source_str = source->getFullName();
        std::string type_str = "normal";
        std::string button_str;
        switch (button) {
            case glvx::Mouse::Button::Left: button_str = "left"; break;
            case glvx::Mouse::Button::Right: button_str = "right"; break;
            case glvx::Mouse::Button::Middle: button_str = "middle"; break;
            default: button_str = "other"; break;
        }
        logger << "Start gesture: \n";
        LoggerIndent indent;
        logger << source_str << "\n";
        logger << type_str << "\n";
        logger << getMousePosf() << "\n";
        logger << button_str << "\n";
    }

    void Application::processLeftPress() {
        if (mouse_gesture_left.active) {
            if (mouse_gesture_left.type == MouseGesture::MOVE) {
                mouse_gesture_left.source->processLeftPress(getMousePosf(), false);
                onProcessLeftPress();
                endGestureLeft();
            }
        } else {
            widgets.processLeftPress(getMousePosf());
            onProcessLeftPress();
            Widget* blocking_widget = widgets.getBlockingWidget();
            startNormalGesture(blocking_widget, glvx::Mouse::Button::Left);
        }
    }

    void Application::processRightPress() {
        if (mouse_gesture_right.active) {
            endGestureRight();
        } else {
            widgets.processRightPress(getMousePosf());
            onProcessRightPress();
            Widget* blocking_widget = widgets.getBlockingWidget();
            startNormalGesture(blocking_widget, glvx::Mouse::Button::Right);
        }
    }

    void Application::processMiddlePress() {
        // Not implemented yet
    }

    void Application::processLeftRelease() {
        if (!mouse_gesture_left.active) {
            return;
        }
        widgets.processLeftRelease(getMousePosf());
        onProcessLeftRelease();
        endGestureLeft();
    }

    void Application::processRightRelease() {
        if (!mouse_gesture_right.active) {
            return;
        }
        widgets.processRightRelease(getMousePosf());
        onProcessRightRelease();
        endGestureRight();
    }

    void Application::processMiddleRelease() {
        // Not implemented yet
    }

    void Application::processMouseMove() {
        widgets.processMouseMove(getMousePosf());
        onProcessMouseMove();
    }

    void Application::processScrollX(float delta) {
        widgets.processScrollX(getMousePosf(), delta);
        onProcessMouseScrollX(delta);
    }

    void Application::processScrollY(float delta) {
        widgets.processScrollY(getMousePosf(), delta);
        onProcessMouseScrollY(delta);
    }

    void Application::processKeyboard() {
        onProcessKeyboard();
    }

    void Application::processMouse() {
        glvx::Vector2f mousePosf = getMousePosf();
        widgets.processMouse(mousePosf);
        Widget* gesture_source_left = getLeftGestureSource();
        Widget* gesture_source_right = getRightGestureSource();
        onProcessMouse();
        if (mouse_gesture_left.active && mouse_gesture_left.type == MouseGesture::NORMAL && gesture_source_left) {
            gesture_source_left->OnProcessDragGesture(glvx::Mouse::Button::Left, mousePosf);
        }
        if (mouse_gesture_right.active && mouse_gesture_right.type == MouseGesture::NORMAL && gesture_source_right) {
            gesture_source_right->OnProcessDragGesture(glvx::Mouse::Button::Right, mousePosf);
        }
        CursorType cursor_type = CursorType::Arrow;
        widgets.getCurrentCursorType(cursor_type);
        setCursorType(cursor_type);
        mousePrevPos = getMousePos();
        mousePrevPosf = getMousePosf();
    }


    void Application::processWorld() {
        onProcessWorld();
    }

    void Application::render() {
        widgets.updateRenderQueue();
        widgets.lock();
        glvx::Vector2i window_size = window.getSize();
        window_view.setPosition(to2f(window_size) / 2.0f);
        window.setView(window_view);
        onRender();
        if (window_size.x != (int)render_texture.getWidth() || window_size.y != (int)render_texture.getHeight()) {
			render_texture.create(window_size.x, window_size.y);
        }
        glvx::View rt_view;
        rt_view.setPosition(to2f(window_size) / 2.0f);
        rt_view.setScale(1.0f, -1.0f);
        render_texture.setView(rt_view);
        render_texture.clear(background_color);
        widgets.render(render_texture);
        render_texture.display();
        draw_texture_rect(
            window,
            render_texture,
            glvx::Vector2f(0.0f, 0.0f),
            to2f(window_size),
            glvx::Color::White
        );
        window.display();
        widgets.unlock();
    }

    void Application::setCursorType(CursorType type) {
        if (type == current_cursor_type) {
            return;
        }
        current_cursor_type = type;
        switch (type) {
            case CursorType::Arrow: window.setMouseCursor(arrow_cursor); break;
            case CursorType::Text: window.setMouseCursor(text_cursor); break;
            case CursorType::SizeTopLeft: window.setMouseCursor(size_top_left_cursor); break;
            case CursorType::SizeTop: window.setMouseCursor(size_top_cursor); break;
            case CursorType::SizeTopRight: window.setMouseCursor(size_top_right_cursor); break;
            case CursorType::SizeLeft: window.setMouseCursor(size_left_cursor); break;
            case CursorType::SizeRight: window.setMouseCursor(size_right_cursor); break;
            case CursorType::SizeBottomLeft: window.setMouseCursor(size_bottom_left_cursor); break;
            case CursorType::SizeBottom: window.setMouseCursor(size_bottom_cursor); break;
            case CursorType::SizeBottomRight: window.setMouseCursor(size_bottom_right_cursor); break;
            default: window.setMouseCursor(arrow_cursor); break;
        }
    }

    MouseGesture::MouseGesture() { }

    MouseGesture::MouseGesture(
        Widget* source,
        MouseGestureType type,
        glvx::Vector2f startPos,
        glvx::Mouse::Button button
    ) {
        this->source = source;
        this->type = type;
        this->startPos = startPos;
        this->button = button;
        active = true;
    }

}
