/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRRENDERHANDLER_H_
#define VRRENDERHANDLER_H_

#include "VRRenderState.h"

namespace MinVR {

class VRRenderHandler {
public:
	virtual ~VRRenderHandler() {}

	// ------------ User defined render functions: -------------
	// allows a user to specify a function which handles per frame functionality.
	// for example, a user may want to use this function to update VBO objects per frame if needed.
	virtual void renderContextCallback(VRRenderState& state) = 0;
	// render() allows a user to specify a render function which displays application specific graphics.
	virtual void renderSceneCallback(VRRenderState& state) {}
};

} /* namespace MinVR */

#endif /* VRRENDERHANDLER_H_ */
