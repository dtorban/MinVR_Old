/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <graphics/VRGraphicsHandler.h>

namespace MinVR {

VRGraphicsHandler::VRGraphicsHandler() {
	// TODO Auto-generated constructor stub

}

VRGraphicsHandler::~VRGraphicsHandler() {
	// TODO Auto-generated destructor stub
}

void VRGraphicsHandler::onVRRenderScene(VRDataIndex* renderState,
		VRDisplayNode* callingNode) {
	VRGraphicsState state(*renderState);
	onVRRenderScene(state);
}

void VRGraphicsHandler::onVRRenderContext(VRDataIndex* renderState,
		VRDisplayNode* callingNode) {
	VRGraphicsState state(*renderState);
	onVRRenderContext(state);
}

} /* namespace MinVR */
