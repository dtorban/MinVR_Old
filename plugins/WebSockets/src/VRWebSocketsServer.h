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
#include <string>

namespace MinVR {

class WSReceiveCallback {
public:
	virtual void receive(const std::string& protocol, int sessionId, const std::string& data) = 0;
};

class VRWebSocketsServer {
public:
	VRWebSocketsServer(int port);
	virtual ~VRWebSocketsServer();

	void service();
	void sendData(const std::string& data);

	void addReceiveCallback(WSReceiveCallback& callback);

private:
	lws_context *context;
	void* serverContext;

};

} /* namespace MinVR */

#endif /* VRWEBSOCKETSNODE_H_ */
