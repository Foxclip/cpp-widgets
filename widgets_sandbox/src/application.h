#pragma once

#include <string>
#include <vector>
#include "widgets/widgets.h"

namespace sandbox {

	class Application : public fw::Application {
	public:
		static const int WINDOW_WIDTH = 1280;
		static const int WINDOW_HEIGHT = 1000;

		explicit Application(const std::string& section, bool show_fps = true, bool debug_render = false);
		bool saveScreenshot(const std::string& file_path);
		// Advance frames for the given duration (after a short warmup) and
		// print the measured average FPS to stdout. For performance checks.
		void reportFps(double duration_seconds);

	protected:
		void onInit() override;
		void onFrameBegin() override;
		void onProcessWindowEvent(const glvx::Event& event) override;

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
		// FPS counter HUD
		static constexpr float FPS_LABEL_Y = 10.0f;
		static constexpr float FPS_LABEL_MARGIN_X = 12.0f;
		static constexpr unsigned int FPS_LABEL_CHAR_SIZE = 14;
		static constexpr float FPS_LABEL_UPDATE_INTERVAL = 0.25f; // seconds between HUD updates

		bool m_show_fps;
		bool m_debug_render;
		fw::TextWidget* m_fps_label = nullptr;
		double m_fps_last_time = 0.0;
		double m_fps_accumulated = 0.0;
		int m_fps_frames = 0;

		std::string m_section;
		glvx::Font m_font;
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
		void updateFpsLabelPosition();
	};

}
