/*
 * VRWhileRenderingNode.h
 *
 *  Created on: Sep 26, 2018
 *      Author: dan
 */

#ifndef VRWHILERENDERINGNODE_H_
#define VRWHILERENDERINGNODE_H_

#include "VRThread.h"
#include "display/VRDisplayNode.h"
#include "VRRenderThreadAction.h"

namespace MinVR {

class VRWhileRenderingNode : public VRDisplayNode {
public:
	VRWhileRenderingNode(const std::string &name);
	virtual ~VRWhileRenderingNode();

	virtual std::string getType() const { return "VRWhileRenderingNode"; }

	// The three main methods that need to be synchronized at the thread level
	virtual void render(VRDataIndex *renderState, VRRenderHandler *renderHandler);
	virtual void waitForRenderToComplete(VRDataIndex *renderState);
	virtual void displayFinishedRendering(VRDataIndex *renderState);
	void renderLoop();

	/// Starts an action on all the threads
	void startThreadAction(VRRenderThreadAction threadAction);

	/// Waits for an action to be started
	VRRenderThreadAction waitForAction();

	/// Notifies the thread group that an action is completed
	void completeAction();

	/// Waits for all threads to be completed
	void waitForComplete();

	static VRDisplayNode* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);

	Thread* thread;
	VRRenderHandler* renderHandler;
	VRDataIndex* renderState;
	VRRenderThreadAction threadAction;
	bool actionCompleted;
	Mutex actionMutex;
	ConditionVariable actionCond;
	Mutex actionCompletedMutex;
	ConditionVariable actionCompletedCond;
};

}

#endif /* CMAKE_FRAMEWORK_EXTERNAL_MINVR_SRC_PLUGINS_THREADING_SRC_VRWHILERENDERINGNODE_H_ */
