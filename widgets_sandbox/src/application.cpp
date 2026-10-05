#include "application.h"
#include <iostream>
#include <vector>

namespace sandbox {

	static std::string treeEntryFullName(fw::TreeViewEntry* entry) {
		std::vector<std::string> names;
		for (fw::TreeViewEntry* current = entry; current != nullptr; current = current->getParent()) {
			names.push_back(current->getTextWidget()->getString());
		}
		std::string full;
		for (auto it = names.rbegin(); it != names.rend(); ++it) {
			if (!full.empty()) {
				full += "/";
			}
			full += *it;
		}
		return full;
	}

	Application::Application(const std::string& section)
		: m_section(section), m_font("fonts/verdana.ttf") {
	}

	void Application::onInit() {
		setBackgroundColor(glvx::Color(35, 38, 44));
		setDefaultFont(m_font);
		getWidgets().OnKeyPressed += [](const glvx::Key& key) {
			if (key == glvx::Key::D) {
				fw::WidgetList::debug_render = !fw::WidgetList::debug_render;
			}
		};
		setupTextShowcase(createSection("text", "Text", 16.0f, 56.0f));
		setupShapesShowcase(createSection("shapes", "Shapes", 80.0f, 64.0f));
		setupButtonShowcase(createSection("button", "Button", 152.0f, 56.0f));
		setupCheckboxShowcase(createSection("checkbox", "Checkbox", 216.0f, 56.0f));
		setupDropdownShowcase(createSection("dropdown", "Dropdown", 280.0f, 56.0f));
		setupTextboxShowcase(createSection("textbox", "Textbox", 344.0f, 56.0f));
		setupLayoutShowcase(createSection("layout", "Layout", 408.0f, 110.0f));
		setupScrollAreaShowcase(createSection("scroll-area", "Scroll area", 526.0f, 88.0f));
		setupTreeViewShowcase(createSection("tree-view", "Tree view", 622.0f, 150.0f));
		setupCanvasShowcase(createSection("canvas", "Canvas", 780.0f, 80.0f));
		setupWindowShowcase(createSection("window", "Window", 868.0f, 112.0f));
		if (!m_section.empty()) {
			bool found = false;
			for (const Section& section : m_sections) {
				if (section.name == m_section) {
					found = true;
				} else {
					section.widget->setVisible(false);
				}
			}
			if (!found) {
				std::cerr << "ERROR: unknown section \"" << m_section << "\". Available sections:";
				for (const Section& section : m_sections) {
					std::cerr << " " << section.name;
				}
				std::cerr << std::endl;
			}
		}
	}

	void Application::onProcessWindowEvent(const glvx::Event& event) {
		if (event.type == glvx::EventType::Closed) {
			close();
		}
	}

	bool Application::saveScreenshot(const std::string& file_path) {
		for (int i = 0; i < SCREENSHOT_WARMUP_FRAMES; i++) {
			advance();
		}
		if (!window.saveScreenshot(file_path)) {
			std::cerr << "Failed to save screenshot to " << file_path << std::endl;
			return false;
		}
		return true;
	}

	fw::EmptyWidget* Application::createSection(const std::string& name, const std::string& label, float y, float height) {
		fw::EmptyWidget* section = getWidgets().createEmptyWidget();
		section->setName(name);
		section->setSize(SECTION_WIDTH, height);
		section->setPosition(0.0f, y);
		m_sections.push_back(Section{ name, section });
		createSectionLabel(label, y);
		return section;
	}

	fw::TextWidget* Application::createSectionLabel(const std::string& text, float y) {
		fw::TextWidget* label = getWidgets().createTextWidget();
		label->setCharacterSize(14);
		label->setFillColor(glvx::Color(210, 210, 210));
		label->setString(text);
		label->setPosition(16.0f, y + 6.0f);
		return label;
	}

	fw::TextWidget* Application::createLabel(fw::Widget* parent, const std::string& text, float x, float y, unsigned int character_size, const glvx::Color& color) {
		fw::TextWidget* label = getWidgets().createTextWidget();
		label->setParent(parent);
		label->setCharacterSize(character_size);
		label->setFillColor(color);
		label->setString(text);
		label->setPosition(x, y);
		return label;
	}

	void Application::setupTextShowcase(fw::Widget* parent) {
		auto add_text = [this, parent](float x, float y, const std::string& str, unsigned int size, const glvx::Color& color) {
			fw::TextWidget* text = getWidgets().createTextWidget();
			text->setParent(parent);
			text->setCharacterSize(size);
			text->setFillColor(color);
			text->setString(str);
			text->setPosition(x, y);
		};
		add_text(CONTENT_X, 22.0f, "TextWidget", 24, glvx::Color::White);
		add_text(CONTENT_X + 150.0f, 32.0f, "character size 16", 16, glvx::Color::White);
		add_text(CONTENT_X + 330.0f, 36.0f, "character size 12", 12, glvx::Color(180, 180, 180));
		add_text(CONTENT_X + 480.0f, 34.0f, "red", 14, glvx::Color(255, 64, 64));
		add_text(CONTENT_X + 540.0f, 34.0f, "green", 14, glvx::Color(64, 255, 64));
		add_text(CONTENT_X + 620.0f, 34.0f, "blue", 14, glvx::Color(64, 128, 255));
	}

	void Application::setupShapesShowcase(fw::Widget* parent) {
		fw::RectangleWidget* rect = getWidgets().createRectangleWidget(36.0f, 36.0f);
		rect->setParent(parent);
		rect->setFillColor(glvx::Color(200, 60, 60));
		rect->setPosition(CONTENT_X, 8.0f);

		fw::PolygonWidget* hexagon = getWidgets().createPolygonWidget(6, 18.0f);
		hexagon->setParent(parent);
		hexagon->setFillColor(glvx::Color(60, 120, 220));
		hexagon->setPosition(CONTENT_X + 60.0f, 32.0f);

		std::vector<glvx::Vector2f> triangle_vertices = {
			glvx::Vector2f(0.0f, -16.0f),
			glvx::Vector2f(14.0f, 12.0f),
			glvx::Vector2f(-14.0f, 12.0f),
		};
		fw::PolygonWidget* triangle = getWidgets().createPolygonWidget(triangle_vertices);
		triangle->setParent(parent);
		triangle->setFillColor(glvx::Color(220, 180, 40));
		triangle->setPosition(CONTENT_X + 130.0f, 32.0f);

		fw::RectangleWidget* rotated = getWidgets().createRectangleWidget(28.0f, 28.0f);
		rotated->setParent(parent);
		rotated->setFillColor(glvx::Color(60, 180, 120));
		rotated->setOrigin(fw::Widget::Anchor::CENTER);
		rotated->setPosition(CONTENT_X + 180.0f, 30.0f);
		rotated->setRotation(fw::to_radians(45.0f));

		createLabel(parent, "rectangle", CONTENT_X, 52.0f);
		createLabel(parent, "hexagon", CONTENT_X + 66.0f, 52.0f);
		createLabel(parent, "polygon", CONTENT_X + 120.0f, 52.0f);
		createLabel(parent, "rotated 45", CONTENT_X + 170.0f, 52.0f);
	}

	void Application::setupButtonShowcase(fw::Widget* parent) {
		fw::ButtonWidget* button = getWidgets().createButtonWidget(120.0f, 32.0f);
		button->setParent(parent);
		button->setPosition(CONTENT_X, 12.0f);
		fw::TextWidget* button_label = getWidgets().createTextWidget();
		button_label->setParent(button);
		button_label->setCharacterSize(14);
		button_label->setString("Click me");
		button_label->setPosition(38.0f, 9.0f);
		button->OnPress += [this] {
			m_button_press_count++;
			m_button_count_label->setString("pressed " + std::to_string(m_button_press_count) + " times");
		};
		m_button_count_label = createLabel(parent, "pressed 0 times", CONTENT_X + 140.0f, 19.0f, 14, glvx::Color::White);

		fw::ButtonWidget* custom_button = getWidgets().createButtonWidget(140.0f, 32.0f);
		custom_button->setParent(parent);
		custom_button->setPosition(CONTENT_X + 380.0f, 12.0f);
		custom_button->setNormalColor(glvx::Color(0, 110, 0));
		custom_button->setPressedColor(glvx::Color(200, 120, 0));
		fw::TextWidget* custom_label = getWidgets().createTextWidget();
		custom_label->setParent(custom_button);
		custom_label->setCharacterSize(14);
		custom_label->setString("custom colors");
		custom_label->setPosition(36.0f, 9.0f);
	}

	void Application::setupCheckboxShowcase(fw::Widget* parent) {
		fw::CheckboxWidget* checkbox = getWidgets().createCheckboxWidget();
		checkbox->setParent(parent);
		checkbox->setPosition(CONTENT_X, 18.0f);
		checkbox->OnValueChanged = [this](bool value) {
			m_checkbox_label->setString(value ? "checked" : "unchecked");
		};
		m_checkbox_label = createLabel(parent, "unchecked", CONTENT_X + 32.0f, 17.0f, 14, glvx::Color::White);

		fw::CheckboxWidget* checked_box = getWidgets().createCheckboxWidget();
		checked_box->setParent(parent);
		checked_box->setPosition(CONTENT_X + 220.0f, 18.0f);
		checked_box->setCheckFillColor(glvx::Color(0, 200, 255));
		checked_box->setValue(true);
		createLabel(parent, "pre-checked, custom check color", CONTENT_X + 252.0f, 17.0f, 14, glvx::Color::White);
	}

	void Application::setupDropdownShowcase(fw::Widget* parent) {
		fw::DropdownWidget* dropdown = getWidgets().createDropdownWidget();
		dropdown->setParent(parent);
		dropdown->setSize(140.0f, 24.0f);
		dropdown->setPosition(CONTENT_X, 16.0f);
		dropdown->setCharacterSize(14);
		dropdown->addOption("Red");
		dropdown->addOption("Green");
		dropdown->addOption("Blue");
		dropdown->addOption("Orange");
		dropdown->selectOption(0);
		dropdown->OnValueChanged = [this, dropdown](size_t index) {
			m_dropdown_label->setString("selected: " + dropdown->getOptionText(index));
		};
		m_dropdown_label = createLabel(parent, "selected: Red", CONTENT_X + 170.0f, 18.0f, 14, glvx::Color::White);
	}

	void Application::setupTextboxShowcase(fw::Widget* parent) {
		auto add_textbox = [this, parent](float x, const std::string& caption, fw::TextBoxWidget::TextBoxType type, const std::string& value, float width) {
			createLabel(parent, caption, x, 0.0f);
			fw::TextBoxWidget* textbox = getWidgets().createTextBoxWidget(width, 24.0f);
			textbox->setParent(parent);
			textbox->setType(type);
			textbox->setCharacterSize(14);
			textbox->setValue(value);
			textbox->setPosition(x, 18.0f);
			textbox->OnConfirm = [this](const std::string& confirmed) {
				m_textbox_label->setString("confirmed: " + confirmed);
			};
			return textbox;
		};
		add_textbox(CONTENT_X, "text", fw::TextBoxWidget::TextBoxType::TEXT, "Type here", 160.0f);
		add_textbox(CONTENT_X + 180.0f, "integer", fw::TextBoxWidget::TextBoxType::INTEGER, "42", 80.0f);
		add_textbox(CONTENT_X + 280.0f, "float", fw::TextBoxWidget::TextBoxType::FLOAT, "3.14", 100.0f);
		m_textbox_label = createLabel(parent, "confirmed: (none yet)", CONTENT_X + 400.0f, 21.0f, 14, glvx::Color::White);
	}

	void Application::setupLayoutShowcase(fw::Widget* parent) {
		// Vertical container with two horizontal rows: a fixed row of colored
		// cells and an EXPAND child filling the row width.
		fw::ContainerWidget* vertical = getWidgets().createContainerWidget(0.0f, 0.0f);
		vertical->setParent(parent);
		vertical->setHorizontal(false);
		vertical->setPadding(8.0f);
		vertical->setFillColor(glvx::Color(70, 74, 82));
		vertical->setPosition(CONTENT_X, 10.0f);

		fw::ContainerWidget* cell_row = getWidgets().createContainerWidget(0.0f, 0.0f);
		cell_row->setParent(vertical);
		cell_row->setPadding(4.0f);
		cell_row->setFillColor(glvx::Color(50, 54, 62));
		const glvx::Color cell_colors[] = { glvx::Color(200, 60, 60), glvx::Color(60, 160, 60), glvx::Color(60, 100, 220) };
		for (int i = 0; i < 3; i++) {
			fw::RectangleWidget* cell = getWidgets().createRectangleWidget(24.0f, 24.0f);
			cell->setParent(cell_row);
			cell->setFillColor(cell_colors[i]);
		}

		fw::RectangleWidget* expand_cell = getWidgets().createRectangleWidget(24.0f, 24.0f);
		expand_cell->setParent(vertical);
		expand_cell->setFillColor(glvx::Color(200, 160, 40));
		expand_cell->setMinSize(20.0f, 24.0f);
		expand_cell->setSizeXPolicy(fw::Widget::SizePolicy::EXPAND);

		createLabel(parent, "nested containers, EXPAND cell", CONTENT_X, 92.0f);

		// EXPAND with min/max size ceilings
		fw::ContainerWidget* expand_row = getWidgets().createContainerWidget(240.0f, 40.0f);
		expand_row->setParent(parent);
		expand_row->setSizeXPolicy(fw::Widget::SizePolicy::NONE);
		expand_row->setSizeYPolicy(fw::Widget::SizePolicy::NONE);
		expand_row->setFillColor(glvx::Color(70, 74, 82));
		expand_row->setPosition(CONTENT_X + 260.0f, 10.0f);
		fw::RectangleWidget* minmax_cell = getWidgets().createRectangleWidget(30.0f, 40.0f);
		minmax_cell->setParent(expand_row);
		minmax_cell->setFillColor(glvx::Color(200, 60, 60));
		minmax_cell->setMinSize(30.0f, 40.0f);
		minmax_cell->setMaxSize(70.0f, 40.0f);
		minmax_cell->setSizeXPolicy(fw::Widget::SizePolicy::EXPAND);
		fw::RectangleWidget* fill_cell = getWidgets().createRectangleWidget(20.0f, 40.0f);
		fill_cell->setParent(expand_row);
		fill_cell->setFillColor(glvx::Color(60, 100, 220));
		fill_cell->setMinSize(20.0f, 40.0f);
		fill_cell->setSizeXPolicy(fw::Widget::SizePolicy::EXPAND);
		createLabel(parent, "EXPAND cells, min 30 / max 70 vs min 20", CONTENT_X + 260.0f, 92.0f);

		// Parent anchors with offset
		fw::RectangleWidget* anchor_box = getWidgets().createRectangleWidget(44.0f, 28.0f);
		anchor_box->setParent(parent);
		anchor_box->setFillColor(glvx::Color(90, 94, 104));
		anchor_box->setPosition(CONTENT_X + 540.0f, 14.0f);
		fw::RectangleWidget* anchored = getWidgets().createRectangleWidget(20.0f, 16.0f);
		anchored->setParent(anchor_box);
		anchored->setFillColor(glvx::Color(255, 255, 0));
		anchored->setParentAnchor(fw::Widget::Anchor::BOTTOM_RIGHT);
		anchored->setAnchorOffset(-4.0f, -4.0f);
		fw::RectangleWidget* center_box = getWidgets().createRectangleWidget(44.0f, 28.0f);
		center_box->setParent(parent);
		center_box->setFillColor(glvx::Color(90, 94, 104));
		center_box->setPosition(CONTENT_X + 600.0f, 14.0f);
		fw::RectangleWidget* centered = getWidgets().createRectangleWidget(16.0f, 16.0f);
		centered->setParent(center_box);
		centered->setFillColor(glvx::Color(0, 200, 255));
		centered->setParentAnchor(fw::Widget::Anchor::CENTER);
		createLabel(parent, "anchors: offset (-4, -4) / center", CONTENT_X + 520.0f, 92.0f);

		// Rotation around different origins
		fw::RectangleWidget* corner_rotated = getWidgets().createRectangleWidget(32.0f, 20.0f);
		corner_rotated->setParent(parent);
		corner_rotated->setFillColor(glvx::Color(200, 100, 200));
		corner_rotated->setPosition(CONTENT_X + 760.0f, 18.0f);
		corner_rotated->setRotation(fw::to_radians(30.0f));
		fw::RectangleWidget* center_rotated = getWidgets().createRectangleWidget(32.0f, 20.0f);
		center_rotated->setParent(parent);
		center_rotated->setFillColor(glvx::Color(200, 100, 200));
		center_rotated->setOrigin(fw::Widget::Anchor::CENTER);
		center_rotated->setPosition(CONTENT_X + 840.0f, 28.0f);
		center_rotated->setRotation(fw::to_radians(30.0f));
		createLabel(parent, "corner origin", CONTENT_X + 745.0f, 92.0f);
		createLabel(parent, "center origin", CONTENT_X + 835.0f, 92.0f);
	}

	void Application::setupScrollAreaShowcase(fw::Widget* parent) {
		fw::ScrollAreaWidget* scroll_area = getWidgets().createScrollAreaWidget(260.0f, 76.0f);
		scroll_area->setParent(parent);
		scroll_area->setPosition(CONTENT_X, 6.0f);
		scroll_area->setBackgroundColor(glvx::Color(60, 64, 72));

		fw::EmptyWidget* content = getWidgets().createEmptyWidget();
		content->setSize(560.0f, 240.0f);
		const glvx::Color cell_colors[] = {
			glvx::Color(180, 70, 70), glvx::Color(70, 150, 70), glvx::Color(70, 100, 190),
			glvx::Color(190, 150, 50), glvx::Color(150, 80, 150), glvx::Color(80, 150, 170),
		};
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 3; col++) {
				fw::RectangleWidget* cell = getWidgets().createRectangleWidget(150.0f, 60.0f);
				cell->setParent(content);
				cell->setFillColor(cell_colors[row * 3 + col]);
				cell->setPosition(16.0f + col * 165.0f, 16.0f + row * 96.0f);
				createLabel(content, "cell " + std::to_string(row * 3 + col), 16.0f + col * 165.0f + 52.0f, 16.0f + row * 96.0f + 24.0f, 12, glvx::Color::White);
			}
		}
		scroll_area->setScrolledWidget(content);

		createLabel(parent, "ScrollAreaWidget: wheel + slider drag,\nscrollbar policy SIZE (auto-shown)", CONTENT_X + 280.0f, 16.0f, 12);
	}

	void Application::setupTreeViewShowcase(fw::Widget* parent) {
		fw::TreeViewWidget* tree_view = getWidgets().createTreeViewWidget(280.0f, 132.0f);
		tree_view->setParent(parent);
		tree_view->setPosition(CONTENT_X, 4.0f);
		fw::TreeViewEntry* alpha = tree_view->addEntry("Alpha");
		fw::TreeViewEntry* beta = tree_view->addEntry("Beta");
		fw::TreeViewEntry* gamma = tree_view->addEntry("Gamma");
		fw::TreeViewEntry* delta = tree_view->addEntry("Delta");
		fw::TreeViewEntry* epsilon = tree_view->addEntry("Epsilon");
		fw::TreeViewEntry* zeta = tree_view->addEntry("Zeta");
		beta->setParent(alpha);
		gamma->setParent(beta);
		delta->setParent(alpha);
		zeta->setParent(epsilon);
		alpha->expand();
		beta->expand();
		gamma->select();
		tree_view->OnEntryClicked += [this](fw::TreeViewEntry* entry) {
			m_tree_label->setString("clicked: " + treeEntryFullName(entry));
		};
		m_tree_label = createLabel(parent, "clicked: (none yet)", CONTENT_X + 300.0f, 10.0f, 14, glvx::Color::White);
		createLabel(parent, "arrows expand / collapse,\nclick to select, drag to move entries", CONTENT_X + 300.0f, 44.0f, 12);
	}

	void Application::setupCanvasShowcase(fw::Widget* parent) {
		fw::CanvasWidget* canvas = getWidgets().createCanvasWidget(220.0f, 68.0f, 220, 68);
		canvas->setParent(parent);
		canvas->setPosition(CONTENT_X, 6.0f);
		canvas->clear(glvx::Color(32, 32, 64));
		fw::draw_line(canvas, glvx::Vector2f(12.0f, 56.0f), glvx::Vector2f(70.0f, 16.0f), glvx::Color::White);
		fw::draw_line(canvas, glvx::Vector2f(70.0f, 16.0f), glvx::Vector2f(128.0f, 44.0f), glvx::Color(255, 255, 0));
		fw::draw_rect(canvas->getRenderTexture(), glvx::Vector2f(150.0f, 14.0f), glvx::Vector2f(48.0f, 36.0f), glvx::Color(0, 128, 255));
		createLabel(parent, "CanvasWidget: direct drawing\ninto its render texture", CONTENT_X + 240.0f, 16.0f, 12);
	}

	void Application::setupWindowShowcase(fw::Widget* parent) {
		fw::WindowWidget* window_widget = getWidgets().createWindowWidget(240.0f, 96.0f);
		window_widget->setParent(parent);
		window_widget->setPosition(CONTENT_X, 4.0f);
		window_widget->setHeaderText("Mini window");
		window_widget->setHeaderColor(glvx::Color(70, 130, 180));
		window_widget->setOutlineColor(glvx::Color(220, 220, 220));
		fw::RectangleWidget* window_child = getWidgets().createRectangleWidget(60.0f, 36.0f);
		window_child->setParent(window_widget);
		window_child->setFillColor(glvx::Color(200, 160, 40));
		window_child->setPosition(24.0f, 44.0f);
		createLabel(window_widget, "drag the header,\nresize at the edges", 104.0f, 40.0f, 12, glvx::Color::White);
		createLabel(parent, "WindowWidget: draggable + resizable\nin-window window with header", CONTENT_X + 270.0f, 20.0f, 12);
	}

}
