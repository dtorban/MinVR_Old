/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRCALLBACKRENDERER_H_
#define VRCALLBACKRENDERER_H_

#include "display/renderers/VRRenderer.h"

namespace MinVR {

/*
 * VRCallbackRenderer is a simple renderer which takes two function callback pointers as input.
 * These functions are registered as callback functions for the VRRenderer.  The state is controlled
 * in the VRBasicRenderer subclass.
 */
class VRCallbackRenderer : public VRRenderer, public VRRenderHandler {
public:
	typedef void (*MethodType)(VRRenderState&);

	VRCallbackRenderer(MethodType sceneCallback, MethodType contextCallback = NULL);
	virtual ~VRCallbackRenderer();

	void renderContextCallback(VRRenderState& state);
	void renderSceneCallback(VRRenderState& state);

private:
	MethodType m_sceneCallback;
	MethodType m_contextCallback;
};

} /* namespace MinVR */

#endif /* VRFUNCTIONRENDERCALLBACK_H_ */
