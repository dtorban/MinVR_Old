/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VROPENHAPTICSDEVICE_H_
#define VROPENHAPTICSDEVICE_H_

#include <vector>

#include <input/VRInputDevice.h>

//#define WITH_HAPTICS
#ifdef WITH_HAPTICS
#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>
#else
#define HDCallbackCode 
#define HDCALLBACK void
#define HD_CALLBACK_DONE 
#define HD_CALLBACK_CONTINUE 
#endif

namespace MinVR {


/** A VRInputDevice that polls input events (mouse, keyboard, window resize, etc.)
    for all of the active GLFWWindows and returns input in MinVR event format.
 */
class VROpenHapticsDeviceHD : public VRInputDevice {
public:
	VROpenHapticsDeviceHD(const std::string& name);
	virtual ~VROpenHapticsDeviceHD();

    void appendNewInputEventsSinceLastCall(VRDataQueue* queue);

#ifdef WITH_HAPTICS
    HHD getDevice() { return hHD; }
#endif

    void beginUpdate();
    void endUpdate();

private:
	void getDeviceState();
	static HDCallbackCode HDCALLBACK getDeviceState(void *data);

	std::string name;
	std::vector<VRDataIndex> _events;

	enum EventType
    {
        BUTTON_1_DOWN = 0,
        BUTTON_1_UP,
        BUTTON_2_DOWN,
        BUTTON_2_UP,
        BUTTON_3_DOWN,
        BUTTON_3_UP,
        DEVICE_ERROR,
        NUM_EVENT_TYPES,
    };

    void handleEvent(EventType type);

#ifdef WITH_HAPTICS
	HHD hHD;
	HDErrorInfo error;

	struct DeviceState {
	    DeviceState() {
	        memset(transform, 0, 16 * sizeof(HDdouble));
	    }

	    DeviceState(const DeviceState &state) {
	        position = state.position;
	        force = state.force;
	        memcpy(transform, state.transform, 16 * sizeof(HDdouble));
	    }

	    hduVector3Dd position;
	    hduVector3Dd force;
	    HDdouble transform[16];
	};

	struct HapticsEvent {
		EventType type;
		DeviceState state;
	};
	
	DeviceState deviceState;
	DeviceState lastState;
	DeviceState currentState;

	std::vector<HapticsEvent*> currentEvents;
	std::vector<HapticsEvent*> hapticsEvents;

#endif
};


} /* namespace MinVR */

#endif /* GLFWINPUTDEVICE_H_ */
