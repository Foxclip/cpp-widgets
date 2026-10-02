#include "widgets/widget.h"
#include "widgets/widget_unclipped_region.h"

namespace fw {

	WidgetUnclippedRegion::WidgetUnclippedRegion(Widget* widget) {
		this->widget = widget;
	}

	const glvx::FloatRect& WidgetUnclippedRegion::get() const {
		if (!valid) {
			recalc();
		}
		return unclippedRegion;
	}

	const glvx::FloatRect& WidgetUnclippedRegion::getQuantized() const {
		if (!valid) {
			recalc();
		}
		return quantizedUnclippedRegion;
	}

	bool WidgetUnclippedRegion::isNonZero() const {
		if (!valid) {
			recalc();
		}
		return unclippedRegion.size.x > 0 && unclippedRegion.size.y > 0;
	}

	bool WidgetUnclippedRegion::isQuantizedNonZero() const {
		if (!valid) {
			recalc();
		}
		return quantizedUnclippedRegion.size.x > 0 && quantizedUnclippedRegion.size.y > 0;
	}

	void WidgetUnclippedRegion::recalc() const {
		glvx::FloatRect result = widget->getVisualGlobalBounds();
		Widget* parent = widget->parent;
		while (parent) {
			if (parent->getClipChildren()) {
			glvx::FloatRect parent_unclipped_region = parent->getUnclippedRegion();
			glvx::FloatRect intersection;
			result.intersects(parent_unclipped_region, intersection);
			if (intersection.size.x > 0.0f && intersection.size.y > 0.0f) {
					result = intersection;
				} else {
					result = glvx::FloatRect(widget->getGlobalPosition(), glvx::Vector2f());
				}
			}
			parent = parent->parent;
		}
		unclippedRegion = result;
		quantizedUnclippedRegion = quantize_rect(result, QUANTIZE_MODE_FLOOR);
		valid = true;
	}

	void WidgetUnclippedRegion::invalidate() {
		valid = false;
		for (size_t i = 0; i < widget->children.size(); i++) {
			widget->children[i]->unclipped_region.invalidate();
		}
	}

}
