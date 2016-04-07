/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRSIDEBYSIDENODE_H_
#define VRSIDEBYSIDENODE_H_

#include "display/nodes/graphics/stereo/VRStereoNode.h"
#include "math/VRRect.h"

namespace MinVR {

class VRSideBySideNode : public VRStereoNode {
public:
	VRSideBySideNode(double interocularDistance);
	virtual ~VRSideBySideNode();

protected:
	int getNumPasses();
	void preRenderPass(VRRenderer& renderer, int passNum);
	void postRenderPass(VRRenderer& renderer, int passNum);

private:
	VRRect m_viewports[2];
};

} /* namespace MinVR */

#endif /* VRSIDEBYSIDENODE_H_ */
