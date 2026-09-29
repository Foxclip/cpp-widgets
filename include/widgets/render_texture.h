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

// If logical texture size is at least this smaller than physical size,
// texture is reallocated
const float RENDER_TEXTURE_DOWNSIZE_FACTOR = 0.75;
// Once texture grows to this size, it cannot become smaller than this size
const unsigned int RENDER_TEXTURE_NO_DEALLOC_BELOW = 512;

class RenderTexture {
public:
	void create(unsigned int new_width, unsigned int new_height);
	glvx::Vector2u getSize() const;
	glvx::Vector2u getPhysicalSize() const;
	glvx::RenderTexture& get();

private:
	unsigned int width = 0;
	unsigned int height = 0;
	glvx::RenderTexture render_texture;

};
