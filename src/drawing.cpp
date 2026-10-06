#include "widgets/drawing.h"
#include "widgets/canvas_widget.h"

namespace fw {

	// Shared primitives, created once. The vertex arrays are plain CPU-side
	// vertex storage (the GL buffers are uploaded on first draw), so reusing
	// them across calls is safe. NOTE: their initial color comes from the
	// Drawable base (glvx::Color::White is the default), which is why the
	// Color constants had to be constant-initialized (see GLVX color.h) —
	// a dynamically-initialized Color::White raced against these statics in
	// Debug builds and made the lines render with a (0,0,0,0) color.
	namespace {
		glvx::VertexArray line_primitive(glvx::PrimitiveType::Lines, 2);
		glvx::VertexArray rect_primitive(glvx::PrimitiveType::TriangleStrip, 4);
	}

	void draw_line(
		glvx::RenderTarget& target,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color
	) {
		line_primitive[0].position = quantize_and_offset(v1);
		line_primitive[0].color = color;
		line_primitive[1].position = quantize_and_offset(v2);
		line_primitive[1].color = color;
		target.draw(line_primitive);
	}

	void draw_line(
		CanvasWidget* canvas,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color
	) {
		line_primitive[0].position = quantize_and_offset(v1);
		line_primitive[0].color = color;
		line_primitive[1].position = quantize_and_offset(v2);
		line_primitive[1].color = color;
		canvas->draw(line_primitive);
	}

	void draw_line(
		glvx::RenderTarget& target,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color,
		const glvx::Transform& transform
	) {
		line_primitive[0].position = quantize_and_offset(transform.transformPoint(v1));
		line_primitive[0].color = color;
		line_primitive[1].position = quantize_and_offset(transform.transformPoint(v2));
		line_primitive[1].color = color;
		target.draw(line_primitive);
	}

	void draw_line(
		CanvasWidget* canvas,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color,
		const glvx::Transform& transform
	) {
		line_primitive[0].position = quantize_and_offset(transform.transformPoint(v1));
		line_primitive[0].color = color;
		line_primitive[1].position = quantize_and_offset(transform.transformPoint(v2));
		line_primitive[1].color = color;
		canvas->draw(line_primitive);
	}

	void draw_rect(
		glvx::RenderTarget& target,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Vector2f& v3,
		const glvx::Vector2f& v4,
		const glvx::Color& color
	) {
		rect_primitive[0].position = quantize(v1);
		rect_primitive[0].color = color;
		rect_primitive[1].position = quantize(v2);
		rect_primitive[1].color = color;
		rect_primitive[2].position = quantize(v3);
		rect_primitive[2].color = color;
		rect_primitive[3].position = quantize(v4);
		rect_primitive[3].color = color;
		target.draw(rect_primitive);
	}

	void draw_rect(glvx::RenderTarget& target,
		const glvx::Vector2f& pos,
		const glvx::Vector2f& size,
		const glvx::Color& color
	) {
		glvx::Vector2f v1 = pos;
		glvx::Vector2f v2 = glvx::Vector2f(pos.x + size.x, pos.y);
		glvx::Vector2f v3 = glvx::Vector2f(pos.x, pos.y + size.y);
		glvx::Vector2f v4 = glvx::Vector2f(pos.x + size.x, pos.y + size.y);
		draw_rect(target, v1, v2, v3, v4, color);
	}

	void draw_wire_rect(
		glvx::RenderTarget& target,
		const glvx::FloatRect& bounds,
		const glvx::Color& color
	) {
		glvx::Vector2f topRight(bounds.position.x + bounds.size.x, bounds.position.y);
		glvx::Vector2f topLeft(bounds.position.x, bounds.position.y);
		glvx::Vector2f bottomLeft(bounds.position.x, bounds.position.y + bounds.size.y);
		glvx::Vector2f bottomRight(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
		draw_line(target, topRight, topLeft, color);
		draw_line(target, topLeft, bottomLeft, color);
		draw_line(target, bottomLeft, bottomRight, color);
		draw_line(target, bottomRight, topRight, color);
	}

	void draw_wire_rect(
		CanvasWidget* canvas,
		const glvx::FloatRect& bounds,
		const glvx::Color& color
	) {
		glvx::Vector2f topRight(bounds.position.x + bounds.size.x, bounds.position.y);
		glvx::Vector2f topLeft(bounds.position.x, bounds.position.y);
		glvx::Vector2f bottomLeft(bounds.position.x, bounds.position.y + bounds.size.y);
		glvx::Vector2f bottomRight(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
		draw_line(canvas, topRight, topLeft, color);
		draw_line(canvas, topLeft, bottomLeft, color);
		draw_line(canvas, bottomLeft, bottomRight, color);
		draw_line(canvas, bottomRight, topRight, color);
	}

	void draw_wire_rect(
		glvx::RenderTarget& target,
		const glvx::FloatRect& bounds,
		const glvx::Color& color,
		const glvx::Transform& transform
	) {
		glvx::Vector2f topRight(bounds.position.x + bounds.size.x, bounds.position.y);
		glvx::Vector2f topLeft(bounds.position.x, bounds.position.y);
		glvx::Vector2f bottomLeft(bounds.position.x, bounds.position.y + bounds.size.y);
		glvx::Vector2f bottomRight(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
		draw_line(target, topRight, topLeft, color, transform);
		draw_line(target, topLeft, bottomLeft, color, transform);
		draw_line(target, bottomLeft, bottomRight, color, transform);
		draw_line(target, bottomRight, topRight, color, transform);
	}

	void draw_wire_rect(
		CanvasWidget* canvas,
		const glvx::FloatRect& bounds,
		const glvx::Color& color,
		const glvx::Transform& transform
	) {
		glvx::Vector2f topRight(bounds.position.x + bounds.size.x, bounds.position.y);
		glvx::Vector2f topLeft(bounds.position.x, bounds.position.y);
		glvx::Vector2f bottomLeft(bounds.position.x, bounds.position.y + bounds.size.y);
		glvx::Vector2f bottomRight(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
		draw_line(canvas, topRight, topLeft, color, transform);
		draw_line(canvas, topLeft, bottomLeft, color, transform);
		draw_line(canvas, bottomLeft, bottomRight, color, transform);
		draw_line(canvas, bottomRight, topRight, color, transform);
	}

	void LineBatch::line(const glvx::Vector2f& v1, const glvx::Vector2f& v2, const glvx::Color& color) {
		m_vertices.pushBack(glvx::Vertex{quantize_and_offset(v1), color, glvx::Vector2f(0.0f, 0.0f)});
		m_vertices.pushBack(glvx::Vertex{quantize_and_offset(v2), color, glvx::Vector2f(0.0f, 0.0f)});
	}

	void LineBatch::line(const glvx::Vector2f& v1, const glvx::Vector2f& v2, const glvx::Color& color, const glvx::Transform& transform) {
		line(transform.transformPoint(v1), transform.transformPoint(v2), color);
	}

	void LineBatch::rect(const glvx::FloatRect& bounds, const glvx::Color& color) {
		glvx::Vector2f topRight(bounds.position.x + bounds.size.x, bounds.position.y);
		glvx::Vector2f topLeft(bounds.position.x, bounds.position.y);
		glvx::Vector2f bottomLeft(bounds.position.x, bounds.position.y + bounds.size.y);
		glvx::Vector2f bottomRight(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
		line(topRight, topLeft, color);
		line(topLeft, bottomLeft, color);
		line(bottomLeft, bottomRight, color);
		line(bottomRight, topRight, color);
	}

	void LineBatch::rect(const glvx::FloatRect& bounds, const glvx::Color& color, const glvx::Transform& transform) {
		glvx::Vector2f topRight(bounds.position.x + bounds.size.x, bounds.position.y);
		glvx::Vector2f topLeft(bounds.position.x, bounds.position.y);
		glvx::Vector2f bottomLeft(bounds.position.x, bounds.position.y + bounds.size.y);
		glvx::Vector2f bottomRight(bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y);
		line(topRight, topLeft, color, transform);
		line(topLeft, bottomLeft, color, transform);
		line(bottomLeft, bottomRight, color, transform);
		line(bottomRight, topRight, color, transform);
	}

	void LineBatch::draw(glvx::RenderTarget& target) const {
		if (m_vertices.getVertexCount() > 0) {
			target.draw(m_vertices);
		}
	}

	void LineBatch::clear() {
		m_vertices.clear();
	}

}
