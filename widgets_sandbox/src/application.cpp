#include "application.h"
#include <iostream>

namespace sandbox {

	Application::Application() : m_font("fonts/verdana.ttf") {
	}

	void Application::onInit() {
		m_text_widget = getWidgets().createTextWidget();
		m_text_widget->setFont(m_font);
		m_text_widget->setCharacterSize(20);
		m_text_widget->setString("Hello, widgets!");
		m_text_widget->setPosition(10.0f, 10.0f);
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

}
