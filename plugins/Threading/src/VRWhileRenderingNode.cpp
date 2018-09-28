/*
 * VRWhileRenderingNode.cpp
 *
 *  Created on: Sep 26, 2018
 *      Author: dan
 */

#include "VRWhileRenderingNode.h"

namespace MinVR {

VRWhileRenderingNode::VRWhileRenderingNode(const std::string &name, VRMainInterface *vrMain) : VRDisplayNode(name), vrMain(vrMain), threadGroup(2), renderNode(NULL), whileRenderNode(NULL) {
	renderThread = new Thread(&VRWhileRenderingNode::renderLoop, this);
	whileThread = new Thread(&VRWhileRenderingNode::whileLoop, this);
}

VRWhileRenderingNode::~VRWhileRenderingNode() {
	threadGroup.startThreadAction(THREADACTION_Terminate, NULL, NULL);

	// Join the thread if needed
	if (renderThread) {
		renderThread->join();
	}

	// Delete thread
	delete renderThread;

	// Join the thread if needed
	if (whileThread) {
		whileThread->join();
	}

	// Delete thread
	delete whileThread;
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
			// Various render actions for the display nodes
			if (action == THREADACTION_Render) {

				renderState = *threadGroup.getRenderState();
				renderHandler = threadGroup.getRenderHandler();
				renderNode->render(&renderState, threadGroup.getRenderHandler());
			}
			else if (action == THREADACTION_WaitForRenderToComplete) {
				renderNode->waitForRenderToComplete(threadGroup.getRenderState());
				renderNode->displayFinishedRendering(threadGroup.getRenderState());
			}
		}

		// Notify the thread group that this thread is complete
		threadGroup.completeAction();

		// Synchronize all threads here
		threadGroup.synchronize();
	}
}

void VRWhileRenderingNode::whileLoop() {
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


			// Various render actions for the display nodes
			if (action == THREADACTION_Render) {
			}
			else if (action == THREADACTION_WaitForRenderToComplete) {
				/*const std::vector<VRRenderHandler*>& handlers = vrMain->getRenderHandlers();
				for (int f = 0; f < handlers.size(); f++) {
					whileRenderNode->render(&renderState, handlers[f]);
				}*/
				whileRenderNode->render(&renderState, renderHandler);
				whileRenderNode->waitForRenderToComplete(threadGroup.getRenderState());
				whileRenderNode->displayFinishedRendering(threadGroup.getRenderState());
			}
		}

		// Notify the thread group that this thread is complete
		threadGroup.completeAction();

		// Synchronize all threads here
		threadGroup.synchronize();
	}
}

void VRWhileRenderingNode::addChild(VRDisplayNode* child) {
	VRDisplayNode::addChild(child);
	if (child->getName().substr(child->getName().length()-6,6) == "Render") {
		std::cout << "RenderNode" << std::endl;
		renderNode = child;
	}

	if (child->getName().substr(child->getName().length()-5,5) == "While") {
		std::cout << "WhileRender" << std::endl;
		whileRenderNode = child;
	}
}

void VRWhileRenderingNode::render(VRDataIndex* renderState, VRRenderHandler* renderHandler) {
	threadGroup.startThreadAction(THREADACTION_Render, renderState, renderHandler);
	threadGroup.waitForComplete();
	//renderNode->waitForRenderToComplete(renderState);
	/*state = *renderState;
	if (getChildren().size() == 0) {
		VRDisplayNode::render(renderState, renderHandler);
	}
	else {
		renderNode->render(renderState, renderHandler);
	}*/
}

void VRWhileRenderingNode::waitForRenderToComplete(VRDataIndex* renderState) {
	threadGroup.startThreadAction(THREADACTION_WaitForRenderToComplete, renderState, NULL);
}

void VRWhileRenderingNode::displayFinishedRendering(VRDataIndex* renderState) {
	threadGroup.waitForComplete();
}

VRDisplayNode* VRWhileRenderingNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VRWhileRenderingNode(nameSpace, vrMain);
}

}
