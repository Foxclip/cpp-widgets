#include "widgets/shape_widget.h"
#include "widgets/widget_list.h"

namespace fw {

	ShapeWidget::ShapeWidget(WidgetList& widget_list) : Widget(widget_list) { }

	const glvx::Color& ShapeWidget::getFillColor() const {
		return fill_color;
	}

	void ShapeWidget::setFillColor(const glvx::Color& color) {
		fill_color = color;
		getShape().setColor(color);
	}

	void ShapeWidget::setOutlineColor(const glvx::Color& color) {
		// no-op: GLVX shapes have no outline support
		outline_color = color;
	}

	void ShapeWidget::setOutlineThickness(float thickness) {
		// no-op: GLVX shapes have no outline support
		outline_thickness = thickness;
	}

}
