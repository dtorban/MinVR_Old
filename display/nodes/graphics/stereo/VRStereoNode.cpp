/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/nodes/graphics/stereo/VRStereoNode.h>
#include "display/nodes/graphics/stereo/VRSideBySideNode.h"

namespace MinVR {

VRStereoNode::VRStereoNode() {
}

VRStereoNode::~VRStereoNode() {
}

void VRStereoNode::render(VRRenderer& renderer) {
	renderAtLeaf(renderer);
}

void VRStereoNode::renderSceneAtLeaf(VRRenderer& renderer) {
	for (int passNum = 0; passNum < getNumPasses(); passNum++)
	{
		preRenderPass(renderer, passNum);
		VRLeafRenderedNode::renderSceneAtLeaf(renderer);
		postRenderPass(renderer, passNum);
	}
}

VRDisplayNode* VRStereoFactory::create(VRDataIndex& config,
		const std::string nameSpace, VRDisplayFactory& baseFactory) {

	std::string ending = "SideBySideStereo";

	if(std::equal(ending.rbegin(), ending.rend(), nameSpace.rbegin()))
	{
		VRSideBySideNode* sideBySideNode = new VRSideBySideNode();
		createChildren(sideBySideNode, baseFactory, config, nameSpace);
		return sideBySideNode;
	}

	return NULL;
}

} /* namespace MinVR */
