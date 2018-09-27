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
			VRDataIndex renderState = state;

			// Various render actions for the display nodes
			if (action == THREADACTION_WaitForRenderToComplete) {

				//renderState.addData("WhileRendering", true);

				//std::cout << getChildren().size() << std::endl;
				if (getChildren().size() >= 2) {
					const std::vector<VRRenderHandler*>& handlers = vrMain->getRenderHandlers();

					for (int f = 0; f < handlers.size(); f++) {
						getChildren()[1]->render(&renderState, handlers[f]);
					}

					getChildren()[1]->waitForRenderToComplete(&renderState);
					getChildren()[1]->displayFinishedRendering(&renderState);
				}

				/*const std::vector<VRCallbackHandler*>& handlers = vrMain->getCallbackHandlers();
				for (int f = 0; f < handlers.size(); f++) {
					if (getChildren().size() < 2) {
						handlers[f]->onVRRenderScene(renderState);
					}
					else {
						getChildren()[1]->render(&renderState, handlers[f]);
					}
				}

				if (getChildren().size() >= 2) {
					getChildren()[1]->waitForRenderToComplete(&renderState);
					getChildren()[1]->displayFinishedRendering(&renderState);
				}*/
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
	state = *renderState;
	if (getChildren().size() == 0) {
		VRDisplayNode::render(renderState, renderHandler);
	}
	else {
		getChildren()[0]->render(renderState, renderHandler);
	}
}

void VRWhileRenderingNode::waitForRenderToComplete(VRDataIndex* renderState) {
	threadGroup.startThreadAction(THREADACTION_WaitForRenderToComplete, NULL, NULL);
	if (getChildren().size() == 0) {
		VRDisplayNode::waitForRenderToComplete(renderState);
	}
	else {
		getChildren()[0]->waitForRenderToComplete(renderState);
	}
}

void VRWhileRenderingNode::displayFinishedRendering(VRDataIndex* renderState) {
	if (getChildren().size() == 0) {
		VRDisplayNode::displayFinishedRendering(renderState);
	}
	else {
		getChildren()[0]->displayFinishedRendering(renderState);
	}
	threadGroup.waitForComplete();
}

VRDisplayNode* VRWhileRenderingNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VRWhileRenderingNode(nameSpace, vrMain);
}

}
