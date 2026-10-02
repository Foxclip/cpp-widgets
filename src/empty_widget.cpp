#include "widgets/empty_widget.h"
#include "widgets/widget_list.h"

namespace fw {

	EmptyWidget::EmptyWidget(WidgetList& widget_list) : Widget(widget_list) {
		type = WidgetType::Empty;
		setName("empty");
		setRenderable(false);
	}

	glvx::FloatRect EmptyWidget::getLocalBounds() const {
		return glvx::FloatRect(glvx::Vector2f(), size);
	}

	const glvx::Color& EmptyWidget::getFillColor() const {
		return glvx::Color::Transparent;
	}

	void EmptyWidget::setFillColor(const glvx::Color& color) { }

	void EmptyWidget::setRenderable(bool value) {
		if (value) {
			wAssert(false, "Cannot set EmptyWidget as renderable");
		} else {
			Widget::setRenderable(value);
		}

	}

	EmptyWidget* EmptyWidget::clone(bool with_children) {
		return widget_list.duplicateWidget(this, with_children);
	}

	glvx::Drawable* EmptyWidget::getDrawable() {
		return nullptr;
	}

	const glvx::Drawable* EmptyWidget::getDrawable() const {
		return nullptr;
	}

	glvx::Transformable* EmptyWidget::getTransformable() {
		return nullptr;
	}

	const glvx::Transformable* EmptyWidget::getTransformable() const {
		return nullptr;
	}

	void EmptyWidget::setSizeInternal(float width, float height) {
		this->size.x = width;
		this->size.y = height;
		updateOrigin();
	}

}
