/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRWEBSOCKETSNODE_H_
#define VRWEBSOCKETSNODE_H_

#include "display/VRDisplayNode.h"
#include <libwebsockets.h>

namespace MinVR {

class VRWebSocketsNode : public VRDisplayNode {
public:
	VRWebSocketsNode(const std::string &name, int port);
	virtual ~VRWebSocketsNode();

	virtual std::string getType() { return "VRWebSocketsNode"; }

	virtual void render(VRDataIndex *renderState, VRRenderHandler *renderHandler);
	virtual void waitForRenderToComplete(VRDataIndex *renderState);
	virtual void displayFinishedRendering(VRDataIndex *renderState);

	static VRDisplayNode* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);

	int getFrame() { return frame; }

	VRRenderHandler* getCurrentRenderHanlder() {
		return currentRenderHanlder;
	}

	VRDataIndex* getCurrentRenderState() {
		return currentRenderState;
	}

private:
	lws_context *context;
	int frame;
	VRDataIndex* currentRenderState;
	VRRenderHandler* currentRenderHanlder;
};

} /* namespace MinVR */

#endif /* VRWEBSOCKETSNODE_H_ */
