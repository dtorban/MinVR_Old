/*
 * VRCallbackNode.cpp
 *
 *  Created on: Sep 27, 2018
 *      Author: dan
 */

#include <display/VRCallbackNode.h>

namespace MinVR {

VRCallbackNode::VRCallbackNode(const std::string &name, const std::string &callbackName, VRMainInterface *vrMain) : VRDisplayNode(name), callbackName(callbackName), vrMain(vrMain) {
	// TODO Auto-generated constructor stub

}

VRCallbackNode::~VRCallbackNode() {
	// TODO Auto-generated destructor stub
}

void VRCallbackNode::render(VRDataIndex *renderState, VRRenderHandler* renderHandler) {
	const std::vector<VRCallbackHandler*>& handlers = vrMain->getCallbackHandlers();

	for (int f = 0; f < handlers.size(); f++) {
		handlers[f]->onRun(callbackName, *renderState);
	}
}

VRDisplayNode*
VRCallbackNode::create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace) {
	std::string nodeNameSpace = nameSpace;

	std::string callbackName = config->getValueWithDefault("CallbackName", std::string(""), nameSpace);
	VRDisplayNode *node = new VRCallbackNode(nameSpace, callbackName, vrMain);

	return node;
}

} /* namespace MinVR */
