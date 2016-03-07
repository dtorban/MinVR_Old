/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/graphics/VRGraphicsWindowNode.h>

namespace MinVR {

VRGraphicsWindowNode::~VRGraphicsWindowNode() {
}

void VRGraphicsWindowNode::render() {
	startRender();
	waitForRenderComplete();
	synchronize();
}

void VRGraphicsWindowNode::startRender() {
	setCurrentContext();
	//TODO: load data
	VRDisplayNode::render();
	flush();
}

void VRGraphicsWindowNode::waitForRenderComplete() {
	finish();
}

void VRGraphicsWindowNode::synchronize() {
	swapBuffers();
	clearCurrentContext();
}

void VRGraphicsWindowNode::addChild(VRGraphicsWindowChild* child) {
	VRDisplayNode::addChild(child);
}

} /* namespace MinVR */