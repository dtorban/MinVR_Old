/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/VRGraphicsRenderState.h>

namespace MinVR {

VRGraphicsRenderState::VRGraphicsRenderState(VRDataIndex& index) : index(index) {
	// TODO Auto-generated constructor stub

}

VRGraphicsRenderState::~VRGraphicsRenderState() {
	// TODO Auto-generated destructor stub
}

int VRGraphicsRenderState::getWindowX() {
	return getValue("WindowX", 0);
}

int VRGraphicsRenderState::getWindowY() {
	return getValue("WindowY", 0);
}

int VRGraphicsRenderState::getWindowWidth() {
	return getValue("WindowWidth", 0);
}

int VRGraphicsRenderState::getWindowHeight() {
	return getValue("WindowHeight", 0);
}

int VRGraphicsRenderState::getSharedContextGroupID() {
	return getValue("SharedContextGroupID", 0);
}

int VRGraphicsRenderState::getWindowID() {
	return getValue("WindowID", 0);
}

bool VRGraphicsRenderState::firstRenderCall() {
	return getValue("InitRender", 0);
}

bool VRGraphicsRenderState::isConsole() {
	return getValue("IsConsole", 0);
}

VRMatrix4 VRGraphicsRenderState::getProjectionMatrix() {
	static VRMatrix4 proj;
	return getValue("ProjectionMatrix", proj);
}

VRMatrix4 VRGraphicsRenderState::getViewMatrix() {
	static VRMatrix4 view;
	return getValue("ViewMatrix", view);
}

} /* namespace IVFramework */
