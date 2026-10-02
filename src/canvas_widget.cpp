#include "widgets/canvas_widget.h"
#include "widgets/widget_list.h"
#include "widgets/widgets_common.h"

namespace fw {

	CanvasWidget::CanvasWidget(WidgetList& widget_list, float width, float height, unsigned int texture_width, unsigned int texture_height)
		: RectangleWidget(widget_list, width, height) {
		type = WidgetType::Canvas;
		setName("canvas");
		setTextureSize(texture_width, texture_height);
	}

	CanvasWidget::CanvasWidget(WidgetList& widget_list, const glvx::Vector2f& size, const glvx::Vector2u& texture_size)
		: CanvasWidget(widget_list, size.x, size.y, texture_size.x, texture_size.y) {
	}

	CanvasWidget::CanvasWidget(const CanvasWidget& other) : RectangleWidget(other) {
		type = WidgetType::Canvas;
		setName(other.name);
		setTextureSize((unsigned int)other.texture.getWidth(), (unsigned int)other.texture.getHeight());
		texture.clear(glvx::Color::Transparent);
		draw_texture_rect(
			texture,
			other.texture,
			glvx::Vector2f(0.0f, 0.0f),
			glvx::Vector2f((float)other.texture.getWidth(), (float)other.texture.getHeight()),
			glvx::Color::White
		);
		setView(other.view);
	}

	glvx::RenderTexture& CanvasWidget::getRenderTexture() {
		return texture;
	}

	glvx::Vector2f CanvasWidget::getTextureSize() const {
		return glvx::Vector2f((float)texture.getWidth(), (float)texture.getHeight());
	}

	const glvx::View& CanvasWidget::getView() const {
		return view;
	}

	void CanvasWidget::setTextureSize(unsigned int width, unsigned int height) {
		if (width == (unsigned int)texture.getWidth() && height == (unsigned int)texture.getHeight()) {
			return;
		}
		texture.create((int)width, (int)height);
		rect.setTexture(&texture);
		resetView();
	}

	void CanvasWidget::setView(const glvx::View& view) {
		this->view = view;
		texture.setView(view);
	}

	void CanvasWidget::setViewCenter(float x, float y) {
		// glvx::View position is the center of the visible region
		view.setPosition(x, y);
		texture.setView(view);
	}

	void CanvasWidget::setViewCenter(const glvx::Vector2f& center) {
		setViewCenter(center.x, center.y);
	}

	void CanvasWidget::setViewSize(float width, float height) {
		// glvx::View scale is the zoom: visible size = texture size / zoom
		view.setScale((float)texture.getWidth() / width, (float)texture.getHeight() / height);
		texture.setView(view);
	}

	void CanvasWidget::setViewSize(const glvx::Vector2f& size) {
		setViewSize(size.x, size.y);
	}

	void CanvasWidget::resetView() {
		view.setPosition((float)texture.getWidth() / 2.0f, (float)texture.getHeight() / 2.0f);
		view.setScale(1.0f, 1.0f);
		texture.setView(view);
	}

	void CanvasWidget::clear(const glvx::Color& color) {
		texture.clear(color);
	}

	void CanvasWidget::draw(const glvx::Drawable& drawable, const glvx::RenderStates& states) {
		glvx::RenderStates states_copy(states);
		states_copy.blend_mode = glvx::BlendAlpha;
		texture.draw(drawable, states_copy);
	}

	void CanvasWidget::display() {
		texture.display();
	}

	void CanvasWidget::saveToFile(std::filesystem::path path) {
		texture.readPixels().saveToFile(path.string());
	}

	CanvasWidget* CanvasWidget::clone(bool with_children) {
		return widget_list.duplicateWidget(this, with_children);
	}

}
