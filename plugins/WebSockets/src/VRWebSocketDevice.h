/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */


#ifndef VRWEBSOCKETDEVICE_H
#define VRWEBSOCKETDEVICE_H

#include <vector>
#include <input/VRInputDevice.h>
#include <config/VRDataIndex.h>
#include <main/VRMainInterface.h>
#include "VRWebSocketsServer.h"

#define WS_SERVER_PORT  8081

namespace MinVR {

class VRWebSocketDevice : public VRInputDevice, public WSReceiveCallback, public VREventHandler
{
public:
	VRWebSocketDevice(VRMainInterface* vrMain, int port = WS_SERVER_PORT);
	virtual ~VRWebSocketDevice();
	
	void appendNewInputEventsSinceLastCall(VRDataQueue *inputEvents);
	void receive(const std::string& protocol, int sessionId, const std::string& data);
	void onVREvent(const VRDataIndex &event);

	static VRInputDevice* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);

private:
	VRWebSocketsServer server;
    std::vector<VRDataIndex> events;

};

}                        // end namespace

#endif                  
