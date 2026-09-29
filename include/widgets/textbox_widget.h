#pragma once

#include "rectangle_widget.h"
#include "text_widget.h"
#include "clip/clip.h"
#include "common/history.h"
#include <chrono>

namespace fw {

	const glvx::Vector2f TEXTBOX_DEFAULT_SIZE = glvx::Vector2f(40.0f, 20.0f);
	const glvx::Vector2f TEXTBOX_TEXT_VIEW_ZERO_POS = glvx::Vector2f(2.0f, 0.0f);
	const glvx::Vector2f TEXTBOX_CURSOR_OFFSET = glvx::Vector2f(0.0f, 0.0f);
	const float TEXTBOX_CURSOR_MARGIN = 2.0f; // cursor sticking out above and below text
	const float TEXTBOX_CURSOR_BLINK_INTERVAL = 0.5f;
	const float TEXTBOX_SELECTION_MARGIN = 2.0f; // selection sticking out above and below text
	const std::string TEXTBOX_VALID_INTEGER_CHARS = "+-0123456789";
	const std::string TEXTBOX_VALID_FLOAT_CHARS = "+-.e0123456789";
	const float TEXTBOX_CURSOR_MOVE_MARGIN = 2.0f; // scroll text when cursor approaches margin

	class Timer {
	public:
		Timer();
		void reset();
		double get() const;
	private:
		std::chrono::steady_clock::time_point begin;
	};

	class WidgetList;

	class TextBoxWidget : public RectangleWidget {
	public:
		std::function<void(bool)> OnEditModeToggle = [](bool new_value) { };
		std::function<void(const std::string&)> OnValueChanged = [](const std::string& new_value) { };
		std::function<void(const std::string&)> OnConfirm = [](const std::string& value) { };
		std::function<void()> OnCancel = []() { };
		enum TextBoxType {
			TEXT,
			INTEGER,
			FLOAT,
		};

		TextBoxWidget(WidgetList& widget_list);
		TextBoxWidget(WidgetList& widget_list, float width, float height);
		TextBoxWidget(WidgetList& widget_list, const glvx::Vector2f& size);
		const glvx::Color& getFillColor() const override;
		const glvx::Color& getHighlightColor() const;
		const glvx::Color& getTextColor() const;
		const glvx::Color& getEditorColor() const;
		const glvx::Color& getEditorTextColor() const;
		const glvx::Color& getSelectionColor() const;
		const glvx::Color& getFailFillColor() const;
		const glvx::Color& getEditFailFillColor() const;
		const TextWidget* getTextWidget() const;
		const fw::Font& getFont() const;
		unsigned int getCharacterSize() const;
		const std::string& getValue() const;
		const std::string getSelectedText() const;
		bool isValidValue() const;
		size_t getStringSize() const;
		TextBoxType getTextboxType() const;
		size_t getCursorPos() const;
		bool isEditMode() const;
		bool isSelectionActive() const;
		glvx::Vector2f getLocalCharPos(size_t index, bool top_aligned = true, bool with_kerning = true) const;
		glvx::Vector2f getGlobalCharPos(size_t index, bool top_aligned = true, bool with_kerning = true) const;
		const glvx::Character& getGlyph(size_t index) const;
		ptrdiff_t getSelectionLeft() const;
		ptrdiff_t getSelectionRight() const;
		void setFillColor(const glvx::Color& color) override;
		void setHighlightColor(const glvx::Color& color);
		void setTextColor(const glvx::Color& color);
		void setEditorColor(const glvx::Color& color);
		void setEditorTextColor(const glvx::Color& color);
		void setSelectionColor(const glvx::Color& color);
		void setFailFillColor(const glvx::Color& color);
		void setEditFailFillColor(const glvx::Color& color);
		void setFont(const fw::Font& font);
		void setCharacterSize(unsigned int size);
		void setValueSilent(const std::string& value);
		void setValue(const std::string& value);
		void setType(TextBoxType type);
		void setCursorPos(size_t pos);
		void typeChar(uint32_t code);
		void insert(size_t pos, const std::string& str);
		void erase(size_t index_first, size_t count);
		void eraseSelection();
		TextBoxWidget* clone(bool with_children = true) override;


	protected:
		enum ActionType {
			ACTION_DELETE,
			ACTION_PASTE,
			ACTION_CUT,
			ACTION_BACKSPACE,
			ACTION_TYPE,
		};
		void internalPreUpdate() override;
		void updateColors();
		void internalOnLeftPress(const glvx::Vector2f& pos, bool became_focused) override;
		void internalOnGlobalLeftRelease(const glvx::Vector2f& pos) override;
		void internalOnEditModeToggle(bool value);
		void internalOnFocused() override;
		void internalOnFocusLost() override;
		void processKeyPressedEvent(const glvx::Event& event);
		void processTextEnteredEvent(const glvx::Event& event);
		void internalProcessKeyboardEvent(const glvx::Event& event) override;
		void internalProcessMouse(const glvx::Vector2f& pos) override;
		void internalOnMouseEnter(const glvx::Vector2f& pos) override;
		void internalOnMouseExit(const glvx::Vector2f& pos) override;
		void internalOnValueChanged(const std::string& new_value);
		void internalOnConfirm(const std::string& value);
		void internalOnCancel();
		void updateValid();
		void enableEditMode();
		void disableEditMode(bool confirm);
		void insertSilent(size_t pos, const std::string& str);
		void eraseSilent(size_t index_first, size_t count);
		void updateTextScroll();
		void updateCursorSize();
		void updateSelection();
		void setSelection(ptrdiff_t pos);
		size_t calcCursorPos(const glvx::Vector2f& pos);
		void trySetCursor(const glvx::Vector2f& pos);
		void selectAll();
		void deselectAll();
		std::string getActionTag(ActionType action_type);
		void doNormalAction(ActionType action_type, const std::function<void()>& action);
		void doCursorAction(const std::function<void()>& action);
		void doGroupAction(ActionType action_type, const std::function<void()>& action);

	private:
		TextWidget* text_widget = nullptr;
		RectangleWidget* cursor_widget = nullptr;
		RectangleWidget* selection_widget = nullptr;
		TextBoxType textbox_type = TextBoxType::TEXT;
		fw::Font font;
		struct TextBoxHistoryEntry {
			std::string str;
			size_t cursor_pos = 0;
			ptrdiff_t last_action_pos = -1;
			std::string last_action_tag;
			ptrdiff_t selection_pos = -1;
		};
		History<TextBoxHistoryEntry> history;
		Timer cursor_timer;
		size_t cursor_pos = 0;
		ptrdiff_t last_action_pos = -1;
		std::string last_action_tag;
		ptrdiff_t selection_pos = -1;
		glvx::Vector2f drag_start_pos;
		size_t dragging_start_char = 0;
		bool edit_mode = false;
		bool highlighted = false;
		bool left_button_pressed = false;
		bool dragging_begun = false;
		bool fail_state = false;
		bool process_text_entered_event = true;
		glvx::Color background_color = glvx::Color(50, 50, 50);
		glvx::Color highlight_color = glvx::Color(100, 100, 100);
		glvx::Color text_color = glvx::Color(255, 255, 255);
		glvx::Color editor_color = glvx::Color(255, 255, 255);
		glvx::Color editor_text_color = glvx::Color(0, 0, 0);
		glvx::Color selection_color = glvx::Color(128, 200, 255);
		glvx::Color fail_background_color = glvx::Color(128, 0, 0);
		glvx::Color editor_fail_background_color = glvx::Color(255, 128, 128);

	};

}
