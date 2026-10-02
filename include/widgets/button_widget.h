#pragma once

#include "widgets/rectangle_widget.h"

namespace fw {

	const glvx::Color BUTTON_DEFAULT_NORMAL_COLOR = glvx::Color(128, 128, 128);
	const glvx::Color BUTTON_DEFAULT_PRESSED_COLOR = glvx::Color(255, 255, 0);

	class WidgetList;

	class ButtonWidget : public RectangleWidget {
	public:
		Event<> OnPress;
		Event<> OnRelease;

		ButtonWidget(WidgetList& widget_list, float width, float height);
		ButtonWidget(WidgetList& widget_list, const glvx::Vector2f& size);
		bool isPressed() const;
		ButtonWidget* clone(bool with_children = true) override;
		void setNormalColor(const glvx::Color& color);
		void setPressedColor(const glvx::Color& color);

	protected:
		glvx::Color normal_color = BUTTON_DEFAULT_NORMAL_COLOR;
		glvx::Color pressed_color = BUTTON_DEFAULT_PRESSED_COLOR;
		bool pressed = false;

		void internalOnLeftPress(const glvx::Vector2f& pos, bool became_focused) override;
		void internalOnGlobalLeftRelease(const glvx::Vector2f& pos) override;
		void updateColors();

	private:
		friend class WindowWidget;

	};

}
