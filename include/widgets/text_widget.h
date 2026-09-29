#pragma once

#include "widget.h"

namespace fw {

	class WidgetList;

	class TextWidget : public Widget {
	public:
		TextWidget(WidgetList& widget_list);
		bool isVisualPositionQuantized() const override;
		glvx::FloatRect getLocalBounds() const override;
		glvx::FloatRect getVisualLocalBounds() const override;
		glvx::Vector2f getRenderPositionOffset() const override;
		const fw::Font& getFont() const;
		size_t getStringSize() const;
		unsigned int getCharacterSize() const;
		const glvx::Color& getFillColor() const override;
		const std::string& getString() const;
		float getKerning(size_t index) const;
		glvx::Vector2f getLocalCharPos(size_t index, bool top_aligned = true, bool with_kerning = true) const;
		glvx::Vector2f getParentLocalCharPos(size_t index, bool top_aligned = true, bool with_kerning = true) const;
		glvx::Vector2f getGlobalCharPos(size_t index, bool top_aligned = true, bool with_kerning = true) const;
		size_t getCharAt(const glvx::Vector2f& pos) const;
		void setFont(const fw::Font& font);
		void setCharacterSize(unsigned int size);
		void setFillColor(const glvx::Color& color) override;
		void setAdjustLocalBounds(bool value);
		void setString(const std::string& string);
		void insert(size_t pos, const std::string& str);
		void erase(size_t index_first, size_t count = 1);
		TextWidget* clone(bool with_children = true) override;

	protected:
		bool adjust_local_bounds = true;

		glvx::Drawable* getDrawable() override;
		const glvx::Drawable* getDrawable() const override;
		glvx::Transformable* getTransformable() override;
		const glvx::Transformable* getTransformable() const override;
		void setSizeInternal(float width, float height) override;
		void internalPreUpdate() override;

	private:
		glvx::Text text{nullptr, ""};
		fw::Font font;
		unsigned int character_size = glvx::FONT_DEFAULT_SIZE;
		glvx::Color fill_color = glvx::Color::White;
		glvx::FloatRect getGlvxTextBounds() const;
	};

}
