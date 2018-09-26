/*
 * VRWhileRenderingNode.cpp
 *
 *  Created on: Sep 26, 2018
 *      Author: dan
 */

#include "VRWhileRenderingNode.h"

namespace MinVR {

VRWhileRenderingNode::VRWhileRenderingNode(const std::string &name) : VRDisplayNode(name), renderHandler(NULL), threadAction(THREADACTION_None), actionCompleted(false) {
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
		// Wait for an action
		VRRenderThreadAction action = waitForAction();

		// If the action is terminate, exit loop
		if (action == THREADACTION_Terminate) {
			return;
		}

		if (action == THREADACTION_WaitForRenderToComplete) {
			renderState->pushState();
			std::cout << "while rendering" << std::endl;
			renderState->addData("WhileRendering", true);
			VRDisplayNode::waitForRenderToComplete(renderState);
			VRDisplayNode::displayFinishedRendering(renderState);
			renderState->popState();
			completeAction();
			startThreadAction(THREADACTION_DisplayFinishedRendering);
		}
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
	VRDisplayNode::render(renderState, renderHandler);
}

void VRWhileRenderingNode::waitForRenderToComplete(VRDataIndex* renderState) {
	//VRDisplayNode::waitForRenderToComplete(renderState);
	startThreadAction(THREADACTION_WaitForRenderToComplete);
}

void VRWhileRenderingNode::displayFinishedRendering(VRDataIndex* renderState) {
	//VRDisplayNode::displayFinishedRendering(renderState);
	completeAction();
	while (waitForAction() != THREADACTION_DisplayFinishedRendering) {
	}
	completeAction();

}

void VRWhileRenderingNode::startThreadAction(VRRenderThreadAction threadAction) {
	// Sets the approriate variables needed to run an action. Then it notifies all
	// the threads to wake up and perform that action.
	actionMutex.lock();
	actionCompleted = false;
	this->threadAction = threadAction;
	actionCond.notify_all();
	actionMutex.unlock();
}

VRRenderThreadAction VRWhileRenderingNode::waitForAction() {
	// Loops until threadAction is not None
	UniqueMutexLock actionLock(actionMutex);
	while (threadAction == THREADACTION_None) {
		actionCond.wait(actionLock);
	}

	// Returns the defined action
	VRRenderThreadAction action = threadAction;

	actionLock.unlock();

	return action;
}

void VRWhileRenderingNode::completeAction() {
	// Loops until threads are completed and sets the action back to None
	// once they are finished
	actionCompletedMutex.lock();
	threadAction = THREADACTION_None;
	actionCompleted = true;
	actionCompletedCond.notify_all();
	actionCompletedMutex.unlock();
}

void VRWhileRenderingNode::waitForComplete() {
	// Waits until all the threads are completed
	UniqueMutexLock completedActionLock(actionCompletedMutex);
	while (!actionCompleted) {
		actionCompletedCond.wait(completedActionLock);
	}
	completedActionLock.unlock();
}

VRDisplayNode* VRWhileRenderingNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VRWhileRenderingNode(nameSpace);
}

}
