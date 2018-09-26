/*
 * VRWhileRenderingNode.cpp
 *
 *  Created on: Sep 26, 2018
 *      Author: dan
 */

#include "VRWhileRenderingNode.h"

namespace MinVR {

VRWhileRenderingNode::VRWhileRenderingNode(const std::string &name) : VRDisplayNode(name), renderHandler(NULL), currentAction(THREADACTION_None) {
	thread = new Thread(&VRWhileRenderingNode::renderLoop, this);
}

VRWhileRenderingNode::~VRWhileRenderingNode() {
	// Join the thread if needed
	if (thread) {
		thread->join();
	}

	// Delete thread
	delete thread;
}

void VRWhileRenderingNode::renderLoop() {
	while(true) {

		std::cout << "Inside" << std::endl;
		/*renderState->pushState();
		renderState->addData("WhileRendering", true);
		VRDisplayNode::render(renderState, renderHandler);
		VRDisplayNode::waitForRenderToComplete(renderState);
		VRDisplayNode::displayFinishedRendering(renderState);
		renderState->popState();*/
	}
}

void VRWhileRenderingNode::render(VRDataIndex* renderState, VRRenderHandler* renderHandler) {
	this->renderHandler = renderHandler;
	this->renderState = renderState;
}

void VRWhileRenderingNode::waitForRenderToComplete(VRDataIndex* renderState) {
	//VRDisplayNode::waitForRenderToComplete(renderState);
}

void VRWhileRenderingNode::displayFinishedRendering(VRDataIndex* renderState) {
	//VRDisplayNode::displayFinishedRendering(renderState);
}

VRDisplayNode* VRWhileRenderingNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VRWhileRenderingNode(nameSpace);
}

}
