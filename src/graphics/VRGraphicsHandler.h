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

class VRGraphicsHandler : public VRRenderHandler {
public:
	VRGraphicsHandler();
	virtual ~VRGraphicsHandler();

	void onVRRenderScene(VRDataIndex *renderState, VRDisplayNode *callingNode);
	void onVRRenderContext(VRDataIndex *renderState, VRDisplayNode *callingNode);
	virtual void onVRRenderScene(VRGraphicsState& renderState) = 0;
	virtual void onVRRenderContext(VRGraphicsState& renderState) {}

};

} /* namespace MinVR */

#endif /* VRGRAPHICSHANDLER_H_ */
