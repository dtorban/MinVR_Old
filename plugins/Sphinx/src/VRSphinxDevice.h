/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRSPHINXDEVICE_H_
#define VRSPHINXDEVICE_H_

#include "input/VRInputDevice.h"
#include <thread>
#include <mutex>
#include "main/VRMainInterface.h"
#include <vector>

namespace MinVR {

class VRSphinxDevice : public VRInputDevice {
public:
	VRSphinxDevice();
	virtual ~VRSphinxDevice();

	void appendNewInputEventsSinceLastCall(VRDataQueue *inputEvents);
	void run();
	void recognize_from_microphone();

	static VRInputDevice* create(VRMainInterface *vrMain, MinVR::VRDataIndex *config, const std::string &nameSpace);

private:
	std::thread* deviceThread;
	std::mutex deviceMutex;
	VRDataIndex dataIndex;
	std::vector<std::string> events;

};

} /* namespace MinVR */

#endif /* VRSPHINXDEVICE_H_ */
