/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRWebSocketsInputDevice.h"

namespace MinVR {

VRWebSocketsInputDevice::VRWebSocketsInputDevice(VRWebSocketsServer& server) : server(server) {
}

VRWebSocketsInputDevice::~VRWebSocketsInputDevice() {
}

void VRWebSocketsInputDevice::appendNewInputEventsSinceLastCall(VRDataQueue* queue) {
	server.service();
}

}