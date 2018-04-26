/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <VROpenHapticsNode.h>
#include <iostream>

namespace MinVR {

VROpenHapticsNode::VROpenHapticsNode(const std::string& name, VRMainInterface &vrMain) : VRDisplayNode(name), vrMain(vrMain) {
}

VROpenHapticsNode::~VROpenHapticsNode() {
}

void VROpenHapticsNode::render(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {
	std::cout << "abc" << std::endl;
	renderState->addData("IsHaptics", true);
	renderHandlers = vrMain.getRenderHandlers();
	renderHandler->onVRRenderContext(*renderState);

	for (int f = 0; f < renderHandlers.size(); f++) {
		renderHandlers[f]->onVRRenderScene(*renderState);
	}
}

VRDisplayNode* VROpenHapticsNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	//bool asyncEnabled = int(config->getValue("AsyncEnabled", nameSpace));
	return new VROpenHapticsNode(nameSpace, *vrMain);
}

} /* namespace MinVR */
