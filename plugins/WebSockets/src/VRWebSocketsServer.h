/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRWEBSOCKETSSERVER_H_
#define VRWEBSOCKETSSERVER_H_

#include <libwebsockets.h>

namespace MinVR {

class VRWebSocketsServer {
public:
	VRWebSocketsServer(int port);
	virtual ~VRWebSocketsServer();

	void service();

private:
	lws_context *context;

};

} /* namespace MinVR */

#endif /* VRWEBSOCKETSNODE_H_ */
