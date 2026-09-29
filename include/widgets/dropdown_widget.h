#pragma once

#include "container_widget.h"
#include "empty_widget.h"
#include "rectangle_widget.h"
#include "text_widget.h"
#include "polygon_widget.h"

namespace fw {

	const glvx::Vector2f DROPDOWN_DEFAULT_SIZE = glvx::Vector2f(100.0f, 20.0f);
	const glvx::Color DROPDOWN_DEFAULT_MAIN_COLOR = glvx::Color(128, 128, 128);
	const glvx::Color DROPDOWN_DEFAULT_OPTION_HOVER_BACKGROUND_COLOR = glvx::Color(150, 150, 150);
	const glvx::Color DROPDOWN_DEFAULT_TRIANGLE_COLOR = glvx::Color(255, 255, 255);
	const glvx::Color DROPDOWN_DEFAULT_SQUARE_COLOR = glvx::Color(150, 150, 150);
	const glvx::Color DROPDOWN_DEFAULT_PANEL_COLOR = glvx::Color(128, 128, 128);
	const glvx::Color DROPDOWN_DEFAULT_MAIN_TEXT_COLOR = glvx::Color(255, 255, 255);
	const glvx::Color DROPDOWN_DEFAULT_PANEL_TEXT_COLOR = glvx::Color(255, 255, 255);

	class WidgetList;

	class DropdownWidget : public EmptyWidget {
	public:
		std::function<void(size_t index)> OnValueChanged = [](ptrdiff_t index) { };

		DropdownWidget(WidgetList& widget_list);
		const glvx::Color& getMainBackgroundColor() const;
		const glvx::Color& getOptionHoverBackgroundColor() const;
		const glvx::Color& getTriangleColor() const;
		const glvx::Color& getSquareColor() const;
		const glvx::Color& getPanelBackgroundColor() const;
		const glvx::Color& getMainTextColor() const;
		const glvx::Color& getPanelTextColor() const;
		RectangleWidget* getMainWidget() const;
		RectangleWidget* getPanelWidget() const;
		RectangleWidget* getOptionWidget(size_t index) const;
		TextWidget* getOptionTextWidget(size_t index) const;
		const std::string& getOptionText(size_t index) const;
		ptrdiff_t getValue() const;
		bool isPanelOpen() const;
		using Widget::setSize;
		void setSize(float width, float height) override;
		void setMainBackgroundColor(const glvx::Color& color);
		void setOptionHoverBackgroundColor(const glvx::Color& color);
		void setTriangleColor(const glvx::Color& color);
		void setSquareColor(const glvx::Color& color);
		void setPanelBackgroundColor(const glvx::Color& color);
		void setMainTextColor(const glvx::Color& color);
		void setPanelTextColor(const glvx::Color& color);
		void setFont(const fw::Font& font);
		void setCharacterSize(unsigned int size);
		void setTextAnchor(Anchor anchor);
		void setTextOriginAnchor(Anchor anchor);
		void showPanel();
		void hidePanel();
		void togglePanel();
		void addOption(const std::string& text, ptrdiff_t index = -1);
		void selectOption(size_t index);
		void setOptionText(size_t index, const std::string& text);
		void removeOption(size_t index);
		void removeOption(const std::string& text);
		DropdownWidget* clone(bool with_children = true) override;

	protected:
		RectangleWidget* main_widget = nullptr;
		TextWidget* text_widget = nullptr;
		PolygonWidget* triangle_widget = nullptr;
		RectangleWidget* square_widget = nullptr;
		RectangleWidget* panel_widget = nullptr;
		CompVector<RectangleWidget*> option_widgets;

	private:
		ptrdiff_t selected = -1;
		glvx::Color option_hover_background_color = DROPDOWN_DEFAULT_OPTION_HOVER_BACKGROUND_COLOR;
		glvx::Color panel_text_color = DROPDOWN_DEFAULT_PANEL_TEXT_COLOR;

		void updateOptions();
		void updateOptionSize();

	};

}
