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

VROpenHapticsNode::VROpenHapticsNode(const std::string& name) : VRDisplayNode(name) {
}

VROpenHapticsNode::~VROpenHapticsNode() {
}

void VROpenHapticsNode::render(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {
	std::cout << "Render haptics." << std::endl;

}

VRDisplayNode* VROpenHapticsNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	//bool asyncEnabled = int(config->getValue("AsyncEnabled", nameSpace));
	return new VROpenHapticsNode(nameSpace);
}

} /* namespace MinVR */
