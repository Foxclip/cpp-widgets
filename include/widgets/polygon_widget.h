#pragma once

#include "shape_widget.h"

namespace fw {

	class WidgetList;

	class PolygonWidget : public ShapeWidget {
	public:
		PolygonWidget(WidgetList& widget_list, const std::vector<glvx::Vector2f>& vertices);
		PolygonWidget(WidgetList& widget_list, size_t vertex_count, float radius = 10.0f, float angle_offset = 0.0f);
		const std::vector<glvx::Vector2f>& getVertices() const;
		void setVertices(const std::vector<glvx::Vector2f>& vertices);
		PolygonWidget* clone(bool with_children = true) override;
		glvx::FloatRect getLocalBounds() const override;

	protected:
		glvx::Shape polygon;
		std::vector<glvx::Vector2f> vertices;

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

		void syncVertices();

	};

}
