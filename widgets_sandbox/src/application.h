#pragma once

#include <string>
#include <vector>
#include "widgets/widgets.h"

namespace sandbox {

	class Application : public fw::Application {
	public:
		static const int WINDOW_WIDTH = 1280;
		static const int WINDOW_HEIGHT = 1000;

		explicit Application(const std::string& section);
		bool saveScreenshot(const std::string& file_path);

	protected:
		void onInit() override;
		void onProcessWindowEvent(const glvx::Event& event) override;
		void onProcessKeyboardEvent(const glvx::Event& event) override;

	private:
		struct Section {
			std::string name;
			fw::EmptyWidget* widget = nullptr;
		};

		// Frames rendered before a screenshot is taken so that the first
		// frame is not captured with an uninitialized render texture.
		static constexpr int SCREENSHOT_WARMUP_FRAMES = 2;
		static constexpr float SECTION_WIDTH = 1280.0f;
		static constexpr float CONTENT_X = 180.0f;

		std::string m_section;
		fw::Font m_font;
		std::vector<Section> m_sections;

		// Live labels updated by widget callbacks
		fw::TextWidget* m_button_count_label = nullptr;
		fw::TextWidget* m_checkbox_label = nullptr;
		fw::TextWidget* m_dropdown_label = nullptr;
		fw::TextWidget* m_textbox_label = nullptr;
		fw::TextWidget* m_tree_label = nullptr;
		int m_button_press_count = 0;

		fw::EmptyWidget* createSection(const std::string& name, const std::string& label, float y, float height);
		fw::TextWidget* createSectionLabel(const std::string& text, float y);
		fw::TextWidget* createLabel(fw::Widget* parent, const std::string& text, float x, float y, unsigned int character_size = 10, const glvx::Color& color = glvx::Color(170, 170, 170));
		void setupTextShowcase(fw::Widget* parent);
		void setupShapesShowcase(fw::Widget* parent);
		void setupButtonShowcase(fw::Widget* parent);
		void setupCheckboxShowcase(fw::Widget* parent);
		void setupDropdownShowcase(fw::Widget* parent);
		void setupTextboxShowcase(fw::Widget* parent);
		void setupLayoutShowcase(fw::Widget* parent);
		void setupScrollAreaShowcase(fw::Widget* parent);
		void setupTreeViewShowcase(fw::Widget* parent);
		void setupCanvasShowcase(fw::Widget* parent);
		void setupWindowShowcase(fw::Widget* parent);
	};

}
