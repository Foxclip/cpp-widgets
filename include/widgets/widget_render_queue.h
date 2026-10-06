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
#include "common/compvector.h"

namespace fw {

	class Widget;
	class WidgetList;

	enum class GlobalRenderLayer {
		BASE,
		DROPDOWN_PANEL,
		TOP
	};

	struct RenderQueueLayer {
		size_t layer;
		CompVector<Widget*> widgets;
		RenderQueueLayer(size_t layer);
		bool operator<(const RenderQueueLayer& other) const;
		bool operator==(const RenderQueueLayer& other) const;
	};

	class WidgetRenderQueue {
	public:
		WidgetRenderQueue(WidgetList& widget_list);
		void update();
		const std::vector<RenderQueueLayer>& get() const;
		void remove(Widget* widget);
		// The queue content depends only on tree structure, widget
		// visibility and render layers, so it is rebuilt lazily:
		// mutation points call invalidate(), update() rebuilds only
		// when dirty.
		void invalidate();

	private:
		WidgetList& widget_list;
		std::vector<RenderQueueLayer> layers;
		bool valid = false;

	};

}
