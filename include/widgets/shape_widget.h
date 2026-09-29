#pragma once

#include "widget.h"

namespace fw {

	class WidgetList;

	class ShapeWidget : public Widget {
	public:
		ShapeWidget(WidgetList& widget_list);
		virtual const glvx::Color& getFillColor() const override;
		virtual void setFillColor(const glvx::Color& color) override;
		void setOutlineColor(const glvx::Color& color);
		void setOutlineThickness(float thickness);

	protected:
		virtual glvx::Shape& getShape() = 0;
		virtual const glvx::Shape& getShape() const = 0;
		virtual glvx::FloatRect getShapeLocalBounds() const = 0;

	private:
		glvx::Color fill_color = glvx::Color::White;
		// GLVX shapes have no outline support; values are stored only
		glvx::Color outline_color = glvx::Color::Black;
		float outline_thickness = 0.0f;
	};

}
