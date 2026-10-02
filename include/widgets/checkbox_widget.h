#pragma once

#include "widgets/rectangle_widget.h"

namespace fw {

	const glvx::Vector2f CHECKBOX_DEFAULT_SIZE = glvx::Vector2f(20.0f, 20.0f);

	class WidgetList;

	class CheckboxWidget : public RectangleWidget {
	public:
		std::function<void(bool)> OnValueChanged = [](bool new_value) { };

		CheckboxWidget(WidgetList& widget_list);
		bool getValue() const;
		const glvx::Color& getFillColor() const override;
		const glvx::Color& getHighlightFillColor() const;
		const glvx::Color& getCheckFillColor() const;
		void setFillColor(const glvx::Color& color) override;
		void setHighlightFillColor(const glvx::Color& color);
		void setCheckFillColor(const glvx::Color& color);
		void setValueSilent(bool value);
		void setValue(bool value);
		void toggleValue();
		CheckboxWidget* clone(bool with_children = true) override;

	protected:
		void internalOnLeftPress(const glvx::Vector2f& pos, bool became_focused) override;
		void internalOnMouseEnter(const glvx::Vector2f& pos) override;
		void internalOnMouseExit(const glvx::Vector2f& pos) override;

	private:
		bool checked = false;
		float check_size = 0.6f;
		RectangleWidget* check_widget = nullptr;
		glvx::Color background_fill_color = glvx::Color(50, 50, 50);
		glvx::Color highlight_fill_color = glvx::Color(100, 100, 100);
		glvx::Color check_fill_color = glvx::Color(255, 128, 0);

	};

}
