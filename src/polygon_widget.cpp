#include "widgets/polygon_widget.h"
#include "widgets/widget_list.h"

namespace fw {

	PolygonWidget::PolygonWidget(WidgetList& widget_list, const std::vector<glvx::Vector2f>& vertices) : ShapeWidget(widget_list) {
		type = WidgetType::Polygon;
		setName("polygon");
		setVertices(vertices);
	}

	PolygonWidget::PolygonWidget(WidgetList& widget_list, size_t vertex_count, float radius, float angle_offset) : ShapeWidget(widget_list) {
		type = WidgetType::Polygon;
		setName("polygon");
		std::vector<glvx::Vector2f> vertices = get_regular_polygon<glvx::Vector2f>(vertex_count, radius, angle_offset);
		setVertices(vertices);
	}

	const std::vector<glvx::Vector2f>& PolygonWidget::getVertices() const {
		return vertices;
	}

	glvx::FloatRect PolygonWidget::getLocalBounds() const {
		return getShapeLocalBounds();
	}

	void PolygonWidget::setVertices(const std::vector<glvx::Vector2f>& vertices) {
		this->vertices = vertices;
		syncVertices();
	}

	PolygonWidget* PolygonWidget::clone(bool with_children) {
		return widget_list.duplicateWidget(this, with_children);
	}

	glvx::Drawable* PolygonWidget::getDrawable() {
		return &polygon;
	}

	const glvx::Drawable* PolygonWidget::getDrawable() const {
		return &polygon;
	}

	glvx::Transformable* PolygonWidget::getTransformable() {
		return &polygon;
	}

	const glvx::Transformable* PolygonWidget::getTransformable() const {
		return &polygon;
	}

	glvx::Shape& PolygonWidget::getShape() {
		return polygon;
	}

	const glvx::Shape& PolygonWidget::getShape() const {
		return polygon;
	}

	void PolygonWidget::setSizeInternal(float width, float height) {
		// nothing
	}

	glvx::FloatRect PolygonWidget::getShapeLocalBounds() const {
		glvx::FloatRect result;
		bool first = true;
		for (size_t i = 0; i < vertices.size(); i++) {
			glvx::FloatRect point_rect(glvx::Vector2f(vertices[i].x, vertices[i].y), glvx::Vector2f());
			if (first) {
				result = point_rect;
				first = false;
			} else {
				result.extend(point_rect);
			}
		}
		return result;
	}

	void PolygonWidget::syncVertices() {
		polygon.setPrimitiveType(glvx::PrimitiveType::TriangleFan);
		polygon.resize((unsigned int)vertices.size());
		for (size_t i = 0; i < vertices.size(); i++) {
			polygon[i].position = vertices[i];
			polygon[i].color = glvx::Color::White;
		}
	}

}
