/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/VRRenderHelper.h>

namespace MinVR {

VRRenderHelper::VRRenderHelper(VRDataIndex& index) : index(index) {
	// TODO Auto-generated constructor stub

}

VRRenderHelper::~VRRenderHelper() {
	// TODO Auto-generated destructor stub
}

int VRRenderHelper::getWindowX() {
	return getValue("WindowX", 0);
}

int VRRenderHelper::getWindowY() {
	return getValue("WindowY", 0);
}

int VRRenderHelper::getWindowWidth() {
	return getValue("WindowWidth", 0);
}

int VRRenderHelper::getWindowHeight() {
	return getValue("WindowHeight", 0);
}

int VRRenderHelper::getSharedContextGroupID() {
	return getValue("SharedContextGroupID", 0);
}

int VRRenderHelper::getWindowID() {
	return getValue("WindowID", 0);
}

bool VRRenderHelper::firstRenderCall() {
	return getValue("InitRender", 0);
}

bool VRRenderHelper::isConsole() {
	return getValue("IsConsole", 0);
}

VRMatrix4 VRRenderHelper::getProjectionMatrix() {
	static VRMatrix4 proj;
	return getValue("ProjectionMatrix", proj);
}

VRMatrix4 VRRenderHelper::getViewMatrix() {
	static VRMatrix4 view;
	return getValue("ViewMatrix", view);
}

} /* namespace IVFramework */
