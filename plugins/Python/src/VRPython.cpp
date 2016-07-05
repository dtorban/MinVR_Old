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
	typedef int (*rendercallback_type)();
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

class VRPythonRenderCallbackHandler : public VRRenderHandler {
public:
	VRPythonRenderCallbackHandler(rendercallback_type renderCallback) : renderCallback(renderCallback) {}
	virtual ~VRPythonRenderCallbackHandler() {}
	virtual void onVRRenderScene(VRDataIndex *renderState, VRDisplayNode *callingNode) { renderCallback(); }
	virtual void onVRRenderContext(VRDataIndex *renderState, VRDisplayNode *callingNode) {}

private:
	rendercallback_type renderCallback;
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

	void VRMain_shutdown(VRMain* vrmain, VREventHandler* eventHandler, VRRenderHandler* renderHandler) {
		vrmain->shutdown();
		delete eventHandler;
		delete renderHandler;
		delete vrmain;
	}

	VREventHandler* VRMain_registerEventCallback(VRMain* vrmain, eventcallback_type eventCallback) {
		VREventHandler* handler = new VRPythonEventCallbackHandler(eventCallback);
		vrmain->addEventHandler(handler);
		return handler;
	}

	VRRenderHandler* VRMain_registerRenderCallback(VRMain* vrmain, rendercallback_type renderCallback) {
		VRRenderHandler* handler = new VRPythonRenderCallbackHandler(renderCallback);
		vrmain->addRenderHandler(handler);
		return handler;
	}

	void VRMain_mainloop(VRMain* vrmain) {
		vrmain->mainloop();
	}
}
