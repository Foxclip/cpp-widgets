#include "widgets/drawing.h"
#include "widgets/canvas_widget.h"

namespace fw {

	// NOTE: These draw helpers intentionally create a fresh, function-local
	// VertexArray on every call instead of reusing a single shared/global
	// VertexArray. A long-lived shared vertex array (whose VAO is reused across
	// many draw calls while its buffer is updated in place) silently fails to
	// render on some drivers (observed with NVIDIA, especially in Debug builds):
	// the clear/background shows but the geometry is missing. A freshly created
	// vertex array per draw renders reliably. The small per-call VBO/VAO churn
	// is an acceptable cost for correctness.

	namespace {

		void draw_line_impl(glvx::RenderTarget& target, const glvx::Vector2f& v1, const glvx::Vector2f& v2, const glvx::Color& color) {
			glvx::VertexArray line(glvx::PrimitiveType::Lines, 2);
			line[0].position = v1;
			line[0].color = color;
			line[1].position = v2;
			line[1].color = color;
			target.draw(line);
		}

		void draw_line_impl(CanvasWidget* canvas, const glvx::Vector2f& v1, const glvx::Vector2f& v2, const glvx::Color& color) {
			glvx::VertexArray line(glvx::PrimitiveType::Lines, 2);
			line[0].position = v1;
			line[0].color = color;
			line[1].position = v2;
			line[1].color = color;
			canvas->draw(line);
		}

		void draw_rect_impl(glvx::RenderTarget& target, const glvx::Vector2f& v1, const glvx::Vector2f& v2, const glvx::Vector2f& v3, const glvx::Vector2f& v4, const glvx::Color& color) {
			glvx::VertexArray rect(glvx::PrimitiveType::TriangleStrip, 4);
			rect[0].position = v1;
			rect[0].color = color;
			rect[1].position = v2;
			rect[1].color = color;
			rect[2].position = v3;
			rect[2].color = color;
			rect[3].position = v4;
			rect[3].color = color;
			target.draw(rect);
		}

	}

	void draw_line(
		glvx::RenderTarget& target,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color
	) {
		draw_line_impl(target, quantize_and_offset(v1), quantize_and_offset(v2), color);
	}

	void draw_line(
		CanvasWidget* canvas,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color
	) {
		draw_line_impl(canvas, quantize_and_offset(v1), quantize_and_offset(v2), color);
	}

	void draw_line(
		glvx::RenderTarget& target,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color,
		const glvx::Transform& transform
	) {
		draw_line_impl(target, quantize_and_offset(transform.transformPoint(v1)), quantize_and_offset(transform.transformPoint(v2)), color);
	}

	void draw_line(
		CanvasWidget* canvas,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color,
		const glvx::Transform& transform
	) {
		draw_line_impl(canvas, quantize_and_offset(transform.transformPoint(v1)), quantize_and_offset(transform.transformPoint(v2)), color);
	}

	void draw_rect(
		glvx::RenderTarget& target,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Vector2f& v3,
		const glvx::Vector2f& v4,
		const glvx::Color& color
	) {
		draw_rect_impl(target, quantize(v1), quantize(v2), quantize(v3), quantize(v4), color);
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

}
