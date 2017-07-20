/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <VRWebSocketsNode.h>
#include <sstream>      // std::stringstream
#include <string>

namespace MinVR {


VRWebSocketsNode::VRWebSocketsNode(const std::string &name, VRWebSocketsServer& server) : VRDisplayNode(name), server(server), frame(0) {
}

VRWebSocketsNode::~VRWebSocketsNode() {
}

void VRWebSocketsNode::render(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {
	frame++;
	std::string pixels = renderState->getValue("pixels", "/");
	//std::cout << "render web sockets \n" << pixels << std::endl;
	if (frame % 20 == 0) {
		std::cout << frame << std::endl;
		server.sendData(pixels);
	}
}

} /* namespace MinVR */
