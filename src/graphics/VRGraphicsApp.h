/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRGRAPHICSAPP_H_
#define VRGRAPHICSAPP_H_

#include <main/VRMain.h>
#include "VRGraphicsHandler.h"

namespace MinVR {

class VRGraphicsApp : public VREventHandler, public VRGraphicsHandler {
public:
	VRGraphicsApp(int argc, char** argv, const std::string& configFile);
	virtual ~VRGraphicsApp();

	virtual void onVREvent(const std::string &eventName, VRDataIndex *eventData) {}
	virtual void onVRRenderScene(VRGraphicsState& renderState) {}
	virtual void onVRRenderContext(VRGraphicsState& renderState) {}

	void run();

protected:
	VRMain *vrMain;
	bool running;
	int frame;
};

} /* namespace MinVR */

#endif /* VRGRAPHICSAPP_H_ */
