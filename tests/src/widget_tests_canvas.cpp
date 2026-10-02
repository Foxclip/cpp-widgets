#include "widget_tests/widget_tests.h"
#include "widget_tests/widget_tests_canvas.h"

WidgetTestsCanvas::WidgetTestsCanvas(const std::string& name, test::TestModule* parent, const std::vector<TestNode*>& required_nodes) : WidgetTest(name, parent, required_nodes) {
    test::Test* canvas_widget_basic_test = addTest("basic", [&](test::Test& test) { canvasWidgetBasicTest(test); });
    test::Test* canvas_widget_draw_test = addTest("draw", { canvas_widget_basic_test }, [&](test::Test& test) { canvasWidgetDrawTest(test); });
    test::Test* canvas_widget_alpha_test = addTest("alpha", { canvas_widget_basic_test }, [&](test::Test& test) { canvasWidgetAlphaTest(test); });
}

void WidgetTestsCanvas::canvasWidgetBasicTest(test::Test& test) {
    fw::Application application(getWindow());
    application.init(test.name, 800, 600, 0, false);
    application.start(true);
    application.mouseMove(400, 300);
    application.advance();
    glvx::Vector2f size(100.0f, 100.0f);
    fw::CanvasWidget* canvas_widget =
        application.getWidgets().createCanvasWidget(size, glvx::Vector2u(100, 100));
    fw::Widget* root_widget = application.getWidgets().getRootWidget();
    T_ASSERT(T_CHECK(canvas_widget));
    glvx::Vector2f position(100.0f, 100.0f);
    canvas_widget->setPosition(position);

    GenericWidgetTest gwt(application, test);
    gwt.widget = canvas_widget;
    gwt.total_widgets = 2;
    gwt.type = fw::Widget::WidgetType::Canvas;
    gwt.name = "canvas";
    gwt.fullname = "root|canvas";
    gwt.is_visual_position_quantized = false;
    fw::WidgetVisibility visibility;
    visibility.addedToRoot = true;
    visibility.allParentsVisible = true;
    visibility.hasUnclippedRegion = true;
    visibility.nonZeroSize = true;
    visibility.onScreen = true;
    visibility.opaque = true;
    visibility.renderableSetting = true;
    visibility.visibleSetting = true;
    gwt.visibility = visibility;
    gwt.is_click_through = true;
    gwt.is_mouse_over = false;
    gwt.focusable_type = fw::Widget::FocusableType::NONE;
    gwt.is_focused = false;
    gwt.clip_children = false;
    gwt.force_custom_cursor = false;
    gwt.parent = root_widget;
    gwt.local_bounds = glvx::FloatRect(glvx::Vector2f(), size);
    gwt.global_bounds = glvx::FloatRect(position, size);
    gwt.parent_local_bounds = gwt.global_bounds;
    gwt.visual_local_bounds = gwt.local_bounds;
    gwt.visual_global_bounds = gwt.global_bounds;
    gwt.visual_parent_local_bounds = gwt.global_bounds;
    T_WRAP_CONTAINER(WidgetTests::genericWidgetTest(gwt));

    T_COMPARE(canvas_widget->getChildren().size(), 0);
}

void WidgetTestsCanvas::canvasWidgetDrawTest(test::Test& test) {
    fw::Application application(getWindow());
    application.init(test.name, 800, 600, 0, false);
    application.start(true);
    application.mouseMove(400, 300);
    application.advance();
    fw::CanvasWidget* canvas_widget = application.getWidgets().createCanvasWidget(100.0f, 100.0f, 100, 100);
    auto color_to_str = &WidgetTests::colorToStr;
    {
        canvas_widget->clear();
        glvx::Image image = canvas_widget->getRenderTexture().readPixels();
        T_ASSERT(T_COMPARE(image.getPixel(0, 0), glvx::Color::Black, color_to_str));
    }
    {
        canvas_widget->clear(glvx::Color(128, 128, 128));
        glvx::Image image = canvas_widget->getRenderTexture().readPixels();
        T_ASSERT(T_COMPARE(image.getPixel(0, 0), glvx::Color(128, 128, 128), color_to_str));
    }
    {
        canvas_widget->clear(glvx::Color::Red);
        glvx::Rectangle rect(glvx::Vector2f(30.0f, 30.0f));
        rect.setPosition(10.0f, 10.0f);
        rect.setColor(glvx::Color::Green);
        canvas_widget->draw(rect);
        canvas_widget->display();
        glvx::Image image = canvas_widget->getRenderTexture().readPixels();
        T_COMPARE(image.getPixel(0, 0), glvx::Color::Red, color_to_str);
        T_COMPARE(image.getPixel(5, 5), glvx::Color::Red, color_to_str);
        T_COMPARE(image.getPixel(10, 10), glvx::Color::Green, color_to_str);
        T_COMPARE(image.getPixel(15, 15), glvx::Color::Green, color_to_str);
        T_COMPARE(image.getPixel(20, 20), glvx::Color::Green, color_to_str);
        T_COMPARE(image.getPixel(25, 25), glvx::Color::Green, color_to_str);
        T_COMPARE(image.getPixel(30, 30), glvx::Color::Green, color_to_str);
        T_COMPARE(image.getPixel(35, 35), glvx::Color::Green, color_to_str);
        T_COMPARE(image.getPixel(40, 40), glvx::Color::Red, color_to_str);
        T_COMPARE(image.getPixel(45, 45), glvx::Color::Red, color_to_str);
        T_COMPARE(image.getPixel(50, 50), glvx::Color::Red, color_to_str);
        T_COMPARE(image.getPixel(55, 55), glvx::Color::Red, color_to_str);
        T_COMPARE(image.getPixel(60, 60), glvx::Color::Red, color_to_str);
    }
}

void WidgetTestsCanvas::canvasWidgetAlphaTest(test::Test& test) {
    fw::Application application(getWindow());
    application.init(test.name, 800, 600, 0, false);
    application.start(true);
    application.mouseMove(400, 300);
    fw::CanvasWidget* canvas_widget = application.getWidgets().createCanvasWidget(100.0f, 100.0f, 100, 100);
    auto color_to_str = &WidgetTests::colorToStr;
    {
        glvx::Rectangle rect(glvx::Vector2f(100.0f, 100.0f));
        canvas_widget->clear();
        rect.setColor(glvx::Color(255, 0, 0, 128));
        canvas_widget->draw(rect);
        rect.setColor(glvx::Color(0, 255, 0, 128));
        canvas_widget->draw(rect);
        rect.setColor(glvx::Color(0, 0, 255, 128));
        canvas_widget->draw(rect);
        glvx::Image image = canvas_widget->getRenderTexture().readPixels();
        T_ASSERT(T_COMPARE(image.getPixel(0, 0), glvx::Color(32, 64, 128), color_to_str));
    }
}
