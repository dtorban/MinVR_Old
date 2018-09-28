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

	void addChild(VRDisplayNode* child);

	// The three main methods that need to be synchronized at the thread level
	virtual void render(VRDataIndex *renderState, VRRenderHandler *renderHandler);
	virtual void waitForRenderToComplete(VRDataIndex *renderState);
	virtual void displayFinishedRendering(VRDataIndex *renderState);
	void renderLoop();
	void whileLoop();

	static VRDisplayNode* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);

	Thread* renderThread;
	Thread* whileThread;
	VRMainInterface* vrMain;
	VRThreadGroup threadGroup;
	VRDisplayNode* renderNode;
	VRDisplayNode* whileRenderNode;
	VRDataIndex renderState;
	VRRenderHandler* renderHandler;

};

}

#endif /* CMAKE_FRAMEWORK_EXTERNAL_MINVR_SRC_PLUGINS_THREADING_SRC_VRWHILERENDERINGNODE_H_ */
