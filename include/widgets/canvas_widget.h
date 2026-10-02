#pragma once

#include "widgets/rectangle_widget.h"
#include <filesystem>

namespace fw {

	class WidgetList;

	class CanvasWidget : public RectangleWidget {
	public:
		CanvasWidget(WidgetList& widget_list, float width, float height, unsigned int texture_width, unsigned int texture_height);
		CanvasWidget(WidgetList& widget_list, const glvx::Vector2f& size, const glvx::Vector2u& texture_size);
		CanvasWidget(const CanvasWidget& widget);
		glvx::RenderTexture& getRenderTexture();
		glvx::Vector2f getTextureSize() const;
		const glvx::View& getView() const;
		void setTextureSize(unsigned int width, unsigned int height);
		void setView(const glvx::View& view);
		void setViewCenter(float x, float y);
		void setViewCenter(const glvx::Vector2f& center);
		void setViewSize(float width, float height);
		void setViewSize(const glvx::Vector2f& size);
		void resetView();
		void clear(const glvx::Color& color = glvx::Color::Black);
		void draw(const glvx::Drawable& drawable, const glvx::RenderStates& states = glvx::RenderStates());
		void display();
		void saveToFile(std::filesystem::path path);
		CanvasWidget* clone(bool with_children = true) override;

	protected:
		glvx::RenderTexture texture;
		glvx::View view;

	};

}
