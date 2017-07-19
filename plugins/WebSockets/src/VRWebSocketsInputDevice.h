/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef WEBSOCKETSINPUTDEVICE_H_
#define WEBSOCKETSINPUTDEVICE_H_


#include <map>
#include <vector>
#include <string>

#include <config/VRDataIndex.h>
#include <config/VRDataQueue.h>
#include <input/VRInputDevice.h>
#include "VRWebSocketsServer.h"
#include <main/VRMainInterface.h>


namespace MinVR {

/** A VRInputDevice that polls input events (mouse, keyboard, window resize, etc.) 
    for all of the active GLFWWindows and returns input in MinVR event format.
 */
class VRWebSocketsInputDevice : public VRInputDevice {
public:
	VRWebSocketsInputDevice(VRWebSocketsServer& server);
	virtual ~VRWebSocketsInputDevice();

	void appendNewInputEventsSinceLastCall(VRDataQueue* queue);
  
private:
	std::vector<std::string> _events;
	//VRDataIndex _dataIndex;
	VRWebSocketsServer& server;
};


} /* namespace MinVR */

#endif /* WEBSOCKETSINPUTDEVICE_H_ */
