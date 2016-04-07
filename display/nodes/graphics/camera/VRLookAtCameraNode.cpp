/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/nodes/graphics/camera/VRLookAtCameraNode.h>
#include "display/renderers/state/VRGraphicsState.h"

namespace MinVR {

VRLookAtCameraNode::VRLookAtCameraNode(VRDataIndex& index, std::string cameraName) {
	m_position = index.getValue("cameraPosition", cameraName);
	m_lookAtDirection = index.getValue("cameraLookAtDirection", cameraName);
	m_upDirection = index.getValue("cameraUpDirection", cameraName);
	m_fieldOfView = index.getValue("fieldOfView", cameraName);
	m_nearClip = index.getValue("nearClip", cameraName);
	m_farClip = index.getValue("farClip", cameraName);
}

VRLookAtCameraNode::~VRLookAtCameraNode() {

}

void VRLookAtCameraNode::render(VRRenderer& renderer) {
	renderer.pushState();
	std::cout << "look at camera" << std::endl;
	VRGraphicsState state(renderer.getState());
	state.setCameraFrame(VRMatrix4::translation(m_position));
	renderAtLeaf(renderer);
	renderer.popState();
}

void VRLookAtCameraNode::renderSceneAtLeaf(VRRenderer& renderer) {
	std::cout << "Scene callback for look at camera" << std::endl;
	VRLeafRenderedNode::renderSceneAtLeaf(renderer);
	std::cout << "Scene callback for look at camera" << std::endl;
}

} /* namespace MinVR */

