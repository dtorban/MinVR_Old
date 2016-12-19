/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <api/VRApp.h>
#include <main/VRMain.h>
#include <iostream>
#include "VRGraphicsHandler.h"

namespace MinVR {

class VRGraphicsAppHandler : public VRGraphicsHandler  {
public:
	VRGraphicsAppHandler(VRApp& app) : app(app) {}

	void onVRRenderGraphics(VRGraphicsState& renderState) { app.onVRRenderGraphics(renderState); }
	void onVRRenderGraphicsContext(VRGraphicsState& renderState)  { app.onVRRenderGraphicsContext(renderState); }

private:
	VRApp& app;
};

class VRApp::VRAppInternal {
public:
	VRAppInternal(VRApp& app) : vrMain(NULL), running(true), frame(0), graphicsHandler(app) {}
	VRMain *vrMain;
	bool running;
	int frame;
	VRGraphicsAppHandler graphicsHandler;
};

VRApp::VRApp(int argc, char** argv, const std::string& configFile) : _(*(new VRAppInternal(*this))) {
	_.vrMain = new VRMain();

	_.vrMain->initialize(argc, argv, configFile);

	_.vrMain->addEventHandler(this);
	_.vrMain->addRenderHandler(&_.graphicsHandler);
}

VRApp::~VRApp()  {
	_.vrMain->shutdown();
	delete _.vrMain;
	delete &_;
}

void VRApp::run()  {
	while (_.running) {
		_.vrMain->mainloop();
		_.frame++;
	}
}

void VRApp::shutdown() {
	_.running = false;
}

bool VRApp::isRunning() const {
	return _.running;
}

int VRApp::getFrame() const {
	return _.frame;
}

} /* namespace MinVR */
