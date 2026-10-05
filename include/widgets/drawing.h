#pragma once

#include <glvx/window.h>
#include <glvx/event.h>
#include <glvx/text.h>
#include <glvx/font.h>
#include <glvx/vertex.h>
#include <glvx/vertex_array.h>
#include <glvx/render_texture.h>
#include <glvx/render_states.h>
#include <glvx/shape.h>
#include <glvx/rectangle.h>
#include <glvx/image.h>
#include <glvx/cursor.h>
#include <glvx/keyboard.h>
#include <glvx/mouse.h>

namespace fw {

	template <typename TVec2>
	TVec2 quantize(const TVec2& vec) {
		return TVec2(floor(vec.x), floor(vec.y));
	}

	template <typename TVec2>
	TVec2 quantize_and_offset(const TVec2& vec) {
		return TVec2(floor(vec.x) + 0.5f, floor(vec.y) + 0.5f);
	}

	// Batches line segments into a single vertex array and issues one draw
	// call for the whole batch instead of one draw call per line. Used by the
	// debug render overlay, which would otherwise issue ~12 draws per visible
	// widget per frame.
	class LineBatch {
	public:
		LineBatch() : m_vertices(glvx::PrimitiveType::Lines, 0) { }

		void line(const glvx::Vector2f& v1, const glvx::Vector2f& v2, const glvx::Color& color);
		void line(const glvx::Vector2f& v1, const glvx::Vector2f& v2, const glvx::Color& color, const glvx::Transform& transform);
		void rect(const glvx::FloatRect& bounds, const glvx::Color& color);
		void rect(const glvx::FloatRect& bounds, const glvx::Color& color, const glvx::Transform& transform);
		void draw(glvx::RenderTarget& target) const;
		void clear();
		std::size_t getVertexCount() const { return m_vertices.getVertexCount(); }

	private:
		glvx::VertexArray m_vertices;
	};

	class CanvasWidget;

	void draw_line(
		glvx::RenderTarget& target,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color
	);

	void draw_line(
		CanvasWidget* canvas,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color
	);

	void draw_line(
		glvx::RenderTarget& target,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color,
		const glvx::Transform& transform
	);

	void draw_line(
		CanvasWidget* canvas,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Color& color,
		const glvx::Transform& transform
	);

	void draw_rect(
		glvx::RenderTarget& target,
		const glvx::Vector2f& v1,
		const glvx::Vector2f& v2,
		const glvx::Vector2f& v3,
		const glvx::Vector2f& v4,
		const glvx::Color& color
	);

	void draw_rect(
		glvx::RenderTarget& target,
		const glvx::Vector2f& pos,
		const glvx::Vector2f& size,
		const glvx::Color& color
	);

	void draw_wire_rect(
		glvx::RenderTarget& target,
		const glvx::FloatRect& bounds,
		const glvx::Color& color
	);

	void draw_wire_rect(
		CanvasWidget* canvas,
		const glvx::FloatRect& bounds,
		const glvx::Color& color
	);

	void draw_wire_rect(
		glvx::RenderTarget& target,
		const glvx::FloatRect& bounds,
		const glvx::Color& color,
		const glvx::Transform& transform
	);

	void draw_wire_rect(
		CanvasWidget* canvas,
		const glvx::FloatRect& bounds,
		const glvx::Color& color,
		const glvx::Transform& transform
	);

}
