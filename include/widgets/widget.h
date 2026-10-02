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
#include <functional>
#include "widgets/widgets_common.h"
#include "widgets/drawing.h"
#include "widgets/render_texture.h"
#include "widgets/widget_transform.h"
#include "widgets/widget_update_queue.h"
#include "widgets/widget_render_queue.h"
#include "widgets/widget_unclipped_region.h"
#include "widgets/widget_parent_chain.h"
#include "common/compvector.h"
#include "common/searchindex.h"
#include "common/event.h"

namespace fw {

	struct WidgetVisibility {
		bool addedToRoot = false;
		bool allParentsVisible = false;
		bool renderableSetting = false;
		bool visibleSetting = false;
		bool onScreen = false;
		bool nonZeroSize = false;
		bool hasUnclippedRegion = false;
		bool opaque = false;
		//bool notCovered = false;
	};

	enum class ColorType {
		VERTEX,
		TEXTURE,
		MULTIPLIED
	};

	class WidgetList;

	// Adding methods:
	// If method changes widget, check this:
	// wAssert(!widget_list.isLocked());
	// And if if changes render queue:
	// widget_list.render_queue.invalidate();

	class Widget {
	public:
		enum class WidgetType {
			None,
			Button,
			Canvas,
			Checkbox,
			Container,
			Dropdown,
			Empty,
			Polygon,
			Rectangle,
			ScrollArea,
			Text,
			Textbox,
			TreeView,
			Window,
		};
		enum class Anchor {
			CUSTOM,
			TOP_LEFT,
			TOP_CENTER,
			TOP_RIGHT,
			CENTER_LEFT,
			CENTER,
			CENTER_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_CENTER,
			BOTTOM_RIGHT,
		};
		enum class Alignment {
			ALIGN_TOP,
			ALIGN_CENTER,
			ALIGN_BOTTOM,
			ALIGN_LEFT,
			ALIGN_RIGHT
		};
		enum class SizePolicy {
			NONE,
			CHILDREN,
			PARENT,
			EXPAND
		};
		enum class FocusableType {
			NONE,
			NORMAL, // can click on other widgets right away
			MODAL // must click away first
		};
		ptrdiff_t debug_id = -1;

		Event<const glvx::Vector2f&> OnLeftPress;
		Event<const glvx::Vector2f&> OnRightPress;
		Event<const glvx::Vector2f&> OnGlobalLeftRelease;
		Event<const glvx::Vector2f&> OnBlockableLeftRelease;
		Event<const glvx::Vector2f&> OnGlobalRightRelease;
		Event<const glvx::Vector2f&> OnBlockableRightRelease;
		Event<const glvx::Vector2f&> OnLeftClick;
		Event<const glvx::Vector2f&> OnRightClick;
		Event<const glvx::Vector2f&, float> OnScrollX;
		Event<const glvx::Vector2f&, float> OnScrollY;
		Event<const glvx::Vector2f&> OnMouseMoved;
		Event<const glvx::Vector2f&> OnMouseEnter;
		Event<const glvx::Vector2f&> OnMouseExit;
		Event<const glvx::Vector2f&> OnProcessMouse;
		Event<glvx::Mouse::Button, const glvx::Vector2f&> OnProcessDragGesture;
		Event<> OnFocused;
		Event<> OnFocusLost;
		Event<> OnPreUpdate;
		Event<> OnPostUpdate;
		Event<glvx::RenderTarget&> OnBeforeGlobalRender;
		Event<glvx::RenderTarget&> OnBeforeRender;
		Event<glvx::RenderTarget&> OnAfterRender;
		Event<glvx::RenderTarget&> OnAfterGlobalRender;
		Event<unsigned int, unsigned int> OnWindowResized;
		std::function<CursorType()> GetCursorType = []() { return CursorType::Arrow; };

		Widget(WidgetList& list);
		Widget(const Widget& other);
		virtual ~Widget();
		WidgetType getType() const;
		bool isContainer() const;
		bool isMouseOver() const;
		void updateMouseState(const glvx::Vector2f& mouse_pos);
		glvx::Vector2f getRelativeMousePos() const;
		virtual bool isVisualPositionQuantized() const;
		bool isRenderable() const;
		bool isVisible() const;
		bool isClickThrough() const;
		bool getChildrenLocked() const;
		WidgetVisibility checkVisibility() const;
		bool containsPoint(const glvx::Vector2f& point, bool include_upper_bound = false) const;
		bool unclippedRegionContainsPoint(const glvx::Vector2f& point, bool include_upper_bound = false) const;
		void processLeftPress(const glvx::Vector2f& pos, bool became_focused);
		void processRightPress(const glvx::Vector2f& pos);
		void processGlobalLeftRelease(const glvx::Vector2f& pos);
		void processBlockableLeftRelease(const glvx::Vector2f& pos);
		void processGlobalRightRelease(const glvx::Vector2f& pos);
		void processBlockableRightRelease(const glvx::Vector2f& pos);
		void processMouseMove(const glvx::Vector2f& pos);
		void processScrollX(const glvx::Vector2f pos, float delta);
		void processScrollY(const glvx::Vector2f pos, float delta);
		void processMouse(const glvx::Vector2f& pos);
		WidgetList& getWidgetList() const;
		FocusableType getFocusableType() const;
		bool isFocused() const;
		bool getForceCustomCursor() const;
		const std::string& getName() const;
		const std::string& getFullName() const;
		bool getClipChildren() const;
		GlobalRenderLayer getGlobalRenderLayer() const;
		size_t getLocalRenderLayer() const;
		size_t getParentLocalRenderLayer() const;
		bool getQuantizeRenderedPosition() const;
		glvx::Shader* getShader() const;
		Widget* getParent() const;
		const CompVector<Widget*>& getParentChain() const;
		const CompVector<Widget*>& getChildren() const;
		size_t getChildrenCount() const;
		CompVector<Widget*> getAllChildren() const;
		Widget* getChild(size_t index) const;
		Widget* tryFind(const std::string& name) const;
		Widget* find(const std::string& name) const;
		CompVector<Widget*> getRenderQueue() const;
		virtual glvx::FloatRect getLocalBounds() const = 0;
		glvx::FloatRect getParentLocalBounds() const;
		glvx::FloatRect getGlobalBounds() const;
		virtual glvx::FloatRect getVisualLocalBounds() const;
		glvx::FloatRect getVisualParentLocalBounds() const;
		glvx::FloatRect getVisualGlobalBounds() const;
		const glvx::FloatRect& getUnclippedRegion() const;
		const glvx::FloatRect& getQuantizedUnclippedRegion() const;
		glvx::RenderTexture& getRenderTexture();
		glvx::Vector2f toGlobal(const glvx::Vector2f& pos) const;
		glvx::Vector2f toLocal(const glvx::Vector2f& pos) const;
		glvx::Vector2f getSize() const;
		float getWidth() const;
		float getHeight() const;
		float getGlobalWidth() const;
		float getGlobalHeight() const;
		Anchor getParentAnchor() const;
		glvx::Vector2f getAnchorOffset() const;
		SizePolicy getSizeXPolicy() const;
		SizePolicy getSizeYPolicy() const;
		const glvx::Vector2f& getMinSize() const;
		const glvx::Vector2f& getMaxSize() const;
		// adding new targets:
		// add removeSocket to WidgetList::removeWidget method
		WidgetUpdateSocket* getPosXTarget();
		WidgetUpdateSocket* getPosYTarget();
		WidgetUpdateSocket* getSizeXTarget();
		WidgetUpdateSocket* getSizeYTarget();
		WidgetUpdateSocket* getChildrenXTarget();
		WidgetUpdateSocket* getChildrenYTarget();
		const CompVector<WidgetLink*>& getLinks() const;
		const glvx::Transform& getTransform() const;
		const glvx::Transform& getInverseTransform() const;
		const glvx::Transform& getGlobalTransform() const;
		const glvx::Transform& getParentGlobalTransform() const;
		const glvx::Transform& getInverseGlobalTransform() const;
		const glvx::Transform& getInverseParentGlobalTransform() const;
		const glvx::Vector2f& getOrigin() const;
		Anchor getOriginAnchor() const;
		const glvx::Vector2f& getPosition() const;
		glvx::Vector2f getTransformPosition() const;
		glvx::Vector2f getGlobalPosition() const;
		glvx::Vector2f getGlobalOriginPosition() const;
		glvx::Vector2f getCenter() const;
		glvx::Vector2f getGlobalCenter() const;
		glvx::Vector2f getVisualGlobalCenter() const;
		glvx::Vector2f getTop() const;
		glvx::Vector2f getLeft() const;
		glvx::Vector2f getRight() const;
		glvx::Vector2f getBottom() const;
		glvx::Vector2f getGlobalTop() const;
		glvx::Vector2f getGlobalLeft() const;
		glvx::Vector2f getGlobalRight() const;
		glvx::Vector2f getGlobalBottom() const;
		glvx::Vector2f getTopLeft() const;
		glvx::Vector2f getTopRight() const;
		glvx::Vector2f getBottomLeft() const;
		glvx::Vector2f getBottomRight() const;
		glvx::Vector2f getGlobalTopLeft() const;
		glvx::Vector2f getGlobalTopRight() const;
		glvx::Vector2f getGlobalBottomLeft() const;
		glvx::Vector2f getGlobalBottomRight() const;
		glvx::Vector2f getVisualGlobalTopLeft() const;
		glvx::Vector2f getVisualGlobalTopRight() const;
		glvx::Vector2f getVisualGlobalBottomLeft() const;
		glvx::Vector2f getVisualGlobalBottomRight() const;
		float getRotation() const;
		virtual const glvx::Color& getFillColor() const = 0;
		float getAlphaMultiplier() const;
		virtual void setSize(float width, float height);
		void setSize(const glvx::Vector2f& size);
		void setWidth(float width);
		void setHeight(float height);
		void setSizeKeepPos(float width, float height);
		void setSizeKeepPos(const glvx::Vector2f& size);
		void setOrigin(Anchor anchor);
		void setOrigin(float x, float y);
		void setOrigin(const glvx::Vector2f& origin);
		void setOriginKeepPos(Anchor anchor);
		void setOriginKeepPos(float x, float y);
		void setOriginKeepPos(const glvx::Vector2f& origin);
		void setParentAnchor(Anchor anchor);
		void setAnchorOffset(float x, float y);
		void setAnchorOffset(const glvx::Vector2f& offset);
		void setAnchorOffsetX(float x);
		void setAnchorOffsetY(float y);
		void setSizeXPolicy(SizePolicy policy);
		void setSizeYPolicy(SizePolicy policy);
		void setSizePolicy(SizePolicy policy);
		void setMinSize(float width, float height);
		void setMinSize(const glvx::Vector2f& size);
		void setMaxSize(float width, float height);
		void setMaxSize(const glvx::Vector2f& size);
		virtual void setFillColor(const glvx::Color& color) = 0;
		void setPosition(float x, float y);
		void setPosition(const glvx::Vector2f& position);
		void setPositionX(float x);
		void setPositionY(float y);
		void setTransformPosition(float x, float y);
		void setTransformPosition(const glvx::Vector2f& position);
		void setGlobalPosition(float x, float y);
		void setGlobalPosition(const glvx::Vector2f& position);
		void setGlobalPositionX(float x);
		void setGlobalPositionY(float y);
		void setRotation(float angle);
		virtual void setRenderable(bool value);
		void setVisible(bool value);
		void toggleVisible();
		void setClickThrough(bool value);
		void setFocusableType(FocusableType value);
		void setParentSilent(Widget* new_parent);
		void setParent(Widget* new_parent);
		void setParentKeepPosSilent(Widget* new_parent);
		void setParentKeepPos(Widget* new_parent);
		void moveToIndex(size_t index);
		void moveToTop();
		void lockChildren();
		void unlockChildren();
		WidgetLink* addLink(
			const std::string& name,
			const std::vector<WidgetUpdateTarget*>& targets,
			const ExecuteFuncType& func
		);
		WidgetLink* addLink(
			const std::string& name,
			WidgetUpdateTarget* target,
			const ExecuteFuncType& func
		);
		WidgetLink* addLink(
			const std::string& name,
			const TargetsFuncType& targets_func,
			const ExecuteFuncType& func
		);
		void removeLink(WidgetLink* link);
		void setForceCustomCursor(bool value);
		void setName(const std::string& new_name);
		void setClipChildren(bool value);
		void setGlobalRenderLayer(GlobalRenderLayer layer);
		void setLocalRenderLayer(size_t layer);
		void setParentLocalRenderLayer(size_t layer);
		void setQuantizeRenderedPosition(bool value);
		void setShader(glvx::Shader* shader);
		void setAlphaMultiplier(float value);
		void removeFocus();
		void processKeyboardEvent(const glvx::Event& event);
		void render(glvx::RenderTarget& target);
		void renderBounds(glvx::RenderTarget& target, const glvx::Color& color, bool include_children, bool transformed);
		void renderOrigin(glvx::RenderTarget& target, bool include_children);
		void setDebugRender(bool value);
		void remove(bool with_clildren = true);

	protected:
		friend class WidgetList;
		friend class WidgetTransform;
		friend class WidgetUnclippedRegion;
		friend class WidgetLink;
		friend class WidgetUpdateSocket;
		friend class WidgetUpdateQueue;
		friend class WidgetParentChain;
		friend class PendingMove;
		friend class PendingDelete;
		WidgetType type = WidgetType::None;
		std::string name = "<unnamed>";
		std::string full_name;
		WidgetList& widget_list;
		WidgetTransform transforms = WidgetTransform(this);
		Widget* parent = nullptr;
		WidgetParentChain parent_chain = WidgetParentChain(*this);
		CompVector<Widget*> children;
		bool children_locked = false;
		SearchIndexMultiple<std::string, Widget*> children_names;
		glvx::Shader* shader = nullptr;
		GlobalRenderLayer global_layer = GlobalRenderLayer::BASE;
		std::map<Widget*, size_t> local_layers;
		Anchor origin_anchor = Anchor::CUSTOM;
		Anchor parent_anchor = Anchor::CUSTOM;
		glvx::Vector2f anchor_offset = glvx::Vector2f(0.0f, 0.0f);
		WidgetUnclippedRegion unclipped_region = WidgetUnclippedRegion(this);
		SizePolicy size_policy_x = SizePolicy::NONE;
		SizePolicy size_policy_y = SizePolicy::NONE;
		WidgetUpdateSocket pos_x_target = WidgetUpdateSocket(this, WidgetUpdateType::POS_X);
		WidgetUpdateSocket pos_y_target = WidgetUpdateSocket(this, WidgetUpdateType::POS_Y);
		WidgetUpdateSocket size_x_target = WidgetUpdateSocket(this, WidgetUpdateType::SIZE_X);
		WidgetUpdateSocket size_y_target = WidgetUpdateSocket(this, WidgetUpdateType::SIZE_Y);
		WidgetUpdateSocket children_x_target = WidgetUpdateSocket(this, WidgetUpdateType::CHILDREN_X);
		WidgetUpdateSocket children_y_target = WidgetUpdateSocket(this, WidgetUpdateType::CHILDREN_Y);
		CompVectorUptr<WidgetLink> links;
		glvx::Vector2f min_size;
		glvx::Vector2f max_size = glvx::Vector2f(-1.0f, -1.0f); // negative values - unlimited
		bool visible = true;
		bool renderable = true;
		bool quantize_position = true;
		FocusableType focusable_type = FocusableType::NONE;
		bool click_through = true;
		bool clip_children = false;
		bool mouseIn = false;
		bool is_left_pressed = false;
		bool is_right_pressed = false;
		bool force_custom_cursor = false;
		bool debug_render = false;
		float alpha_multiplier = 1.0f;

		glvx::Vector2f anchorToPos(Anchor p_anchor, const glvx::Vector2f& size);
		virtual glvx::Drawable* getDrawable() = 0;
		virtual const glvx::Drawable* getDrawable() const = 0;
		virtual glvx::Transformable* getTransformable() = 0;
		virtual const glvx::Transformable* getTransformable() const = 0;
		virtual glvx::Vector2f getRenderPositionOffset() const;
		virtual void setSizeInternal(float width, float height) = 0;
		void setOriginInternal(float x, float y);
		void setOriginInternal(const glvx::Vector2f& origin);
		void setSizeInternal(const glvx::Vector2f& size);
		void setRenderIterations(size_t iterations);
		virtual Widget* clone(bool with_children = true) = 0;
		virtual void addChild(Widget* child);
		virtual void removeChild(Widget* child);
		void removeSocket(WidgetUpdateSocket* socket);
		void updateOrigin();
		void preUpdate();
		void postUpdate();
		void updatePositionX();
		void updatePositionY();
		virtual void updateSizeX();
		virtual void updateSizeY();
		virtual void updateChildrenX();
		virtual void updateChildrenY();
		virtual void internalPreUpdate();
		virtual void internalPostUpdate();
		virtual void internalOnSetParent(Widget* parent);
		virtual void internalOnLeftPress(const glvx::Vector2f& pos, bool became_focused);
		virtual void internalOnRightPress(const glvx::Vector2f& pos);
		virtual void internalOnGlobalLeftRelease(const glvx::Vector2f& pos);
		virtual void internalOnBlockableLeftRelease(const glvx::Vector2f& pos);
		virtual void internalOnGlobalRightRelease(const glvx::Vector2f& pos);
		virtual void internalOnBlockableRightRelease(const glvx::Vector2f& pos);
		virtual void internalOnLeftClick(const glvx::Vector2f& pos);
		virtual void internalOnRightClick(const glvx::Vector2f& pos);
		virtual void internalOnScrollX(const glvx::Vector2f& pos, float delta);
		virtual void internalOnScrollY(const glvx::Vector2f& pos, float delta);
		virtual void internalProcessKeyboardEvent(const glvx::Event& event);
		virtual void internalProcessMouse(const glvx::Vector2f& pos);
		virtual void internalOnMouseMoved(const glvx::Vector2f& pos);
		virtual void internalOnMouseEnter(const glvx::Vector2f& pos);
		virtual void internalOnMouseExit(const glvx::Vector2f& pos);
		virtual void internalOnFocused();
		virtual void internalOnFocusLost();
		virtual void internalOnBeforeRender();
		virtual void internalOnAfterRender();

	private:
		WidgetVisibility visibility;
		RenderTexture render_textures;
		size_t render_iterations = 1;
		glvx::View render_view;

		std::string calcFullName() const;
		void updateFullName();
		void updateVisibility();
		void updateRenderTexture(const glvx::FloatRect& texture_bounds);
		void moveChildToIndex(Widget* child, size_t index);

	};

}
