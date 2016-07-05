/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "main/VRMain.h"

using namespace MinVR;

extern "C" {
	typedef int (*eventcallback_type)(const char* eventName);
}

class VRPythonEventCallbackHandler : public VREventHandler {
public:
	VRPythonEventCallbackHandler(eventcallback_type eventCallback) : eventCallback(eventCallback) {}
	virtual ~VRPythonEventCallbackHandler() {}

	void onVREvent(const std::string &eventName, VRDataIndex *eventData) {
		eventCallback(eventName.c_str());
	}

private:
	eventcallback_type eventCallback;
};

extern "C" {
	VRMain* VRMain_init(char* config) {
		VRMain* vrmain = new VRMain();
		char** input = new char*[2];
		input[1] = config;
		vrmain->initialize(2, input);
		delete[] input;
		return vrmain;
	}

	VREventHandler* VRMain_registerEventCallback(VRMain* vrmain, eventcallback_type eventCallback) {
		VREventHandler* handler = new VRPythonEventCallbackHandler(eventCallback);
		std::cout << "callback registered" << std::endl;
		vrmain->addEventHandler(handler);
//		handler->onVREvent("hit", NULL);
		return handler;
	}
}
