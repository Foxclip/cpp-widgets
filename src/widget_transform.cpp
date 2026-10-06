#include "widgets/widget_transform.h"
#include "widgets/widget.h"
#include <numbers>

namespace fw {

	WidgetTransform::WidgetTransform(Widget* widget) {
		this->widget = widget;
	}

	const glvx::Transform& WidgetTransform::getTransform() const {
		if (!transform_valid) {
			recalcTransform();
		}
		return transform;
	}

	const glvx::Transform& WidgetTransform::getInverseTransform() const {
		if (!inv_transform_valid) {
			recalcInverseTransform();
		}
		return inv_transform;
	}

	const glvx::Transform& WidgetTransform::getGlobalTransform() const {
		if (!global_transform_valid) {
			recalcGlobalTransform();
		}
		return global_transform;
	}

	const glvx::Transform& WidgetTransform::getInverseGlobalTransform() const {
		if (!inv_global_transform_valid) {
			recalcInverseGlobalTransform();
		}
		return inv_global_transform;
	}

	const glvx::Vector2f& WidgetTransform::getPosition() const {
		return position;
	}

	glvx::Angle WidgetTransform::getRotation() const {
		return rotation;
	}

	const glvx::Vector2f& WidgetTransform::getScale() const {
		return scale;
	}

	const glvx::Vector2f& WidgetTransform::getOrigin() const {
		return origin;
	}

	void WidgetTransform::invalidateTransform() {
		transform_valid = false;
		inv_transform_valid = false;
		invalidateGlobalTransform();
	}

	void WidgetTransform::invalidateGlobalTransform() {
		global_transform_valid = false;
		inv_global_transform_valid = false;
		for (size_t i = 0; i < widget->getChildren().size(); i++) {
			Widget* child = widget->getChildren()[i];
			child->transforms.invalidateGlobalTransform();
		}
	}

	void WidgetTransform::setPosition(float x, float y) {
		if (this->position.x == x && this->position.y == y) {
			return;
		}
		this->position.x = x;
		this->position.y = y;
		invalidateTransform();
	}

	void WidgetTransform::setPosition(const glvx::Vector2f& position) {
		if (this->position == position) {
			return;
		}
		this->position = position;
		invalidateTransform();
	}

	void WidgetTransform::setGlobalPosition(float x, float y) {
		const glvx::Transform& inv_parent_global_transform = widget->getInverseParentGlobalTransform();
		glvx::Vector2f pos = glvx::Vector2f(x, y);
		glvx::Vector2f local_pos = inv_parent_global_transform * pos;
		setPosition(local_pos);
	}

	void WidgetTransform::setGlobalPosition(const glvx::Vector2f& position) {
		setGlobalPosition(position.x, position.y);
	}

	void WidgetTransform::setRotation(glvx::Angle angle) {
		if (this->rotation.asRadians() == angle.asRadians()) {
			return;
		}
		this->rotation = angle;
		invalidateTransform();
	}

	void WidgetTransform::setScale(const glvx::Vector2f& scale) {
		if (this->scale == scale) {
			return;
		}
		this->scale = scale;
		invalidateTransform();
	}

	void WidgetTransform::setOrigin(float x, float y) {
		if (this->origin.x == x && this->origin.y == y) {
			return;
		}
		this->origin.x = x;
		this->origin.y = y;
		invalidateTransform();
	}

	void WidgetTransform::setOrigin(const glvx::Vector2f origin) {
		setOrigin(origin.x, origin.y);
	}

	void WidgetTransform::copyFrom(const WidgetTransform& other) {
		this->position = other.position;
		this->rotation = other.rotation;
		this->scale = other.scale;
		this->origin = other.origin;
	}

	void WidgetTransform::recalcTransform() const {
		glvx::Transform t = glvx::Transform();
		t.translate(position);
		t.rotate(rotation, glvx::Vector2f());
		t.scale(scale.x, scale.y);
		t.translate(-origin);
		transform = t;
		transform_valid = true;
	}

	void WidgetTransform::recalcInverseTransform() const {
		inv_transform = getTransform().getInverse();
		inv_transform_valid = true;
	}

	void WidgetTransform::recalcGlobalTransform() const {
		const glvx::Transform& transform = getTransform();
		const glvx::Transform& parent_transform = widget->getParentGlobalTransform();
		global_transform = parent_transform * transform;
		global_transform_valid = true;
	}

	void WidgetTransform::recalcInverseGlobalTransform() const {
		const glvx::Transform& transform = getTransform();
		const glvx::Transform& global_transform = getGlobalTransform();
		inv_global_transform = global_transform.getInverse();
		inv_global_transform_valid = true;
	}

}
