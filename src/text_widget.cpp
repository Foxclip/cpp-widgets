#include "widgets/text_widget.h"
#include "widgets/widget_list.h"

namespace fw {

	TextWidget::TextWidget(WidgetList& widget_list) : Widget(widget_list) {
		type = WidgetType::Text;
		setName("text");
		if (widget_list.getDefaultFont().isLoaded()) {
			setFont(widget_list.getDefaultFont());
		}
	}

	bool TextWidget::isVisualPositionQuantized() const {
		return true;
	}

	glvx::FloatRect TextWidget::getLocalBounds() const {
		glvx::FloatRect result;
		if (adjust_local_bounds) {
			glvx::FloatRect local_bounds = getGlvxTextBounds();
			glvx::Vector2f size(local_bounds.position.x + local_bounds.size.x, (float)character_size);
			result = glvx::FloatRect(glvx::Vector2f(), size);
		} else {
			result = getVisualLocalBounds();
		}
		return result;
	}

	glvx::FloatRect TextWidget::getVisualLocalBounds() const {
		glvx::FloatRect local_bounds = getGlvxTextBounds();
		glvx::Vector2f offset = getRenderPositionOffset();
		glvx::Vector2f offset_pos = local_bounds.position + offset;
		glvx::FloatRect offset_bounds = glvx::FloatRect(offset_pos, local_bounds.size);
		return offset_bounds;
	}

	glvx::Vector2f TextWidget::getRenderPositionOffset() const {
		glvx::Vector2f offset(0.0f, 0.0f);
		if (!adjust_local_bounds) {
			glvx::FloatRect bounds = getGlvxTextBounds();
			offset = glvx::Vector2f(0.0f, bounds.position.y);
		}
		return -offset;
	}

	glvx::FloatRect TextWidget::getGlvxTextBounds() const {
		if (!font.isLoaded()) {
			return glvx::FloatRect();
		}
		return getTextVisualBounds(font.getFont(character_size), character_size, getString());
	}

	const fw::Font& TextWidget::getFont() const {
		return font;
	}

	size_t TextWidget::getStringSize() const {
		return getString().size();
	}

	unsigned int TextWidget::getCharacterSize() const {
		return character_size;
	}

	const glvx::Color& TextWidget::getFillColor() const {
		return fill_color;
	}

	const std::string& TextWidget::getString() const {
		return text.getString();
	}

	float TextWidget::getKerning(size_t index) const {
		if (index == 0 || index > getStringSize()) {
			return 0.0f;
		}
		wAssert(font.isLoaded());
		unsigned char left_char = (unsigned char)getString()[index - 1];
		unsigned char right_char = (unsigned char)getString()[index];
		return (float)font.getFont(character_size).getKerning(character_size, left_char, right_char);
	}

	glvx::Vector2f TextWidget::getLocalCharPos(size_t index, bool top_aligned, bool with_kerning) const {
		wAssert(getFont().isLoaded());
		// The position is computed widget-side to replicate SFML's
		// findCharacterPos semantics; it is returned in local coordinates
		glvx::Vector2f local_char_pos = findTextCharacterPos(font.getFont(character_size), character_size, getString(), index);
		if (with_kerning) {
			local_char_pos.x += getKerning(index);
		}
		if (top_aligned) {
			return glvx::Vector2f(local_char_pos.x, 0.0f);
		} else {
			return local_char_pos;
		}
	}

	glvx::Vector2f TextWidget::getParentLocalCharPos(size_t index, bool top_aligned, bool with_kerning) const {
		glvx::Vector2f local_char_pos = getLocalCharPos(index, top_aligned, with_kerning);
		glvx::Vector2f parent_local_char_pos = getTransform() * local_char_pos;
		return parent_local_char_pos;
	}

	glvx::Vector2f TextWidget::getGlobalCharPos(size_t index, bool top_aligned, bool with_kerning) const {
		glvx::Vector2f local_pos = getLocalCharPos(index, top_aligned, with_kerning);
		glvx::Vector2f global_pos = toGlobal(local_pos);
		return global_pos;
	}

	size_t TextWidget::getCharAt(const glvx::Vector2f& pos) const {
		ptrdiff_t result = 0;
		for (size_t i = 0; i <= getStringSize(); i++) {
			glvx::Vector2f local_char_pos = getLocalCharPos(i, true, true);
			if (pos.x >= local_char_pos.x) {
				result = i;
			}
		}
		return result;
	}

	void TextWidget::setFont(const fw::Font& font) {
		this->font = font;
		setRenderIterations(font.isDoubleRendered() ? 2 : 1);
		glvx::Font* glvx_font = font.isLoaded() ? &font.getFont(character_size) : nullptr;
		text.setFont(glvx_font);
		text.setCharacterSize(character_size);
	}

	void TextWidget::setCharacterSize(unsigned int size) {
		character_size = size;
		text.setCharacterSize(character_size);
		if (font.isLoaded()) {
			text.setFont(&font.getFont(character_size));
		}
	}

	void TextWidget::setFillColor(const glvx::Color& color) {
		text.setColor(color);
		fill_color = color;
	}

	void TextWidget::setAdjustLocalBounds(bool value) {
		adjust_local_bounds = value;
	}

	void TextWidget::setString(const std::string& string) {
		text.setString(string);
	}

	void TextWidget::insert(size_t pos, const std::string& str) {
		wAssert(pos >= 0 && pos <= getStringSize());
		std::string new_str = text.getString();
		new_str.insert(pos, str);
		text.setString(new_str);
	}

	void TextWidget::erase(size_t index_first, size_t count) {
		if (text.getString().empty()) {
			return;
		}
		size_t index_last = index_first + count - 1;
		wAssert(index_first >= 0 && index_last < getStringSize());
		std::string str = text.getString();
		str.erase(index_first, count);
		text.setString(str);
	}

	TextWidget* TextWidget::clone(bool with_children) {
		return widget_list.duplicateWidget(this, with_children);
	}

	glvx::Drawable* TextWidget::getDrawable() {
		return &text;
	}

	const glvx::Drawable* TextWidget::getDrawable() const {
		return &text;
	}

	glvx::Transformable* TextWidget::getTransformable() {
		return &text;
	}

	const glvx::Transformable* TextWidget::getTransformable() const {
		return &text;
	}

	void TextWidget::setSizeInternal(float width, float height) {
		// do nothing
	}

	void TextWidget::internalPreUpdate() {
		wAssert(getFont().isLoaded(),
			"Font is not set for " + full_name +
			", consider setting default font in WidgetList"
		);
	}

}
