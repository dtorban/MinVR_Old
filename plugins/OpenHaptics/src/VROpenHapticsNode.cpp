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
	hapticsState.addData("IsHaptics", true);
}

VROpenHapticsNode::~VROpenHapticsNode() {
}

struct HapticsData {
	VROpenHapticsNode* node;
	VRDataIndex* renderState;
	VRRenderHandler* renderHandler;
};

void VROpenHapticsNode::render(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {

	renderState->pushState();
	renderState->addData("IsHaptics", true);
    HapticsData hapticsData;
    hapticsData.node = this;
    hapticsData.renderState = renderState;
    hapticsData.renderHandler = renderHandler;
    // schedule haptics synchronous
    setHapticsState(&hapticsData);
	renderState->popState();

    bool initRender = (int)renderState->getValue("InitRender","/") == 1;
    if (initRender) {
    	// schedule haptics asynchronous
		renderHaptics(this);	
    }

}

void VROpenHapticsNode::setHapticsState(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {
	renderHandlers = vrMain.getRenderHandlers();
	renderHandler->onVRRenderContext(*renderState);
}

void VROpenHapticsNode::renderHaptics() {
	for (int f = 0; f < renderHandlers.size(); f++) {
		renderHandlers[f]->onVRRenderScene(hapticsState);
	}
}

HDCallbackCode HDCALLBACK VROpenHapticsNode::setHapticsState(void *data) {
	HapticsData* hapticsData = static_cast<HapticsData*>(data);
	hapticsData->node->setHapticsState(hapticsData->renderState, hapticsData->renderHandler);
}

HDCallbackCode HDCALLBACK VROpenHapticsNode::renderHaptics(void *data) {
	VROpenHapticsNode* node = static_cast<VROpenHapticsNode*>(data);
	node->renderHaptics();
}

VRDisplayNode* VROpenHapticsNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VROpenHapticsNode(nameSpace, *vrMain);
}

} /* namespace MinVR */
