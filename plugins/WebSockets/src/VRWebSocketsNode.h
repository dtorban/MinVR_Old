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
#include "VRWebSocketsServer.h"
#include <main/VRItemFactory.h>

namespace MinVR {

class VRWebSocketsNode : public VRDisplayNode {
public:
	VRWebSocketsNode(const std::string &name, VRWebSocketsServer& server);
	virtual ~VRWebSocketsNode();

	virtual std::string getType() { return "VRWebSocketsNode"; }

	virtual void render(VRDataIndex *renderState, VRRenderHandler *renderHandler);

	//static VRDisplayNode* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);

private:
	VRWebSocketsServer &server;
	int frame;

};

class VRWebSocketsNodeFactory : public VRSpecificItemFactory<VRDisplayNode> {
public:
	VRWebSocketsNodeFactory(VRWebSocketsServer& server) : VRSpecificItemFactory<VRDisplayNode>("VRWebSocketsNode"), server(server) {}
	virtual ~VRWebSocketsNodeFactory() {}
	VRDisplayNode* createConcrete(VRMainInterface *vrMain, VRDataIndex *config, const std::string &itemNameSpace) {
		return new VRWebSocketsNode(itemNameSpace, server);
	}
private:
	VRWebSocketsServer& server;
};

} /* namespace MinVR */

#endif /* VRWEBSOCKETSNODE_H_ */
