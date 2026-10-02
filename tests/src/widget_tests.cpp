#include "widget_tests/widget_tests.h"
#include "widget_tests/widget_tests_application.h"
#include "widget_tests/widget_tests_basic.h"
#include "widget_tests/widget_tests_button.h"
#include "widget_tests/widget_tests_canvas.h"
#include "widget_tests/widget_tests_checkbox.h"
#include "widget_tests/widget_tests_container.h"
#include "widget_tests/widget_tests_dropdown.h"
#include "widget_tests/widget_tests_post_actions.h"
#include "widget_tests/widget_tests_render.h"
#include "widget_tests/widget_tests_scroll_area.h"
#include "widget_tests/widget_tests_size_policy.h"
#include "widget_tests/widget_tests_text.h"
#include "widget_tests/widget_tests_textbox.h"
#include "widget_tests/widget_tests_tree_view.h"
#include "widget_tests/widget_tests_toposort.h"
#include "widget_tests/widget_tests_widget_link.h"
#include "widget_tests/widget_tests_window.h"

WidgetTests::WidgetTests(const std::string& name, test::TestModule* parent, const std::vector<TestNode*>& required_nodes) : TestModule(name, parent, required_nodes) {
    WidgetTestsToposort* toposort_list = addModule<WidgetTestsToposort>("Toposort");
    WidgetTestsApplication* application_list = addModule<WidgetTestsApplication>("Application", { toposort_list });
    WidgetTestsBasic* widgets_basic_list = addModule<WidgetTestsBasic>("WidgetsBasic", { application_list });
    WidgetTestsRender* render_list = addModule<WidgetTestsRender>("Render", { widgets_basic_list });
    WidgetTestsPostActions* pending_list = addModule<WidgetTestsPostActions>("PostActions", { widgets_basic_list });
    WidgetTestsText* text_widget_list = addModule<WidgetTestsText>("TextWidget", { application_list });
    WidgetTestsCheckbox* checkbox_widget_list = addModule<WidgetTestsCheckbox>("CheckboxWidget", { widgets_basic_list });
    WidgetTestsContainer* container_widget_list = addModule<WidgetTestsContainer>("ContainerWidget", { widgets_basic_list });
    WidgetTestsSizePolicy* size_policy_list = addModule<WidgetTestsSizePolicy>("SizePolicy", { widgets_basic_list });
    WidgetTestsWidgetLink* widget_link_list = addModule<WidgetTestsWidgetLink>("WidgetLink", { widgets_basic_list, size_policy_list });
    WidgetTestsTextbox* textbox_widget_list = addModule<WidgetTestsTextbox>("TextBoxWidget", { widgets_basic_list, text_widget_list });
    WidgetTestsCanvas* canvas_widget_list = addModule<WidgetTestsCanvas>("CanvasWidget", { widgets_basic_list });
    WidgetTestsWindow* window_widget_list = addModule<WidgetTestsWindow>("WindowWidget", { widgets_basic_list, text_widget_list });
    WidgetTestsDropdown* dropdown_widget_list = addModule<WidgetTestsDropdown>("DropdownWidget", { widgets_basic_list, text_widget_list });
    WidgetTestsScrollArea* scroll_area_widget_list = addModule<WidgetTestsScrollArea>("ScrollAreaWidget", { widgets_basic_list });
    WidgetTestsTreeView* tree_view_widget_list = addModule<WidgetTestsTreeView>("TreeViewWidget", { widgets_basic_list });
    WidgetTestsButton* button_widget_list = addModule<WidgetTestsButton>("ButtonWidget", { widgets_basic_list });
}

void WidgetTests::beforeRunModule() {
    window.create(800, 600, "Widget tests", 0, minimized);
    textbox_font = fw::Font("fonts/verdana.ttf");
    fw::WidgetList::debug_mouse = true;
}

void WidgetTests::afterRunModule() {
    window.close();
    fw::WidgetList::debug_mouse = false;
}

std::string WidgetTests::sfVec2fToStr(const glvx::Vector2f& vec) {
    return "(" + fw::vec_to_str(vec) + ")";
}

std::string WidgetTests::sfVec2iToStr(const glvx::Vector2i& vec) {
    return "(" + fw::vec_to_str(vec) + ")";
}

std::string WidgetTests::sfVec2uToStr(const glvx::Vector2u& vec) {
    return "(" + fw::vec_to_str(vec) + ")";
}

std::string WidgetTests::cursorTypeToStr(fw::CursorType type) {
    switch (type) {
        case fw::CursorType::Arrow:           return "Arrow";
        case fw::CursorType::Text:            return "Text";
        case fw::CursorType::SizeTopLeft:     return "SizeTopLeft";
        case fw::CursorType::SizeTop:         return "SizeTop";
        case fw::CursorType::SizeTopRight:    return "SizeTopRight";
        case fw::CursorType::SizeLeft:        return "SizeLeft";
        case fw::CursorType::SizeRight:       return "SizeRight";
        case fw::CursorType::SizeBottomLeft:  return "SizeBottomLeft";
        case fw::CursorType::SizeBottom:      return "SizeBottom";
        case fw::CursorType::SizeBottomRight: return "SizeBottomRight";
        default:                          wAssert(false, "Unknown cursor type"); return "Unknown";
    }
}

std::string WidgetTests::floatRectToStr(const glvx::FloatRect& rect) {
    return "pos: " + sfVec2fToStr(rect.position) + " size: " + sfVec2fToStr(rect.size);
}

std::string WidgetTests::colorToStr(const glvx::Color& color) {
    return "(" + fw::color_to_str(color) + ")";
}

std::string WidgetTests::anchorToStr(fw::Widget::Anchor anchor) {
    switch (anchor) {
        case fw::Widget::Anchor::CUSTOM:        return "CUSTOM";
        case fw::Widget::Anchor::TOP_LEFT:      return "TOP_LEFT";
        case fw::Widget::Anchor::TOP_CENTER:    return "TOP_CENTER";
        case fw::Widget::Anchor::TOP_RIGHT:     return "TOP_RIGHT";
        case fw::Widget::Anchor::CENTER_LEFT:   return "CENTER_LEFT";
        case fw::Widget::Anchor::CENTER:        return "CENTER";
        case fw::Widget::Anchor::CENTER_RIGHT:  return "CENTER_RIGHT";
        case fw::Widget::Anchor::BOTTOM_LEFT:   return "BOTTOM_LEFT";
        case fw::Widget::Anchor::BOTTOM_CENTER: return "BOTTOM_CENTER";
        case fw::Widget::Anchor::BOTTOM_RIGHT:  return "BOTTOM_RIGHT";
        default:                                wAssert("Unknown anchor type"); return "Unknown";
    }
}

bool WidgetTests::rectApproxCmp(const glvx::FloatRect& left, const glvx::FloatRect& right) {
    float epsilon = 0.0001f;
    if (abs(left.position.x - right.position.x) >= epsilon) {
        return false;
    }
    if (abs(left.position.y - right.position.y) >= epsilon) {
        return false;
    }
    if (abs(left.size.x - right.size.x) >= epsilon) {
        return false;
    }
    if (abs(left.size.y - right.size.y) >= epsilon) {
        return false;
    }
    return true;
}

void WidgetTests::mouseDragGesture(
    fw::Application& application,
    const glvx::Vector2f& begin_pos,
    const glvx::Vector2f& offset
) {
    application.mouseMove(begin_pos);
    application.advance();
    application.mouseLeftPress();
    application.advance();
    application.mouseMove(begin_pos + offset);
    application.advance();
    application.mouseLeftRelease();
    application.advance();
}

glvx::Vector2f WidgetTests::getGrabPos(fw::WindowWidget* window, ResizePoint resize_point) {
    float cursor_offset = fw::WINDOW_RESIZE_MARGIN / 2.0f;
    glvx::Vector2f grab_pos;
    if (resize_point == ResizePoint::TOP_LEFT) {
        grab_pos = window->getGlobalTopLeft() + glvx::Vector2f(-cursor_offset, -cursor_offset);
    } else if (resize_point == ResizePoint::TOP) {
        grab_pos = window->getGlobalTop() + glvx::Vector2f(0.0f, -cursor_offset);
    } else if (resize_point == ResizePoint::TOP_RIGHT) {
        grab_pos = window->getGlobalTopRight() + glvx::Vector2f(cursor_offset, -cursor_offset);
    } else if (resize_point == ResizePoint::LEFT) {
        grab_pos = window->getGlobalLeft() + glvx::Vector2f(-cursor_offset, 0.0f);
    } else if (resize_point == ResizePoint::RIGHT) {
        grab_pos = window->getGlobalRight() + glvx::Vector2f(cursor_offset, 0.0f);
    } else if (resize_point == ResizePoint::BOTTOM_LEFT) {
        grab_pos = window->getGlobalBottomLeft() + glvx::Vector2f(-cursor_offset, cursor_offset);
    } else if (resize_point == ResizePoint::BOTTOM) {
        grab_pos = window->getGlobalBottom() + glvx::Vector2f(0.0f, cursor_offset);
    } else if (resize_point == ResizePoint::BOTTOM_RIGHT) {
        grab_pos = window->getGlobalBottomRight() + glvx::Vector2f(cursor_offset, cursor_offset);
    } else {
        wAssert(false, "Unknown resize point");
    }
    return grab_pos;
}

void WidgetTests::resizeWindow(fw::WindowWidget* window, ResizePoint resize_point, const glvx::Vector2f offset) {
    glvx::Vector2f grab_pos = getGrabPos(window, resize_point);
    mouseDragGesture(window->getWidgetList().getApplication(), grab_pos, offset);
}

void WidgetTests::dragWindow(fw::Application& application, fw::WindowWidget* window, const glvx::Vector2f& offset) {
    glvx::Vector2f header_center = window->getHeaderWidget()->getGlobalCenter();
    application.mouseMove(header_center);
    application.advance();
    application.mouseLeftPress();
    application.advance();
    application.mouseMove(header_center + offset);
    application.advance();
    application.mouseLeftRelease();
    application.advance();
}

void WidgetTests::genericWidgetTest(const GenericWidgetTest& gwt) {
    fw::Application& application = gwt.application;
    test::Test& test = gwt.test;
    fw::Widget* widget = gwt.widget;
    T_COMPARE(application.getWidgets().getSize(), gwt.total_widgets);
    T_CHECK(widget->getType() == gwt.type);
    T_COMPARE(widget->getName(), gwt.name);
    T_COMPARE(widget->getFullName(), gwt.fullname);
    T_COMPARE(widget->isVisualPositionQuantized(), gwt.is_visual_position_quantized);
    fw::WidgetVisibility wv = widget->checkVisibility();
    T_COMPARE(wv.addedToRoot, gwt.visibility.addedToRoot);
    T_COMPARE(wv.allParentsVisible, gwt.visibility.allParentsVisible);
    T_COMPARE(wv.hasUnclippedRegion, gwt.visibility.hasUnclippedRegion);
    T_COMPARE(wv.nonZeroSize, gwt.visibility.nonZeroSize);
    T_COMPARE(wv.onScreen, gwt.visibility.onScreen);
    T_COMPARE(wv.opaque, gwt.visibility.opaque);
    T_COMPARE(wv.renderableSetting, gwt.visibility.renderableSetting);
    T_COMPARE(wv.visibleSetting, gwt.visibility.visibleSetting);
    T_COMPARE(widget->isClickThrough(), gwt.is_click_through);
    T_COMPARE(widget->isMouseOver(), gwt.is_mouse_over);
    T_CHECK(widget->getFocusableType() == gwt.focusable_type);
    T_COMPARE(widget->isFocused(), gwt.is_focused);
    T_COMPARE(widget->getClipChildren(), gwt.clip_children);
    T_COMPARE(widget->getForceCustomCursor(), gwt.force_custom_cursor);
    T_CHECK(widget->getParent() == gwt.parent);
    auto rect_to_str = &WidgetTests::floatRectToStr;
    auto rect_approx_cmp = &WidgetTests::rectApproxCmp;
    T_COMPARE(widget->getLocalBounds(), gwt.local_bounds, rect_to_str, rect_approx_cmp);
    T_COMPARE(widget->getParentLocalBounds(), gwt.parent_local_bounds, rect_to_str, rect_approx_cmp);
    T_COMPARE(widget->getGlobalBounds(), gwt.global_bounds, rect_to_str, rect_approx_cmp);
    T_COMPARE(widget->getVisualLocalBounds(), gwt.visual_local_bounds, rect_to_str, rect_approx_cmp);
    T_COMPARE(widget->getVisualParentLocalBounds(), gwt.visual_parent_local_bounds, rect_to_str, rect_approx_cmp);
    T_COMPARE(widget->getVisualGlobalBounds(), gwt.visual_global_bounds, rect_to_str, rect_approx_cmp);
    T_COMPARE(widget->getUnclippedRegion(), gwt.visual_global_bounds, rect_to_str, rect_approx_cmp);
    glvx::FloatRect quantized = gwt.visual_global_bounds;
    quantized.position.x = floor(quantized.position.x);
    quantized.position.y = floor(quantized.position.y);
    quantized.size.x = floor(quantized.size.x);
    quantized.size.y = floor(quantized.size.y);
    T_COMPARE(widget->getQuantizedUnclippedRegion(), quantized, rect_to_str, rect_approx_cmp);
    T_COMPARE(widget->getWidth(), gwt.parent_local_bounds.size.x);
    T_APPROX_COMPARE(widget->getHeight(), gwt.parent_local_bounds.size.y);
    T_APPROX_COMPARE(widget->getGlobalWidth(), gwt.parent_local_bounds.size.x);
    T_APPROX_COMPARE(widget->getGlobalHeight(), gwt.parent_local_bounds.size.y);
    T_VEC2_APPROX_COMPARE(widget->getSize(), gwt.local_bounds.size);
    auto get_corners = [&](const glvx::FloatRect& bounds) {
        std::vector<glvx::Vector2f> corners(4);
        corners[0] = bounds.position;
        corners[1] = bounds.position + glvx::Vector2f(gwt.local_bounds.size.x, 0.0f);
        corners[2] = bounds.position + glvx::Vector2f(0.0f, bounds.size.y);
        corners[3] = bounds.position + bounds.size;
        return corners;
    };
    std::vector<glvx::Vector2f> parent_local_corners = get_corners(gwt.parent_local_bounds);
    std::vector<glvx::Vector2f> global_corners = get_corners(gwt.global_bounds);
    std::vector<glvx::Vector2f> visual_global_corners = get_corners(gwt.visual_global_bounds);
    T_VEC2_APPROX_COMPARE(widget->getTopLeft(), parent_local_corners[0]);
    T_VEC2_APPROX_COMPARE(widget->getTopRight(), parent_local_corners[1]);
    T_VEC2_APPROX_COMPARE(widget->getBottomLeft(), parent_local_corners[2]);
    T_VEC2_APPROX_COMPARE(widget->getBottomRight(), parent_local_corners[3]);
    T_VEC2_APPROX_COMPARE(widget->getGlobalTopLeft(), global_corners[0]);
    T_VEC2_APPROX_COMPARE(widget->getGlobalTopRight(), global_corners[1]);
    T_VEC2_APPROX_COMPARE(widget->getGlobalBottomLeft(), global_corners[2]);
    T_VEC2_APPROX_COMPARE(widget->getGlobalBottomRight(), global_corners[3]);
    T_VEC2_APPROX_COMPARE(widget->getVisualGlobalTopLeft(), visual_global_corners[0]);
    T_VEC2_APPROX_COMPARE(widget->getVisualGlobalTopRight(), visual_global_corners[1]);
    T_VEC2_APPROX_COMPARE(widget->getVisualGlobalBottomLeft(), visual_global_corners[2]);
    T_VEC2_APPROX_COMPARE(widget->getVisualGlobalBottomRight(), visual_global_corners[3]);
}

GenericWidgetTest::GenericWidgetTest(
    fw::Application& application,
    test::Test& test
) : application(application), test(test) { }
