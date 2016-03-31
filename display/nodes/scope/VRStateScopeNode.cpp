/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRStateScopeNode.h"

namespace MinVR {

VRStateScopeNode::VRStateScopeNode() {
}

VRStateScopeNode::~VRStateScopeNode() {
}

void VRStateScopeNode::render(VRRenderer& renderer) {
	renderer.pushState();
	VRDisplayNode::render(renderer);
	renderer.popState();
}

} /* namespace MinVR */
