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
	m_position = index.getValue("position", cameraName);
	m_lookAtDirection = index.getValue("lookAtDirection", cameraName);
	m_upDirection = index.getValue("upDirection", cameraName);
	m_fieldOfView = index.getValue("fieldOfView", cameraName);
	m_nearClip = index.getValue("nearClip", cameraName);
	m_farClip = index.getValue("farClip", cameraName);
}

VRLookAtCameraNode::~VRLookAtCameraNode() {

}

void VRLookAtCameraNode::render(VRRenderer& renderer) {
	renderer.pushState();
	VRGraphicsState state(renderer.getState());
	state.setCameraFrame(VRMatrix4::translation(m_position));
	renderAtLeaf(renderer);
	renderer.popState();
}

void VRLookAtCameraNode::renderSceneAtLeaf(VRRenderer& renderer) {
	VRGraphicsState state(renderer.getState());
	state.setViewMatrix(calculateViewMatrix(state.getCameraPosition(), m_lookAtDirection, m_upDirection));
	VRLeafRenderedNode::renderSceneAtLeaf(renderer);
}

VRMatrix4 VRLookAtCameraNode::calculateViewMatrix(const VRVector3& eye, const VRVector3& lookAt, const VRVector3& up) const
{
	VRVector3 zaxis = lookAt*-1.0;
	VRVector3 xaxis = (up.cross(zaxis)).normalize();
	VRVector3 yaxis = zaxis. cross(xaxis);

	VRMatrix4 view = VRMatrix4(
	       xaxis.x, xaxis.y, xaxis.z, -xaxis.dot(eye),
	       yaxis.x, yaxis.y, yaxis.z, -yaxis.dot(eye),
	       zaxis.x, zaxis.y, zaxis.z, -zaxis.dot(eye),
	       0,       0,       0,     1 );

    return (view);
}

} /* namespace MinVR */

