/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRGRAPHICSHANDLER_H_
#define VRGRAPHICSHANDLER_H_

#include "main/VRRenderHandler.h"
#include "VRGraphicsState.h"

namespace MinVR {

/**
 * VRGraphicsHandler wraps a VRGraphicsState around a VRDataIndex so that users can
 * use the VRGraphisState functions to retrieve graphics specific variables rather
 * than generic structures.
 */
class VRGraphicsHandler : public VRRenderHandler {
public:
	virtual ~VRGraphicsHandler() {}

	/**
	 * onVRRenderScene handles drawing graphics for each viewport.
	 */
	virtual void onVRRenderScene(VRGraphicsState& renderState) = 0;

	/**
	 * onVRRenderContext handles graphics specific implementations for each window.
	 */
	virtual void onVRRenderContext(VRGraphicsState& renderState) {}

	/// onVRRenderScene(VRDataIndex) calls onVRRenderScene(VRGraphicsState)
	void onVRRenderScene(VRDataIndex *renderState, VRDisplayNode *callingNode);
	/// onVRRenderContext(VRDataIndex) calls onVRRenderContext(VRGraphicsState)
	void onVRRenderContext(VRDataIndex *renderState, VRDisplayNode *callingNode);
};

} /* namespace MinVR */

#endif /* VRGRAPHICSHANDLER_H_ */
