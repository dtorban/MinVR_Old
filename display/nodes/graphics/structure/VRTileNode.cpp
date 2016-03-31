/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/nodes/graphics/structure/VRTileNode.h>

namespace MinVR {

VRTileNode::VRTileNode(const VRTile& tile) : m_tile(tile) {
}

VRTileNode::~VRTileNode() {
}

void VRTileNode::render(VRRenderer& renderer) {
//	renderer.pushState();
	renderer.getState().writeValue("tile", m_tile);

	VRDisplayNode::render(renderer);

//	renderer.popState();
}

} /* namespace MinVR */
