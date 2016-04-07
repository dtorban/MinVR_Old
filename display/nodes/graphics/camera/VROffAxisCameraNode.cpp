/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/nodes/graphics/camera/VROffAxisCameraNode.h>
#include "display/nodes/graphics/camera/VRLookAtCameraNode.h"

namespace MinVR {

VROffAxisCameraNode::VROffAxisCameraNode() {
	// TODO Auto-generated constructor stub

}

VROffAxisCameraNode::~VROffAxisCameraNode() {
	// TODO Auto-generated destructor stub
}

void VROffAxisCameraNode::render(VRRenderer& renderer) {
	renderer.pushState();
	std::cout << "off axis camera" << std::endl;
	renderAtLeaf(renderer);
	renderer.popState();
}

void VROffAxisCameraNode::renderSceneAtLeaf(VRRenderer& renderer) {
	std::cout << "Scene callback for off axis" << std::endl;
	VRLeafRenderedNode::renderSceneAtLeaf(renderer);
	std::cout << "Scene callback for off axis2" << std::endl;
}

VRDisplayNode* VRCameraDisplayFactory::create(VRDataIndex& config,
		const std::string nameSpace, std::string type,
		VRDisplayFactory& baseFactory) {

	VRDisplayNode* cameraNode = NULL;

	if (type == "offAxisCamera")
	{
		cameraNode = new VROffAxisCameraNode();
	}
	else if (type == "lookAtCamera")
	{
		std::string camera = config.getValue("camera", nameSpace);
		cameraNode = new VRLookAtCameraNode(config, "/" + camera);
	}

	if (cameraNode)
	{
		createChildren(cameraNode, baseFactory, config, nameSpace);
	}

	return cameraNode;
}

} /* namespace MinVR */

