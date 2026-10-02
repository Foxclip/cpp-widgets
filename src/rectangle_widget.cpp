#include "widgets/rectangle_widget.h"
#include "widgets/widget_list.h"

namespace fw {

	RectangleWidget::RectangleWidget(WidgetList& widget_list, float width, float height) : ShapeWidget(widget_list) {
		type = WidgetType::Rectangle;
		setName("rectangle");
		setSize(width, height);
	}

	RectangleWidget::RectangleWidget(WidgetList& widget_list, const glvx::Vector2f& size)
		: RectangleWidget(widget_list, size.x, size.y) { }

	glvx::FloatRect RectangleWidget::getLocalBounds() const {
		return getShapeLocalBounds();
	}

	glvx::Drawable* RectangleWidget::getDrawable() {
		return &rect;
	}

	const glvx::Drawable* RectangleWidget::getDrawable() const {
		return &rect;
	}

	glvx::Transformable* RectangleWidget::getTransformable() {
		return &rect;
	}

	const glvx::Transformable* RectangleWidget::getTransformable() const {
		return &rect;
	}

	glvx::Shape& RectangleWidget::getShape() {
		return rect;
	}

	const glvx::Shape& RectangleWidget::getShape() const {
		return rect;
	}

	glvx::FloatRect RectangleWidget::getShapeLocalBounds() const {
		return glvx::FloatRect(glvx::Vector2f(), rect.getSize());
	}

	void RectangleWidget::setSizeInternal(float width, float height) {
		rect.setSize(glvx::Vector2f(width, height));
		updateOrigin();
	}

	RectangleWidget* RectangleWidget::clone(bool with_children) {
		return widget_list.duplicateWidget(this, with_children);
	}

}
