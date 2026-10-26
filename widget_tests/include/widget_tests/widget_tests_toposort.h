#pragma once

#include "widgets/widget.h"
#include "test_lib/test.h"
#include "widget_tests/widget_test.h"
#include <random>

struct Node;

class WidgetTestsToposort : public WidgetTest {
public:
	WidgetTestsToposort(const std::string& name, test::TestModule* parent, const std::vector<TestNode*>& required_nodes = { });
	void toposortEmptyTest(test::Test& test);
	void toposort1NodeTest(test::Test& test);
	void toposort3NodesTest(test::Test& test);
	void toposort4NodesDiamondTest(test::Test& test);
	void toposort5NodesXTest(test::Test& test);
	void toposort5NodesRandomTest(test::Test& test);
	void toposortHairTest(test::Test& test);
	void toposortInverseHairTest(test::Test& test);
	void toposortLoopExceptionTest(test::Test& test);
	void toposortLoopEventTest(test::Test& test);
	void toposortTwoLoopsTest(test::Test& test);
	void toposortForkingLoopTest(test::Test& test);
	void toposortTrianglesLoopTest(test::Test& test);

private:
	// Fixed seed: every call produces the same permutation, independent of any
	// shared RNG state, so each test's input (and its exact-order loop
	// expectations) is stable no matter how the test set grows or reorders.
	//
	// Deliberately NOT std::shuffle: the standard only requires its result to
	// be uniformly distributed, not the algorithm itself. MSVC shuffles with
	// plain modulo while libstdc++ uses rejection sampling, so the same seeded
	// mt19937 stream is consumed differently and the two STLs produce different
	// permutations. The explicit Fisher-Yates below is pinned to this exact
	// seed and arithmetic, so it yields the identical permutation on every
	// platform.
	static constexpr unsigned int shuffle_seed = 0x435EE8Bu;
	bool layer_contains(const std::vector<Node*>& layer, const std::string& name);
	template <typename TVec>
	TVec get_shuffled(const TVec& vec) {
		TVec copy(vec);
		std::mt19937 rng(shuffle_seed);
		for (size_t i = copy.size(); i > 1; --i) {
			const size_t j = rng() % i;
			std::swap(copy[i - 1], copy[j]);
		}
		return copy;
	}

};

struct Node {
	std::string str;
	std::vector<Node*> parents;
	Node(const std::string& str);
	void addParent(Node* parent);
	static const std::vector<Node*>& getParents(Node* node);
};

class NodeList {
public:
	Node* createNode(const std::string& name);
	const std::vector<Node*>& getNodes() const;
private:
	CompVectorUptr<Node> nodes;
};
