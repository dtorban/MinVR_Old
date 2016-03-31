/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRSCOPENODE_H_
#define VRSCOPENODE_H_

#include "display/VRDisplayNode.h"

namespace MinVR {

/*
 * Super simple node that pushes the render state before rendering its
 * children and pops the render state after rendering.
 */
class VRStateScopeNode : public VRDisplayNode  {
public:
	VRStateScopeNode();
	virtual ~VRStateScopeNode();

	void render(VRRenderer& renderer);
};

} /* namespace MinVR */

#endif /* VRSCOPENODE_H_ */
