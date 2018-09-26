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
#include "VRThreadGroup.h"

namespace MinVR {

class VRWhileRenderingNode : public VRDisplayNode {
public:
	VRWhileRenderingNode(const std::string &name, VRMainInterface *vrMain);
	virtual ~VRWhileRenderingNode();

	virtual std::string getType() const { return "VRWhileRenderingNode"; }

	// The three main methods that need to be synchronized at the thread level
	virtual void render(VRDataIndex *renderState, VRRenderHandler *renderHandler);
	virtual void waitForRenderToComplete(VRDataIndex *renderState);
	virtual void displayFinishedRendering(VRDataIndex *renderState);
	void renderLoop();

	static VRDisplayNode* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);

	Thread* thread;
	VRMainInterface* vrMain;
	VRThreadGroup threadGroup;
};

}

#endif /* CMAKE_FRAMEWORK_EXTERNAL_MINVR_SRC_PLUGINS_THREADING_SRC_VRWHILERENDERINGNODE_H_ */
