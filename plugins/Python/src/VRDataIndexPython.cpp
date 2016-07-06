/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <plugin/VRPlugin.h>
#include "config/VRDataIndex.h"


using namespace MinVR;

extern "C" {
	//PLUGIN_API typedef int (*eventcallback_type)(const char* eventName);
	//PLUGIN_API typedef int (*rendercallback_type)();
}

/*class VRPythonEventCallbackHandler : public VREventHandler {
public:
	PLUGIN_API VRPythonEventCallbackHandler(eventcallback_type eventCallback) : eventCallback(eventCallback) {}
	PLUGIN_API virtual ~VRPythonEventCallbackHandler() {}

	PLUGIN_API void onVREvent(const std::string &eventName, VRDataIndex *eventData) {
		eventCallback(eventName.c_str());
	}

private:
	eventcallback_type eventCallback;
};*/


extern "C" {
	PLUGIN_API int VRDataIndex_getType(void* renderState, const char* valName, const char* nameSpace) {
		return ((VRDataIndex*)renderState)->getType(valName, nameSpace);
	}

	PLUGIN_API int VRDataIndex_getIntValue(void* renderState, const char* valName, const char* nameSpace) {
			return  ((VRDataIndex*)renderState)->getValue(valName, nameSpace);
	}
	/*PLUGIN_API int VRDataIndex_getType(VRMain* vrmain, eventcallback_type eventCallback) {
		VREventHandler* handler = new VRPythonEventCallbackHandler(eventCallback);
		vrmain->addEventHandler(handler);
		return handler;
	}*/
}
