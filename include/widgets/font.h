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
#include <map>
#include <memory>
#include <string>

namespace fw {

	// Caches one glvx::Font per requested size (each instance lazily
	// rasterizes the pages for the sizes it is queried with).
	class Font {
	public:
		Font();
		explicit Font(const std::string& filename, bool double_render = false, bool smooth = false);
		bool isLoaded() const;
		bool isDoubleRendered() const;
		// Returns the font at the requested size, constructing and caching
		// it on first use.
		glvx::Font& getFont(unsigned int size) const;

	private:
		bool loaded = false;
		std::string filename;
		bool double_render = false;
		bool smooth = false;
		// Copies share the underlying glvx::Font instances (they are
		// immutable after construction).
		mutable std::map<unsigned int, std::shared_ptr<glvx::Font>> fonts;
	};

}
