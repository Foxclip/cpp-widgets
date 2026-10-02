#pragma once

#include "widgets/shape_widget.h"

namespace fw {

	class WidgetList;

	class RectangleWidget : public ShapeWidget {
	public:
		RectangleWidget(WidgetList& widget_list, float width, float height);
		RectangleWidget(WidgetList& widget_list, const glvx::Vector2f& size);
		RectangleWidget* clone(bool with_children = true) override;
		glvx::FloatRect getLocalBounds() const override;

	protected:
		glvx::Rectangle rect{0.0f, 0.0f};

		glvx::Drawable* getDrawable() override;
		const glvx::Drawable* getDrawable() const override;
		glvx::Transformable* getTransformable() override;
		const glvx::Transformable* getTransformable() const override;
		glvx::Shape& getShape() override;
		const glvx::Shape& getShape() const override;
		glvx::FloatRect getShapeLocalBounds() const override;
		void setSizeInternal(float width, float height) override;

	private:
		friend class WindowWidget;

	};

}
