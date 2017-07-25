#pragma once
#include <string>

namespace NodeType {
	enum NodeType {
		NullNode = -1,						// Change this when extended
		BaseNode,							// Base Node. Should only ever be one
		StructureNode,						// Used as a "folder", has no special characteristics
		TextNode,							// Draws some text at a position in 2D screen-space
		Island								// Class for a simple island that is rendered to the screen
	};

	inline std::string toString(NodeType type) {
		switch (type) {
		case NullNode:
			return "NullNode";
		case BaseNode:
			return "BaseNode";
		case StructureNode:
			return "StructureNode";
		case TextNode:
			return "TextNode";
		case Island:
			return "Island";
		default:
			return "UnknownType";
		}
	}
}