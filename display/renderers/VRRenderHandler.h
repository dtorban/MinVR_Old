/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRBASICRENDERER_H_
#define VRBASICRENDERER_H_

#include "VRRenderState.h"

namespace MinVR {

/*
 * VRRenderHandler has a simple implementation of controlling state.  When a state is pushed,
 * a new namespace in the VRDataIndex is created as a sub namespace to the current namespace.
 * This enables the use of the VRDataIndex for walking up a namespace tree.  The user defined
 * functions updateFrame(VRRenderState& state) and render(VRRenderState& state) still need to be
 * defined by the user.
 */
class VRRenderHandler{
public:
	VRRenderHandler();
	virtual ~VRRenderHandler();

	// The render functions use a template method pattern to call renderContextCallback(state) and renderSceneCallback(state)
	virtual void renderContextCallback();
	virtual void renderSceneCallback();

	// ------------ User defined render functions: -------------
	// allows a user to specify a function which handles per frame functionality.
	// for example, a user may want to use this function to update VBO objects per frame if needed.
	virtual void renderContextCallback(VRRenderState& state) = 0;
	// render() allows a user to specify a render function which displays application specific graphics.
	virtual void renderSceneCallback(VRRenderState& state) = 0;

	// ------------ State control functions: -------------
	// Returns the current state
	virtual VRRenderState& getState();
	// Pushes a state, which usually implies that previous state information is available and changes to the state
	// will not affect the previous states
	virtual void pushState();
	// Pops the state, so the state will return to a previous state and undo changes from the popped state.
	virtual void popState();
	// Resets the state
	virtual void resetState();

private:
	VRRenderState m_state;
	int m_nodeNum;
	std::vector<std::string> m_nameSpaces;
};

} /* namespace MinVR */

#endif /* VRBASICRENDERER_H_ */
