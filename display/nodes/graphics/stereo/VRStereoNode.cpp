/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/nodes/graphics/stereo/VRStereoNode.h>
#include "display/nodes/graphics/stereo/VRSideBySideNode.h"
#include "display/renderers/state/VRGraphicsState.h"

namespace MinVR {

VRStereoNode::VRStereoNode(double interocularDistance) : m_interocularDistance(interocularDistance) {
}

VRStereoNode::~VRStereoNode() {
}

void VRStereoNode::render(VRRenderer& renderer) {
	renderAtLeaf(renderer);
}

void VRStereoNode::renderSceneAtLeaf(VRRenderer& renderer) {
	VRGraphicsState state(renderer.getState());

	for (int passNum = 0; passNum < getNumPasses(); passNum++)
	{
		VRMatrix4 cameraFrame = state.getCameraFrame();
		cameraFrame = VRMatrix4::translation(VRVector3(-m_interocularDistance/2.0 + m_interocularDistance*(passNum), 0, 0))*cameraFrame;
		renderer.pushState();
		state.setCameraFrame(cameraFrame);
		preRenderPass(renderer, passNum);
		VRLeafRenderedNode::renderSceneAtLeaf(renderer);
		postRenderPass(renderer, passNum);
		renderer.popState();
	}
}

VRDisplayNode* VRStereoFactory::create(VRDataIndex& config,
		const std::string nameSpace, VRDisplayFactory& baseFactory) {

	std::string ending = "SideBySideStereo";

	if(std::equal(ending.rbegin(), ending.rend(), nameSpace.rbegin()))
	{
		double ioc = config.getValue("interocularDistance", nameSpace);
		VRSideBySideNode* sideBySideNode = new VRSideBySideNode(ioc);
		createChildren(sideBySideNode, baseFactory, config, nameSpace);
		return sideBySideNode;
	}

	return NULL;
}

} /* namespace MinVR */
