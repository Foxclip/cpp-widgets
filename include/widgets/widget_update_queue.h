#pragma once

#include <string>
#include <vector>
#include "widgets/widget_link.h"

namespace fw {

	class Widget;
	class WidgetList;

	class WidgetUpdateQueue {
	public:
		WidgetUpdateQueue(WidgetList& widget_list);
		void update();
		const std::vector<std::vector<WidgetUpdateTarget*>>& get() const;
		// The dependency structure only changes when widgets/links are added,
		// removed, re-parented, or their anchors/size policies/visibility
		// change, so the toposorted order is cached until invalidate() is
		// called from those mutation points.
		void invalidate();

	private:
		WidgetList& widget_list;
		std::vector<std::vector<WidgetUpdateTarget*>> queue;
		bool toposort_valid = false;

		static std::vector<WidgetUpdateTarget*> getParents(const WidgetUpdateTarget* entry);
	};

}
