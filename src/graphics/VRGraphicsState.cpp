/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRGraphicsState.h"

namespace MinVR {

VRGraphicsState::VRGraphicsState(VRDataIndex& internalState) : internalState(&internalState) {
	if (internalState.exists("ProjectionMatrix", "/")) {
		projectionMatrix = internalState.getValue("ProjectionMatrix", "/");
	}
	else {
		projectionMatrix = VRMatrix4();
	}

	if (internalState.exists("ViewMatrix", "/")) {
		viewMatrix = internalState.getValue("ViewMatrix", "/");
	}
	else {
		viewMatrix = VRMatrix4();
	}

	initRender = (int)internalState.getValue("InitRender","/");
}

const double * VRGraphicsState::getProjectionMatrix() const {
	return projectionMatrix.m;
}

const double * VRGraphicsState::getViewMatrix() const {
	return viewMatrix.m;
}

bool VRGraphicsState::isInitialRenderCall() const {
	return initRender;
}

}
