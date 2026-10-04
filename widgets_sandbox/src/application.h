#pragma once

#include <string>
#include "widgets/widgets.h"

namespace sandbox {

	class Application : public fw::Application {
	public:
		Application();
		bool saveScreenshot(const std::string& file_path);

	protected:
		void onInit() override;
		void onProcessWindowEvent(const glvx::Event& event) override;

	private:
		// Frames rendered before a screenshot is taken so that the first
		// frame is not captured with an uninitialized render texture.
		static const int SCREENSHOT_WARMUP_FRAMES = 2;
		fw::Font m_font;
		fw::TextWidget* m_text_widget = nullptr;
	};

}
