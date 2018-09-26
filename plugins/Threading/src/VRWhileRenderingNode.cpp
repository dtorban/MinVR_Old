/*
 * VRWhileRenderingNode.cpp
 *
 *  Created on: Sep 26, 2018
 *      Author: dan
 */

#include "VRWhileRenderingNode.h"

namespace MinVR {

VRWhileRenderingNode::VRWhileRenderingNode(const std::string &name, VRMainInterface *vrMain) : VRDisplayNode(name), vrMain(vrMain), threadGroup(1) {
	thread = new Thread(&VRWhileRenderingNode::renderLoop, this);
}

VRWhileRenderingNode::~VRWhileRenderingNode() {
	threadGroup.startThreadAction(THREADACTION_Terminate, NULL, NULL);

	// Join the thread if needed
	if (thread) {
		thread->join();
	}

	// Delete thread
	delete thread;
}

void VRWhileRenderingNode::renderLoop() {
	// Ensure all threads are synchronized before starting to render
	threadGroup.synchronize();

	while (true) {
		// Wait for an action
		VRRenderThreadAction action = threadGroup.waitForAction();

		// If the action is terminate, exit loop
		if (action == THREADACTION_Terminate) {
			return;
		}
		else
		{
			VRDataIndex renderState;

			// Various render actions for the display nodes
			if (action == THREADACTION_WaitForRenderToComplete) {
				//renderState->pushState();
				renderState.addData("WhileRendering", true);
				//renderHandler->onVRRenderScene(renderState);
				//VRDisplayNode::render(&renderState, renderHandler);
				const std::vector<VRRenderHandler*>& handlers = vrMain->getRenderHandlers();
				for (int f = 0; f < handlers.size(); f++) {
					handlers[f]->onVRRenderScene(renderState);
				}

				VRDisplayNode::waitForRenderToComplete(&renderState);
				VRDisplayNode::displayFinishedRendering(&renderState);
				//renderState->popState();
			}
		}

		// Notify the thread group that this thread is complete
		threadGroup.completeAction();

		// Synchronize all threads here
		threadGroup.synchronize();
	}
}

void VRWhileRenderingNode::render(VRDataIndex* renderState, VRRenderHandler* renderHandler) {
	//threadGroup.startThreadAction(THREADACTION_Render, NULL, NULL);
	VRDisplayNode::render(renderState, renderHandler);
	//threadGroup.waitForComplete();
}

void VRWhileRenderingNode::waitForRenderToComplete(VRDataIndex* renderState) {
	threadGroup.startThreadAction(THREADACTION_WaitForRenderToComplete, NULL, NULL);
}

void VRWhileRenderingNode::displayFinishedRendering(VRDataIndex* renderState) {
	threadGroup.waitForComplete();
}

VRDisplayNode* VRWhileRenderingNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VRWhileRenderingNode(nameSpace, vrMain);
}

}
