#include "widgets/widgets_common.h"
#include <fstream>
#include <algorithm>

namespace fw {

#ifndef NDEBUG

	void _print_msg(bool value) { }

	void _print_msg(bool value, const std::string& message) {
		if (!value) {
			std::cout << message << "\n";
		}
	}

#endif // !NDEBUG

	Logger& operator<<(Logger& lg, const glvx::Vector2f& value) {
		return lg << "(" << value.x << " " << value.y << ")";
	}

	glvx::Vector2i to2i(const glvx::Vector2f& vec) {
		return glvx::Vector2i((int)vec.x, (int)vec.y);
	}

	glvx::Vector2i to2i(const glvx::Vector2u& vec) {
		return glvx::Vector2i((int)vec.x, (int)vec.y);
	}

	glvx::Vector2f to2f(const glvx::Vector2i& vec) {
		return glvx::Vector2f((float)vec.x, (float)vec.y);
	}

	glvx::Vector2f to2f(const glvx::Vector2u& vec) {
		return glvx::Vector2f((float)vec.x, (float)vec.y);
	}

	void extend_bounds(glvx::FloatRect& rect1, const glvx::FloatRect& rect2) {
		float rect1_right = rect1.position.x + rect1.size.x;
		float rect2_right = rect2.position.x + rect2.size.x;
		float rect1_bottom = rect1.position.y + rect1.size.y;
		float rect2_bottom = rect2.position.y + rect2.size.y;
		if (rect2.position.x < rect1.position.x) {
			rect1.position.x = rect2.position.x;
		}
		if (rect2.position.y < rect1.position.y) {
			rect1.position.y = rect2.position.y;
		}
		if (rect2_right > rect1_right) {
			rect1.size.x += rect2_right - rect1_right;
		}
		if (rect2_bottom > rect1_bottom) {
			rect1.size.y += rect2_bottom - rect1_bottom;
		}
	}

	bool parseLL(const std::string& str, long long& result) {
		if (str.size() == 0) {
			return false;
		}
		const char* start = str.c_str();
		const char* end = start + str.size();
		char* pos;
		result = std::strtoll(start, &pos, 10);
		return pos == end;
	}

	bool parseFloat(const std::string& str, float& result) {
		if (str.size() == 0) {
			return false;
		}
		const char* start = str.c_str();
		const char* end = start + str.size();
		char* pos;
		result = std::strtof(start, &pos);
		return pos == end;
	}

	bool contains_point(const glvx::FloatRect& rect, const glvx::Vector2f& point, bool include_upper_bound) {
		auto cmp = [&](float left, float right) {
			if (include_upper_bound) {
				return left <= right;
			} else {
				return left < right;
			}
		};
		return (
			point.x >= rect.position.x
			&& cmp(point.x, rect.position.x + rect.size.x)
			&& point.y >= rect.position.y
			&& cmp(point.y, rect.position.y + rect.size.y)
		);
	}

	bool contains_point(const glvx::Rectangle& shape, const glvx::Vector2f& point, bool include_upper_bound) {
		glvx::FloatRect local_bounds(glvx::Vector2f(), shape.getSize());
		return contains_point(shape.getTransform().transformRect(local_bounds), point, include_upper_bound);
	}

	void quantize_position(glvx::Transform& transform) {
		const float* matrix_data = transform.toMatrix4().getData();
		float x_pos = matrix_data[12];
		float y_pos = matrix_data[13];
		float x_offset = x_pos - floor(x_pos);
		float y_offset = y_pos - floor(y_pos);
		glvx::Vector2f subpixel_offset = glvx::Vector2f(x_offset, y_offset);
		transform.translate(-subpixel_offset);
	}

	// Replicates sf::Text::findCharacterPos: the position of the start of the
	// character at `index`, with y measured as the line offset from the top of
	// the first line (not the baseline).
	glvx::Vector2f findTextCharacterPos(glvx::Font& font, unsigned int character_size, const std::string& str, size_t index) {
		if (index > str.size()) {
			index = str.size();
		}
		float whitespace_width = (float)font.getCharacter(character_size, ' ').advance;
		float line_height = (float)font.getLineHeight(character_size);
		float x = 0.0f;
		float y = 0.0f;
		unsigned char prev_char = 0;
		for (size_t i = 0; i < index; i++) {
			unsigned char cur_char = (unsigned char)str[i];
			x += (float)font.getKerning(character_size, prev_char, cur_char);
			prev_char = cur_char;
			if (cur_char == ' ') {
				x += whitespace_width;
			} else if (cur_char == '\t') {
				x += whitespace_width * 4.0f;
			} else if (cur_char == '\n') {
				y += line_height;
				x = 0.0f;
			} else if (cur_char != '\r') {
				x += (float)font.getCharacter(character_size, cur_char).advance;
			}
		}
		return glvx::Vector2f(x, y);
	}

	// Replicates sf::Text::getLocalBounds() geometry (Text::ensureGeometryUpdate
	// in SFML's Text.cpp): the baseline of the first line sits at y = character
	// size, each glyph contributes the rect (x + bearingLeft, baseline -
	// bearingTop, bitmapWidth, bitmapRows), spaces/newlines contribute the
	// pen position, and the result is the union of all contributions.
	glvx::FloatRect getTextVisualBounds(glvx::Font& font, unsigned int character_size, const std::string& str) {
		if (str.empty()) {
			return glvx::FloatRect();
		}
		float line_height = (float)font.getLineHeight(character_size);
		float font_size = (float)character_size;
		float whitespace_width = (float)font.getCharacter(character_size, ' ').advance;
		float x = 0.0f;
		float y = font_size;
		float min_x = font_size;
		float min_y = font_size;
		float max_x = 0.0f;
		float max_y = 0.0f;
		unsigned char prev_char = 0;
		for (size_t i = 0; i < str.size(); i++) {
			unsigned char cur_char = (unsigned char)str[i];
			if (cur_char == '\r') {
				continue;
			}
			x += (float)font.getKerning(character_size, prev_char, cur_char);
			prev_char = cur_char;
			if (cur_char == ' ' || cur_char == '\n' || cur_char == '\t') {
				min_x = std::min(min_x, x);
				min_y = std::min(min_y, y);
				if (cur_char == ' ') {
					x += whitespace_width;
				} else if (cur_char == '\t') {
					x += whitespace_width * 4.0f;
				} else {
					y += line_height;
					x = 0.0f;
				}
				max_x = std::max(max_x, x);
				max_y = std::max(max_y, y);
				continue;
			}
			const glvx::Character& ch = font.getCharacter(character_size, cur_char);
			float left = (float)ch.x;
			float top = (float)ch.top;
			float right = left + (float)ch.width;
			min_x = std::min(min_x, x + left);
			max_x = std::max(max_x, x + right);
			min_y = std::min(min_y, y - top);
			max_y = std::max(max_y, y - top + (float)ch.glyph_height);
			x += (float)ch.advance;
		}
		return glvx::FloatRect(glvx::Vector2f(min_x, min_y), glvx::Vector2f(max_x - min_x, max_y - min_y));
	}

	namespace {
		glvx::VertexArray texture_quad(glvx::PrimitiveType::TriangleStrip, 4);
	}

	void draw_texture_rect(
		glvx::RenderTarget& target,
		const glvx::AbstractTexture& texture,
		const glvx::Vector2f& pos,
		const glvx::Vector2f& size,
		const glvx::Color& color,
		const glvx::Transform& extra_transform,
		const glvx::FloatRect& uv_rect,
		glvx::Shader* shader
	) {
		texture_quad[0].position = pos;
		texture_quad[1].position = glvx::Vector2f(pos.x + size.x, pos.y);
		texture_quad[2].position = glvx::Vector2f(pos.x, pos.y + size.y);
		texture_quad[3].position = glvx::Vector2f(pos.x + size.x, pos.y + size.y);
		for (size_t i = 0; i < 4; i++) {
			texture_quad[i].color = color;
		}
		texture_quad[0].tex_coords = glvx::Vector2f(uv_rect.position.x, uv_rect.position.y + uv_rect.size.y);
		texture_quad[1].tex_coords = glvx::Vector2f(uv_rect.position.x + uv_rect.size.x, uv_rect.position.y + uv_rect.size.y);
		texture_quad[2].tex_coords = glvx::Vector2f(uv_rect.position.x, uv_rect.position.y);
		texture_quad[3].tex_coords = glvx::Vector2f(uv_rect.position.x + uv_rect.size.x, uv_rect.position.y);
		glvx::RenderStates states;
		states.transform = extra_transform;
		states.texture = const_cast<glvx::AbstractTexture*>(&texture);
		states.shader = shader;
		states.blend_mode = glvx::BlendAlpha;
		target.draw(texture_quad, states);
	}

	glvx::FloatRect quantize_rect(const glvx::FloatRect& rect, QuantizeMode quantize_mode) {
		auto rounding_func = [&](float x) {
			if (quantize_mode == QUANTIZE_MODE_FLOOR) {
				return floor(x);
			} else if (quantize_mode == QUANTIZE_MODE_FLOOR_SUBTRACT) {
				return floor(x) - 1.0f;
			} else if (quantize_mode == QUANTIZE_MODE_CEIL_SUBTRACT) {
				return ceil(x) - 1.0f;
			} else {
				wAssert(false, "Unknown QuantizeMode"); return floor(x);
			}
		};
		glvx::Vector2f top_left = rect.position;
		glvx::Vector2f bottom_right = top_left + rect.size;
		glvx::Vector2f quantized_top_left = glvx::Vector2f(floor(top_left.x), floor(top_left.y));
		glvx::Vector2f quantized_bottom_right = glvx::Vector2f(rounding_func(bottom_right.x), rounding_func(bottom_right.y));
		float quantized_width = quantized_bottom_right.x - quantized_top_left.x;
		float quantized_height = quantized_bottom_right.y - quantized_top_left.y;
		glvx::FloatRect quantized_bounds(quantized_top_left, glvx::Vector2f(quantized_width, quantized_height));
		return quantized_bounds;
	}

	void str_to_file(std::string& str, const std::filesystem::path& path) {
		std::ofstream ofstream(path);
		if (ofstream.is_open()) {
			ofstream << str;
		} else {
			std::string p = path.string();
			p.resize(FILENAME_MAX);
			throw std::runtime_error("File write error: " + p);
		}
	}

	std::string file_to_str(const std::filesystem::path& path) {
		if (!std::filesystem::exists(path)) {
			throw std::format("File not found: {}", path.string());
		}
		std::ifstream t(path);
		std::stringstream buffer;
		buffer << t.rdbuf();
		return buffer.str();
	}

	std::vector<std::string> read_file_lines(const std::filesystem::path& path) {
		std::vector<std::string> lines;
		std::ifstream file(path);
		if (!file.is_open()) {
			throw(std::runtime_error("Failed to open file: " + path.string()));
		}
		std::string line;
		while (std::getline(file, line)) {
			lines.push_back(line);
		}
		file.close();
		return lines;
	}

	std::string trim(const std::string &s) {
		if (!s.empty()) {
			std::string copy = s;
			copy.erase(copy.begin(), std::find_if(copy.begin(), copy.end(), [](unsigned char ch) { return !std::isspace(ch); }));
			copy.erase(std::find_if(copy.rbegin(), copy.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), copy.end());
			return copy;
		}
		return s;
	}

	std::string color_to_str(glvx::Color color) {
		return
			std::to_string(color.r)
			+ " " + std::to_string(color.g)
			+ " " + std::to_string(color.b)
			+ " " + std::to_string(color.a)
		;
	}

}
