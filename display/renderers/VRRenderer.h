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
#include "VRRenderHandler.h"

namespace MinVR {

/*
 * VRRenderer has a simple implementation of controlling state.  When a state is pushed,
 * a new namespace in the VRDataIndex is created as a sub namespace to the current namespace.
 * This enables the use of the VRDataIndex for walking up a namespace tree.  The user defined
 * functions updateFrame(VRRenderState& state) and render(VRRenderState& state) still need to be
 * defined by the user.
 */
class VRRenderer {
public:
	VRRenderer(VRRenderHandler* renderHandler);
	virtual ~VRRenderer();

	virtual VRRenderHandler& getRenderHandler() { return *m_renderHandler; }

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
	VRRenderHandler* m_renderHandler;
};

} /* namespace MinVR */

#endif /* VRBASICRENDERER_H_ */
