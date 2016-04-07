/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRSTEREONODE_H_
#define VRSTEREONODE_H_

#include "display/VRDisplayNode.h"
#include "display/factories/VRDisplayFactory.h"
#include "display/nodes/scope/VRLeafRenderedNode.h"

namespace MinVR {

class VRStereoNode : public VRLeafRenderedNode {
public:
	VRStereoNode(double interocularDistance);
	virtual ~VRStereoNode();

	virtual void render(VRRenderer& renderer);
	virtual void renderSceneAtLeaf(VRRenderer& renderer);

protected:
	virtual int getNumPasses() = 0;
	virtual void preRenderPass(VRRenderer& renderer, int passNum) = 0;
	virtual void postRenderPass(VRRenderer& renderer, int passNum) = 0;

private:
	double m_interocularDistance;
};

class VRStereoFactory : public VRDisplayFactory {
public:
	VRStereoFactory() {}
	virtual ~VRStereoFactory() {}

	VRDisplayNode* create(VRDataIndex& config, const std::string nameSpace, VRDisplayFactory& baseFactory);
};

} /* namespace MinVR */

#endif /* VRSTEREOFORMATTER_H_ */
