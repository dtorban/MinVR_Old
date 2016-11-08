/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRLEAPMOTIONDEVICE_H_
#define VRLEAPMOTIONDEVICE_H_

#include "input/VRInputDevice.h"
#include <vector>
#include "config/VRDataIndex.h"
#include "Leap.h"
#include "main/VRMainInterface.h"

namespace MinVR {

class VRLeapMotionDevice : public VRInputDevice, public Leap::Listener {
public:
	VRLeapMotionDevice(const std::string& name);
	virtual ~VRLeapMotionDevice();

	void appendNewInputEventsSinceLastCall(VRDataQueue *inputEvents);

	virtual void onInit(const Leap::Controller&);
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onExit(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
	virtual void onFocusGained(const Leap::Controller&);
	virtual void onFocusLost(const Leap::Controller&);
	virtual void onDeviceChange(const Leap::Controller&);
	virtual void onServiceConnect(const Leap::Controller&);
	virtual void onServiceDisconnect(const Leap::Controller&);

	static VRInputDevice* create(VRMainInterface *vrMain, VRDataIndex *config,const std::string &nameSpace);

private:
	std::string name;
	VRDataIndex dataIndex;
	std::vector<std::string> events;
	static Leap::Controller controller;

};

} /* namespace MinVR */

#endif /* VRLEAPMOTIONDEVICE_H_ */
