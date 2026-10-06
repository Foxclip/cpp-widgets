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

namespace fw {

	class Widget;

	class WidgetTransform {
	public:
		WidgetTransform(Widget* widget);
		const glvx::Transform& getTransform() const;
		const glvx::Transform& getInverseTransform() const;
		const glvx::Transform& getGlobalTransform() const;
		const glvx::Transform& getInverseGlobalTransform() const;
		const glvx::Vector2f& getPosition() const;
		glvx::Angle getRotation() const;
		const glvx::Vector2f& getScale() const;
		const glvx::Vector2f& getOrigin() const;
		void invalidateTransform();
		void invalidateGlobalTransform();
		void setPosition(float x, float y);
		void setPosition(const glvx::Vector2f& position);
		void setGlobalPosition(float x, float y);
		void setGlobalPosition(const glvx::Vector2f& position);
		void setRotation(glvx::Angle angle);
		void setScale(const glvx::Vector2f& scale);
		void setOrigin(float x, float y);
		void setOrigin(const glvx::Vector2f origin);
		void copyFrom(const WidgetTransform& other);

	private:
		Widget* widget = nullptr;
		glvx::Vector2f position;
		glvx::Angle rotation;
		glvx::Vector2f scale = glvx::Vector2f(1.0f, 1.0f);
		glvx::Vector2f origin;
		mutable glvx::Transform transform;
		mutable glvx::Transform inv_transform;
		mutable glvx::Transform global_transform;
		mutable glvx::Transform inv_global_transform;
		mutable bool transform_valid = false;
		mutable bool inv_transform_valid = false;
		mutable bool global_transform_valid = false;
		mutable bool inv_global_transform_valid = false;

		void recalcTransform() const;
		void recalcInverseTransform() const;
		void recalcGlobalTransform() const;
		void recalcInverseGlobalTransform() const;

	};

}
