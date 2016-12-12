/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRGraphicsHandler.h"

namespace MinVR {

void VRGraphicsHandler::onVRRenderScene(VRDataIndex* renderState,
		VRDisplayNode* callingNode) {
	// Wraps VRDataIndex inside VRGraphicsState
	VRGraphicsState state(*renderState);
	onVRRenderScene(state);
}

void VRGraphicsHandler::onVRRenderContext(VRDataIndex* renderState,
		VRDisplayNode* callingNode) {
	// Wraps VRDataIndex inside VRGraphicsState
	VRGraphicsState state(*renderState);
	onVRRenderContext(state);
}

} /* namespace MinVR */
