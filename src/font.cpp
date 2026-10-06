#include "widgets/font.h"
#include "widgets/widgets_common.h"

namespace fw {

	Font::Font() { }

	Font::Font(const std::string& filename, bool double_render, bool smooth) {
		if (!std::filesystem::exists(filename)) {
			throw std::runtime_error("Font loading error (file not found: " + filename + ")");
		}
		this->filename = filename;
		this->double_render = double_render;
		this->smooth = smooth;
		loaded = true;
	}

	bool Font::isLoaded() const {
		return loaded;
	}

	bool Font::isDoubleRendered() const {
		return double_render;
	}

	glvx::Font& Font::getFont(unsigned int size) const {
		wAssert(isLoaded());
		auto it = fonts.find(size);
		if (it != fonts.end()) {
			return *it->second;
		}
		auto font_ptr = dp::make_shared_data_pointer<glvx::Font>("Font " + filename + " size " + std::to_string(size), filename, false);
		glvx::Font& font_ref = *font_ptr;
		fonts.emplace(size, std::move(font_ptr));
		return font_ref;
	}

}
