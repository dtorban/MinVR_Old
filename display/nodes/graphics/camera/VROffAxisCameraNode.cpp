/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/nodes/graphics/camera/VROffAxisCameraNode.h>
#include "display/nodes/graphics/camera/VRLookAtCameraNode.h"
#include "display/renderers/state/VRGraphicsState.h"

namespace MinVR {

VROffAxisCameraNode::VROffAxisCameraNode(VRDataIndex& index, std::string cameraName) {
	m_defaultCameraFrame = index.getValue("defaultFrame", cameraName);
	m_nearClip = index.getValue("nearClip", cameraName);
	m_farClip = index.getValue("farClip", cameraName);
}

VROffAxisCameraNode::~VROffAxisCameraNode() {
}

void VROffAxisCameraNode::render(VRRenderer& renderer) {
	renderer.pushState();
	VRGraphicsState state(renderer.getState());
	state.setCameraFrame(m_defaultCameraFrame);
	renderAtLeaf(renderer);
	renderer.popState();
}

void VROffAxisCameraNode::renderSceneAtLeaf(VRRenderer& renderer) {
	VRGraphicsState state(renderer.getState());
	VRTile tile = state.getTile();

	double halfWidth = (tile.getTopRight() - tile.getTopLeft()).length() / 2.0;
	double halfHeight = (tile.getTopRight() - tile.getBottomRight()).length() / 2.0;

	VRVector3 center = (tile.getTopLeft().toVector() + tile.getTopRight().toVector() + tile.getBottomLeft().toVector() + tile.getBottomRight().toVector()) / 4.0;

	VRVector3 x = (tile.getTopRight() - tile.getTopLeft()).normalize();
	VRVector3 y = (tile.getTopLeft() - tile.getBottomLeft()).normalize();
	VRVector3 z = x.cross(y).normalize();
	VRMatrix4 tile2room(x.x, y.x, z.x, center.x,
						x.y, y.y, z.y, center.y,
						x.z, y.z, z.z, center.z,
						0, 0, 0, 1);

	VRMatrix4 room2tile = tile2room.inverse();

	VRMatrix4 camera2Room = state.getCameraFrame();
	VRMatrix4 cameraFrame = room2tile * camera2Room;
	VRVector3 cameraPos(cameraFrame(0,3), cameraFrame(1,3), cameraFrame(2,3));

	double lcamera = (-halfWidth - cameraPos.x);
	double rcamera = (halfWidth - cameraPos.x);
	double b = (-halfHeight - cameraPos.y);
	double t = (halfHeight - cameraPos.y);
	double dist = cameraPos.z;
	double k = m_nearClip / dist;

	VRMatrix4 view = VRMatrix4::translation(-cameraPos) *room2tile;

	static VRMatrix4 invertYMat(1,  0, 0, 0,
						  0, -1, 0, 0,
						  0,  0, 1, 0,
						  0,  0, 0, 1);

	VRMatrix4 projection = invertYMat * VRMatrix4::projection(lcamera*k, rcamera*k, b*k, t*k, m_nearClip, m_farClip);

	state.setProjectionMatrix(projection);
	state.setViewMatrix(view);

	VRLeafRenderedNode::renderSceneAtLeaf(renderer);
}

VRDisplayNode* VRCameraDisplayFactory::create(VRDataIndex& config,
		const std::string nameSpace, std::string type,
		VRDisplayFactory& baseFactory) {

	VRDisplayNode* cameraNode = NULL;

	if (type == "offAxisCamera")
	{
		cameraNode = new VROffAxisCameraNode(config, nameSpace);
	}
	else if (type == "lookAtCamera")
	{
		cameraNode = new VRLookAtCameraNode(config, nameSpace);
	}

	if (cameraNode)
	{
		createChildren(cameraNode, baseFactory, config, nameSpace);
	}

	return cameraNode;
}

} /* namespace MinVR */

