#pragma once

#include "widgets/widget.h"

namespace fw {

	class WidgetList;

	class EmptyWidget : public Widget {
	public:
		EmptyWidget(WidgetList& widget_list);
		glvx::FloatRect getLocalBounds() const override;
		const glvx::Color& getFillColor() const override;
		void setFillColor(const glvx::Color& color) override;
		void setRenderable(bool value) override;
		EmptyWidget* clone(bool with_children = true) override;

	protected:
		glvx::Drawable* getDrawable() override;
		const glvx::Drawable* getDrawable() const override;
		glvx::Transformable* getTransformable() override;
		const glvx::Transformable* getTransformable() const override;
		void setSizeInternal(float width, float height) override;

	private:
		glvx::Vector2f size;

	};

}
