/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef OFFAXISCAMERANODE_H_
#define OFFAXISCAMERANODE_H_

#include "display/VRDisplayNode.h"
#include "display/factories/VRTypedDisplayFactory.h"
#include "display/nodes/scope/VRLeafRenderedNode.h"
#include "math/VRMath.h"

namespace MinVR {

class VROffAxisCameraNode : public VRLeafRenderedNode {
public:
	VROffAxisCameraNode(VRDataIndex& index, std::string cameraName);
	virtual ~VROffAxisCameraNode();

	void render(VRRenderer& renderer);

	void renderSceneAtLeaf(VRRenderer& renderer);

private:
	VRMatrix4 m_defaultCameraFrame;
	double m_nearClip;
	double m_farClip;
};

class VRCameraDisplayFactory : public VRTypedDisplayFactory {
public:
	VRCameraDisplayFactory() {}
	virtual ~VRCameraDisplayFactory() {}

protected:
	VRDisplayNode* create(VRDataIndex& config, const std::string nameSpace, std::string type, VRDisplayFactory& baseFactory);
};


} /* namespace MinVR */

#endif /* OFFAXISCAMERANODE_H_ */
