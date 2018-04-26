/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VROpenHapticsDevice.h"
#include <api/VRButtonEvent.h>
#include <api/VRCursorEvent.h>

namespace MinVR {

VROpenHapticsDevice::VROpenHapticsDevice(const std::string& name) : name(name) {
    #ifdef WITH_HAPTICS
     /* Initialize the device, must be done before attempting to call any hd 
       functions. Passing in HD_DEFAULT_DEVICE causes the default device to be 
       initialized. */
        hHD = hdInitDevice(name);
        if (HD_DEVICE_ERROR(error = hdGetError())) 
        {
            hduPrintError(stderr, &error, "Failed to initialize haptic device");
            fprintf(stderr, "\nPress any key to quit.\n");
            //getch();
        }

        printf("Hello Haptic Device!\n");
        printf("Found device model: %s.\n\n", hdGetString(HD_DEVICE_MODEL_TYPE));
        
        hdStartScheduler();

        /* Check for errors and abort if so. */
        if (HD_DEVICE_ERROR(error = hdGetError()))
        {
            hduPrintError(stderr, &error, "Failed to start scheduler");
            fprintf(stderr, "\nPress any key to quit.\n");
        }
#endif
}

VROpenHapticsDevice::~VROpenHapticsDevice() {
#ifdef WITH_HAPTICS
    hdStopScheduler();

    /* Disable the device. */
    hdDisableDevice(hHD);
#endif
}

void VROpenHapticsDevice::appendNewInputEventsSinceLastCall(VRDataQueue* queue) {
#ifdef WITH_HAPTICS
    hdScheduleSynchronous(getDeviceState, this,
                          HD_MIN_SCHEDULER_PRIORITY);

    for (int f = 0; f < hapticsEvents.size(); f++) {
        if (hapticsEvents[f] == BUTTON_1_DOWN || hapticsEvents[f] == BUTTON_1_UP) {
            std::string name = "BUTTON_1_";
            if (hapticsEvents[f] == BUTTON_1_DOWN) {
                name = name + "_Down";
            }
            else {
                name = name + "_Up";
            }

            VRDataIndex di = VRButtonEvent::createValidDataIndex(name, down);
            _events.push_back(di);
        }

        delete hapticsEvents[f];
    }

    hapticsEvents.clear();
#endif

    std::cout << "Called haptics event " << name << "." << std::endl;


    for (size_t f = 0; f < _events.size(); f++)
    {
    	queue->push(_events[f].serialize());
    }

    _events.clear();
}

void VROpenHapticsDevice::beginUpdate() {
#ifdef WITH_HAPTICS
    hdMakeCurrentDevice(hHD);
    /* Begin haptics frame.  ( In general, all state-related haptics calls
       should be made within a frame. ) */
    hdBeginFrame(hHD);

    lastState = currentState;

    hdGetDoublev(HD_CURRENT_POSITION, currentState.position);
    hdGetDoublev(HD_CURRENT_TRANSFORM, currentState.transform);
    hdGetDoublev(HD_CURRENT_FORCE, currentState.force);

    /* Check for a stylus switch state change. */
    HDint nCurrentButtonState, nLastButtonState;
    hdGetIntegerv(HD_CURRENT_BUTTONS, &nCurrentButtonState);
    hdGetIntegerv(HD_LAST_BUTTONS, &nLastButtonState);

    if ((nCurrentButtonState & HD_DEVICE_BUTTON_1) != 0 &&
        (nLastButtonState & HD_DEVICE_BUTTON_1) == 0)
    {
        handleEvent(BUTTON_1_DOWN);
    }
    else if ((nCurrentButtonState & HD_DEVICE_BUTTON_1) == 0 &&
             (nLastButtonState & HD_DEVICE_BUTTON_1) != 0)
    {
        //handleEvent(BUTTON_1_UP);
    }
    
    if ((nCurrentButtonState & HD_DEVICE_BUTTON_2) != 0 &&
        (nLastButtonState & HD_DEVICE_BUTTON_2) == 0)
    {
        //handleEvent(BUTTON_2_DOWN);
    }
    else if ((nCurrentButtonState & HD_DEVICE_BUTTON_2) == 0 &&
             (nLastButtonState & HD_DEVICE_BUTTON_2) != 0)
    {
        //handleEvent(BUTTON_2_UP);
    }
    if ((nCurrentButtonState & HD_DEVICE_BUTTON_3) != 0 &&
        (nLastButtonState & HD_DEVICE_BUTTON_3) == 0)
    {
        //handleEvent(BUTTON_3_DOWN);
    }
    else if ((nCurrentButtonState & HD_DEVICE_BUTTON_3) == 0 &&
             (nLastButtonState & HD_DEVICE_BUTTON_3) != 0)
    {
        //handleEvent(BUTTON_3_UP);
    }
#endif
}

void VROpenHapticsDevice::endUpdate() {
#ifdef WITH_HAPTICS

    /* End haptics frame. */
    hdEndFrame(hHD);
#endif
}

void VROpenHapticsDevice::handleEvent(EventType type) {
#ifdef WITH_HAPTICS
    HapticsEvent* event = new HapticsEvent();
    event->type = type;
    event->state = currentState;
    currentEvents.push_back(event);
#endif
}

void VROpenHapticsDevice::getDeviceState() {
#ifdef WITH_HAPTICS
    hdMakeCurrentDevice(hHD);

    deviceState = currentState;
    for (int f = 0; f < currentEvents.size(); f++) {
        hapticsEvents->push_back(currentEvents[f]);
    }

    currentEvents.clear();
#endif
}

HDCallbackCode HDCALLBACK VROpenHapticsDevice::getDeviceState(void *data) {
    VROpenHapticsDevice* device = static_cast<VROpenHapticsDevice*>(data);
    device->getDeviceState();

    return HD_CALLBACK_DONE;
}

} /* namespace MinVR */

