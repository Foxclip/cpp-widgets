#include "widget_tests/widget_tests.h"
#include "widget_tests/widget_tests_render.h"
#include "widgets/widgets_common.h"

WidgetTestsRender::WidgetTestsRender(const std::string& name, test::TestModule* parent, const std::vector<TestNode*>& required_nodes) : WidgetTest(name, parent, required_nodes) {
    test::Test* empty_test = addTest("empty", [&](test::Test& test) { emptyTest(test); });
    test::Test* rectangle_test = addTest("rectangle", { empty_test }, [&](test::Test& test) { rectangleTest(test); });
    test::Test* visibility_test = addTest("visibility", { rectangle_test }, [&](test::Test& test) { visibilityTest(test); });
    test::Test* local_layers_test = addTest("local_layers", { rectangle_test }, [&](test::Test& test) { localLayersTest(test); });
    test::Test* global_layers_test = addTest("global_layers", { rectangle_test }, [&](test::Test& test) { globalLayersTest(test); });
}

void WidgetTestsRender::emptyTest(test::Test& test) {
    glvx::Vector2u size(3, 3);
    glvx::Window& window = getWindow();
    fw::Application application(window);
    application.init(test.name, size.x, size.y, 0, false);
    application.start(true);
    application.mouseMove(size.x / 2, size.y / 2);
    application.advance();

    checkPixels(test, application, size, [&](test::Test&, const glvx::Image& image, unsigned int x, unsigned int y) {
        T_ASSERT(T_COMPARE(image.getPixel(x, y), glvx::Color::Black, &WidgetTests::colorToStr));
    });

    glvx::Color bg_color = glvx::Color::Red;
    application.setBackgroundColor(bg_color);
    application.advance();

    checkPixels(test, application, size, [&](test::Test&, const glvx::Image& image, unsigned int x, unsigned int y) {
        T_ASSERT(T_COMPARE(image.getPixel(x, y), bg_color, &WidgetTests::colorToStr));
    });
}

void WidgetTestsRender::rectangleTest(test::Test& test) {
    glvx::Vector2u size(4, 4);
    glvx::Window& window = getWindow();
    fw::Application application(window);
    application.init(test.name, size.x, size.y, 0, false);
    application.start(true);
    application.mouseMove(size.x / 2, size.y / 2);
    application.advance();

    glvx::Color rect_color = glvx::Color::Red;
    glvx::Color bg_color = glvx::Color::Black;
    fw::WidgetList& widgets = application.getWidgets();
    fw::RectangleWidget* rectangle_widget = widgets.createRectangleWidget(2.0f, 2.0f);
    rectangle_widget->setPosition(glvx::Vector2f(1.0f, 1.0f));
    rectangle_widget->setFillColor(rect_color);
    application.advance();

    checkPixels(test, application, size, [&](test::Test&, const glvx::Image& image, unsigned int x, unsigned int y) {
            if (x == 0 || x == image.getWidth() - 1 || y == 0 || y == image.getHeight() - 1) {
            T_ASSERT(T_COMPARE(image.getPixel(x, y), bg_color, &WidgetTests::colorToStr));
        } else {
            T_ASSERT(T_COMPARE(image.getPixel(x, y), rect_color, &WidgetTests::colorToStr));
        }
    });
}

void WidgetTestsRender::visibilityTest(test::Test& test) {
    glvx::Vector2u size(4, 4);
    glvx::Window& window = getWindow();
    fw::Application application(window);
    application.init(test.name, size.x, size.y, 0, false);
    application.start(true);
    application.mouseMove(size.x / 2, size.y / 2);
    application.advance();

    glvx::Color rect_color = glvx::Color::Red;
    glvx::Color bg_color = glvx::Color::Black;
    fw::WidgetList& widgets = application.getWidgets();
    fw::RectangleWidget* rectangle_widget = widgets.createRectangleWidget(2.0f, 2.0f);
    rectangle_widget->setPosition(glvx::Vector2f(1.0f, 1.0f));
    rectangle_widget->setFillColor(rect_color);
    application.advance();

    auto check_rect_visible = [&]() {
		T_CONTAINER("check_rect_visible");
        checkPixels(test, application, size, [&](test::Test&, const glvx::Image& image, unsigned int x, unsigned int y) {
        if (x == 0 || x == image.getWidth() - 1 || y == 0 || y == image.getHeight() - 1) {
                T_ASSERT(T_COMPARE(image.getPixel(x, y), bg_color, &WidgetTests::colorToStr));
            } else {
                T_ASSERT(T_COMPARE(image.getPixel(x, y), rect_color, &WidgetTests::colorToStr));
            }
        });
    };
    auto check_rect_invisible = [&]() {
        T_CONTAINER("check_rect_invisible");
        checkPixels(test, application, size, [&](test::Test&, const glvx::Image& image, unsigned int x, unsigned int y) {
            T_ASSERT(T_COMPARE(image.getPixel(x, y), bg_color, &WidgetTests::colorToStr));
        });
    };

    check_rect_visible();

    rectangle_widget->setVisible(false);
    application.advance();
    check_rect_invisible();

    rectangle_widget->setVisible(true);
    rectangle_widget->setRenderable(false);
    application.advance();
    check_rect_invisible();

    rectangle_widget->setVisible(true);
    rectangle_widget->setRenderable(true);
    application.advance();
    check_rect_visible();
}

void WidgetTestsRender::localLayersTest(test::Test& test) {
    glvx::Vector2u size(3, 3);
    glvx::Window& window = getWindow();
    fw::Application application(window);
    application.init(test.name, size.x, size.y, 0, false);
    application.start(true);
    application.mouseMove(size.x / 2, size.y / 2);
    application.advance();

    fw::WidgetList& widgets = application.getWidgets();
    fw::RectangleWidget* rectangle_1_widget = widgets.createRectangleWidget(3.0f, 3.0f);
    fw::RectangleWidget* rectangle_2_widget = widgets.createRectangleWidget(3.0f, 3.0f);
    rectangle_1_widget->setFillColor(glvx::Color::Red);
    rectangle_2_widget->setFillColor(glvx::Color::Green);
    application.advance();

    checkPixels(test, application, size, [&](test::Test&, const glvx::Image& image, unsigned int x, unsigned int y) {
        T_ASSERT(T_COMPARE(image.getPixel(x, y), glvx::Color::Green, &WidgetTests::colorToStr));
    });
    rectangle_1_widget->moveToTop();
    application.advance();
    checkPixels(test, application, size, [&](test::Test&, const glvx::Image& image, unsigned int x, unsigned int y) {
        T_ASSERT(T_COMPARE(image.getPixel(x, y), glvx::Color::Red, &WidgetTests::colorToStr));
    });
}

void WidgetTestsRender::globalLayersTest(test::Test& test) {
    glvx::Vector2u size(3, 3);
    glvx::Window& window = getWindow();
    fw::Application application(window);
    application.init(test.name, size.x, size.y, 0, false);
    application.start(true);
    application.mouseMove(size.x / 2, size.y / 2);
    application.advance();

    fw::WidgetList& widgets = application.getWidgets();
    fw::RectangleWidget* rectangle_1_widget = widgets.createRectangleWidget(3.0f, 3.0f);
    fw::RectangleWidget* rectangle_2_widget = widgets.createRectangleWidget(3.0f, 3.0f);
    rectangle_1_widget->setFillColor(glvx::Color::Red);
    rectangle_2_widget->setFillColor(glvx::Color::Green);
    application.advance();

    checkPixels(test, application, size, [&](test::Test&, const glvx::Image& image, unsigned int x, unsigned int y) {
        T_ASSERT(T_COMPARE(image.getPixel(x, y), glvx::Color::Green, &WidgetTests::colorToStr));
    });
    rectangle_1_widget->setGlobalRenderLayer(fw::GlobalRenderLayer::TOP);
    application.advance();
    checkPixels(test, application, size, [&](test::Test&, const glvx::Image& image, unsigned int x, unsigned int y) {
        T_ASSERT(T_COMPARE(image.getPixel(x, y), glvx::Color::Red, &WidgetTests::colorToStr));
    });
}

void WidgetTestsRender::beforeRunModule() {
    debug_mouse_saved = fw::WidgetList::debug_mouse;
    fw::WidgetList::debug_mouse = false; // mouse cursor will be on rendered textures otherwise
}

void WidgetTestsRender::afterRunModule() {
	fw::WidgetList::debug_mouse = debug_mouse_saved;
}

void WidgetTestsRender::checkPixels(
    test::Test& test,
    fw::Application& application,
    const glvx::Vector2u& size,
    const std::function<void(test::Test&, const glvx::Image&, unsigned int x, unsigned int y)> func
) const {
    const glvx::Image& image = application.getRenderedImage();
    T_ASSERT(T_VEC2_COMPARE(glvx::Vector2u(image.getWidth(), image.getHeight()), size));
    for (unsigned int y = 0; y < (unsigned int)image.getHeight(); y++) {
        for (unsigned int x = 0; x < (unsigned int)image.getWidth(); x++) {
            T_ASSERT_NO_ERRORS();
            T_CONTAINER("Pixel (" + std::to_string(x) + ", " + std::to_string(y) + ")");
            func(test, image, x, y);
        }
    }
}
