/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef LOOKATCAMERANODE_H_
#define LOOKATCAMERANODE_H_

#include "display/VRDisplayNode.h"
#include "display/factories/VRTypedDisplayFactory.h"
#include "display/nodes/scope/VRLeafRenderedNode.h"
#include "math/VRMath.h"

namespace MinVR {

class VRLookAtCameraNode : public VRLeafRenderedNode {
public:
	VRLookAtCameraNode(VRDataIndex& index, std::string cameraName);
	virtual ~VRLookAtCameraNode();

	void render(VRRenderer& renderer);

	void renderSceneAtLeaf(VRRenderer& renderer);

private:
	VRVector3 m_position;
	VRVector3 m_lookAtDirection;
	VRVector3 m_upDirection;
	double m_fieldOfView;
	double m_nearClip;
	double m_farClip;
};


} /* namespace MinVR */

#endif /* LOOKATCAMERANODE_H_ */
