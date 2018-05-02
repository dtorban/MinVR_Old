/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VROpenHapticsNodeHL.h"
#include <iostream>

namespace MinVR {

VROpenHapticsNodeHL::VROpenHapticsNodeHL(const std::string& name, VRMainInterface &vrMain) : VRDisplayNode(name), vrMain(vrMain) {
	hapticsState.addData("IsHaptics", true);

	device = new VROpenHapticsDeviceHL("Omni");
	vrMain.addInputDevice(device);
}

VROpenHapticsNodeHL::~VROpenHapticsNodeHL() {
#ifdef WITH_HAPTICS
    hdUnschedule(hRenderHaptics);
#endif
	delete device;
}

struct HapticsData {
	VROpenHapticsNodeHL* node;
	VRDataIndex* renderState;
	VRRenderHandler* renderHandler;
};

void VROpenHapticsNodeHL::render(VRDataIndex* renderState, VRRenderHandler* renderHandler) {

    bool initRender = (int)renderState->getValue("InitRender","/") == 1;

#ifdef WITH_HAPTICS
    hdMakeCurrentDevice(device->getDevice());
#endif

	renderState->pushState();
	renderState->addData("IsHaptics", true);
    HapticsData hapticsData;
    hapticsData.node = this;
    hapticsData.renderState = renderState;
    hapticsData.renderHandler = renderHandler;
#ifdef WITH_HAPTICS
    // schedule haptics synchronous
    hdScheduleSynchronous(setHapticsState, &hapticsData,
                          HD_MIN_SCHEDULER_PRIORITY);
#endif
	renderState->popState();

    if (initRender) {
#ifdef WITH_HAPTICS
    	// schedule haptics asynchronous
	    hRenderHaptics = hdScheduleAsynchronous(
	        renderHaptics, this, 
	        HD_MAX_SCHEDULER_PRIORITY);
#endif
    }

}

void VROpenHapticsNodeHL::setHapticsState(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {
	renderHandlers = vrMain.getRenderHandlers();
	renderHandler->onVRRenderContext(*renderState);
}

void VROpenHapticsNodeHL::renderHaptics() {
	device->beginUpdate();

	for (int f = 0; f < renderHandlers.size(); f++) {
		renderHandlers[f]->onVRRenderScene(hapticsState);
	}

	device->endUpdate();
}

HDCallbackCode HDCALLBACK VROpenHapticsNodeHL::setHapticsState(void *data) {
	HapticsData* hapticsData = static_cast<HapticsData*>(data);
	hapticsData->node->setHapticsState(hapticsData->renderState, hapticsData->renderHandler);

    // execute this only once.
    return HD_CALLBACK_DONE;
}

HDCallbackCode HDCALLBACK VROpenHapticsNodeHL::renderHaptics(void *data) {
	VROpenHapticsNodeHL* node = static_cast<VROpenHapticsNodeHL*>(data);
    node->renderHaptics();

#ifdef WITH_HAPTICS
    /* Check for errors and abort the callback if a scheduler error
       is detected. */
    /*if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, 
                      "Error detected while rendering\n");
        
        if (hduIsSchedulerError(&error))
        {
            return HD_CALLBACK_DONE;
        }
    }*/
#endif

    /* Signify that the callback should continue running, i.e. that
       it will be called again the next scheduler tick. */
    return HD_CALLBACK_CONTINUE;
}

VRDisplayNode* VROpenHapticsNodeHL::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {

	return new VROpenHapticsNodeHL(nameSpace, *vrMain);
}

} /* namespace MinVR */
