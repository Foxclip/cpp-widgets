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
#include <glvx/angle.h>
#include <glvx/image.h>
#include <glvx/cursor.h>
#include <glvx/keyboard.h>
#include <glvx/mouse.h>
#include <algorithm>
#include <cassert>
#include <concepts>
#include <functional>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <iostream>
#include <numbers>
#include <vector>
#include "logger/logger.h"

namespace fw {

	enum class CursorType {
		Arrow, Text,
		SizeTopLeft, SizeTop, SizeTopRight,
		SizeLeft, SizeRight,
		SizeBottomLeft, SizeBottom, SizeBottomRight
	};

#ifndef NDEBUG

	void _print_msg(bool value);
	void _print_msg(bool value, const std::string& message);

#define wAssert(value, ...) \
	fw::_print_msg(value, __VA_ARGS__); \
	assert(value);

#else

#define wAssert(value, ...)

#endif // !NDEBUG

	Logger& operator<<(Logger& lg, const glvx::Vector2f& value);
	glvx::Vector2i to2i(const glvx::Vector2f& vec);
	glvx::Vector2i to2i(const glvx::Vector2u& vec);
	glvx::Vector2f to2f(const glvx::Vector2i& vec);
	glvx::Vector2f to2f(const glvx::Vector2u& vec);
	void extend_bounds(glvx::FloatRect& rect1, const glvx::FloatRect& rect2);
	bool parseLL(const std::string& str, long long& result);
	bool parseFloat(const std::string& str, float& result);
	bool contains_point(const glvx::FloatRect& rect, const glvx::Vector2f& point, bool include_upper_bound = false);
	bool contains_point(const glvx::Rectangle& shape, const glvx::Vector2f& point, bool include_upper_bound = false);
	void quantize_position(glvx::Transform& transform);
	// Position of the character at `index` (baseline y) in the local
	// coordinates of a glvx::Text rendering `str` with `font` at
	// `character_size`.
	glvx::Vector2f findTextCharacterPos(glvx::Font& font, unsigned int character_size, const std::string& str, size_t index);
	// Visual bounds of `str` rendered by glvx::Text with `font` at
	// `character_size`, replicating the glyph-rect semantics of
	// glvx::Text::calculateVisualBounds().
	glvx::FloatRect getTextVisualBounds(glvx::Font& font, unsigned int character_size, const std::string& str);
	enum QuantizeMode {
		QUANTIZE_MODE_FLOOR,
		QUANTIZE_MODE_FLOOR_SUBTRACT,
		QUANTIZE_MODE_CEIL_SUBTRACT,
	};
	glvx::FloatRect quantize_rect(const glvx::FloatRect& rect, QuantizeMode quantize_mode);
	// Draws a textured quad with vertex color `color` using BlendAlpha.
	// Textures are expected in GLVX render texture orientation (screen top
	// maps to v = 1). uv_rect is in [0,1] texture space, (0,0) at the
	// top-left of the texture image.
	void draw_texture_rect(
		glvx::RenderTarget& target,
		const glvx::AbstractTexture& texture,
		const glvx::Vector2f& pos,
		const glvx::Vector2f& size,
		const glvx::Color& color = glvx::Color::White,
		const glvx::Transform& extra_transform = glvx::Transform(),
		const glvx::FloatRect& uv_rect = glvx::FloatRect(0.0f, 0.0f, 1.0f, 1.0f),
		glvx::Shader* shader = nullptr
	);
	void str_to_file(std::string& str, const std::filesystem::path& path);
	std::string file_to_str(const std::filesystem::path& path);
	std::vector<std::string> read_file_lines(const std::filesystem::path& path);
	std::string trim(const std::string &s);
	std::string color_to_str(glvx::Color color);

	template <typename T>
	std::string vec_to_str(const T& vec) {
		return std::to_string(vec.x) + " " + std::to_string(vec.y);
	}

	template <typename TNode>
	concept NodeLess = requires(const TNode& left, const TNode& right) {
		left < right;
	};

	template <typename TPFunc, typename TNode>
	concept NodeVectorFunc = requires(TPFunc f, TNode n) {
		{ f(n) } -> std::convertible_to<const std::vector<TNode>&>;
	};

	template<typename TNode, typename TPFunc>
	requires NodeLess<TNode> && std::equality_comparable<TNode> && NodeVectorFunc<TPFunc, TNode>
	std::vector<std::vector<TNode>> toposort(
		const std::vector<TNode>& nodes,
		const TPFunc& get_parents_func,
		std::function<void(const std::vector<TNode>&)>* OnLoopDetected = nullptr
	) {
		std::vector<std::vector<TNode>> result;
		std::map<TNode, size_t> processed_nodes;
		std::vector<TNode> node_stack;
		std::set<TNode> node_stack_set;
		bool loop_detected = false;
		std::set<std::set<TNode>> loops;
		std::function<ptrdiff_t(const TNode&)> process_node = [&](const TNode& node) {
			node_stack.push_back(node);
			node_stack_set.insert(node);
			const std::vector<TNode>& parents = get_parents_func(node);
			ptrdiff_t max_layer = -1;
			bool looping = false;
			for (const TNode& parent : parents) {
				if (node_stack_set.contains(parent)) {
					looping = true;
					loop_detected = true;
					max_layer = -1;
					if (OnLoopDetected) {
						auto it = std::find(node_stack.begin(), node_stack.end(), parent);
						std::vector<TNode> loop(it, node_stack.end());
						std::set<TNode> loop_set = std::set<TNode>(loop.begin(), loop.end());
						if (!loops.contains(loop_set)) {
							loops.insert(loop_set);
							(*OnLoopDetected)(loop);
						}
					} else {
						throw std::runtime_error("toposort: loop detected");
					}
				} else {
					ptrdiff_t parent_layer = -1;
					auto it = processed_nodes.find(parent);
					if (loop_detected || it == processed_nodes.end()) {
						parent_layer = process_node(parent);
					} else {
						parent_layer = it->second;
					}
					if (parent_layer == -1) {
						looping = true;
					}
					if (!looping) {
						max_layer = std::max(max_layer, parent_layer);
					}
				}
			}
			if (!loop_detected) {
				max_layer++;
				if (max_layer >= (ptrdiff_t)result.size()) {
					result.push_back(std::vector<TNode>());
				}
				result[max_layer].push_back(node);
			}
			processed_nodes[node] = max_layer;
			node_stack.pop_back();
			node_stack_set.erase(node);
			return max_layer;
		};
		for (const TNode& node : nodes) {
			if (loop_detected || !processed_nodes.contains(node)) {
				process_node(node);
			}
		}
		return result;
	}

	template <typename TVec2>
	TVec2 get_circle_vertex(ptrdiff_t index, size_t point_count, float radius, glvx::Angle angle_offset = glvx::Angle()) {
		glvx::Angle angle = glvx::Angle::fromRadians((float)index / (float)point_count * 2.0f * std::numbers::pi + angle_offset.asRadians());
		float x = std::cos(angle.asRadians()) * radius;
		float y = std::sin(angle.asRadians()) * radius;
		return TVec2(x, y);
	}

	template <typename TVec2>
	std::vector<TVec2> get_regular_polygon(size_t point_count, float radius, glvx::Angle angle_offset = glvx::Angle()) {
		std::vector<TVec2> vertices;
		for (size_t i = 0; i < point_count; i++) {
			TVec2 vertex = get_circle_vertex<TVec2>(i, point_count, radius, angle_offset);
			vertices.push_back(vertex);
		}
		return vertices;
	}

	template <typename T>
	float length(const T& vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}

}
