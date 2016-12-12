/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRGraphicsState.h"
#include "math/VRMath.h"

namespace MinVR {

VRGraphicsState::VRGraphicsState(VRDataIndex& internalState) : internalState(&internalState), projectionMatrix(NULL), viewMatrix(NULL) {
}

VRGraphicsState::~VRGraphicsState() {
	if (projectionMatrix) { delete[] projectionMatrix; }
	if (viewMatrix) { delete[] viewMatrix; }
}

float* VRGraphicsState::getMatrix(VRDataIndex* state, const std::string& name) const {
	float* val = new float[16];

	VRMatrix4 mat;
	if (internalState->exists(name, "/")) {
		mat = internalState->getValue(name, "/");
	}

	for (int f = 0; f < 16; f++) {
		val[f] = mat.m[f];
	}

	return val;
}

const float * VRGraphicsState::getProjectionMatrix() {
	if (!projectionMatrix) {
		projectionMatrix = getMatrix(internalState, "ProjectionMatrix");
	}

	return projectionMatrix;
}

const float * VRGraphicsState::getViewMatrix() {
	if (!viewMatrix) {
		viewMatrix = getMatrix(internalState, "ViewMatrix");
	}

	return viewMatrix;
}

bool VRGraphicsState::isInitialRenderCall() {
	bool initRender = (int)internalState->getValue("InitRender","/") == 1;
	return initRender;
}

}
