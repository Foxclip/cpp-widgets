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

	class Widget;

	class WidgetUnclippedRegion {
	public:
		WidgetUnclippedRegion(Widget* widget);
		const glvx::FloatRect& get() const;
		const glvx::FloatRect& getQuantized() const;
		bool isNonZero() const;
		bool isQuantizedNonZero() const;
		void recalc() const;
		void invalidate();

	private:
		Widget* widget = nullptr;
		mutable glvx::FloatRect unclippedRegion;
		mutable glvx::FloatRect quantizedUnclippedRegion;
		mutable bool valid = false;

	};

}
