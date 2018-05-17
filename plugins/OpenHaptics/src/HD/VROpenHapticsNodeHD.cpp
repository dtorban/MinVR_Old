/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VROpenHapticsNodeHD.h"
#include <iostream>

namespace MinVR {

VROpenHapticsNodeHD::VROpenHapticsNodeHD(const std::string& name, VRMainInterface &vrMain) : VRDisplayNode(name), vrMain(vrMain) {
	hapticsState.addData("IsHaptics", true);
    hapticsState.addData("API", "HD");

	device = new VROpenHapticsDeviceHD("Omni");
	vrMain.addInputDevice(device);
}

VROpenHapticsNodeHD::~VROpenHapticsNodeHD() {
#ifdef WITH_HAPTICS
    hdUnschedule(hRenderHaptics);
#endif
	delete device;
}

struct HapticsDataHD {
	VROpenHapticsNodeHD* node;
	VRDataIndex* renderState;
	VRRenderHandler* renderHandler;
};

void VROpenHapticsNodeHD::render(VRDataIndex* renderState, VRRenderHandler* renderHandler) {

    bool initRender = (int)renderState->getValue("InitRender","/") == 1;

#ifdef WITH_HAPTICS
    hdMakeCurrentDevice(device->getDevice());
#endif

	renderState->pushState();
	renderState->addData("IsHaptics", true);
    renderState->addData("API", "HD");
    HapticsDataHD hapticsData;
    hapticsData.node = this;
    hapticsData.renderState = renderState;
    hapticsData.renderHandler = renderHandler;
#ifdef WITH_HAPTICS
    // schedule haptics synchronous
    hdScheduleSynchronous(setHapticsState, &hapticsData,
                          HD_MIN_SCHEDULER_PRIORITY);
#else
    setHapticsState(renderState, renderHandler);
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

#ifndef WITH_HAPTICS
    renderHaptics();
#endif
}

void VROpenHapticsNodeHD::setHapticsState(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {
	renderHandlers = vrMain.getRenderHandlers();
	renderHandler->onVRRenderContext(*renderState);
}

void VROpenHapticsNodeHD::renderHaptics() {
	device->beginUpdate();

	for (int f = 0; f < renderHandlers.size(); f++) {
		renderHandlers[f]->onVRRenderScene(hapticsState);
	}

	device->endUpdate();
}

HDCallbackCode HDCALLBACK VROpenHapticsNodeHD::setHapticsState(void *data) {
	HapticsDataHD* hapticsData = static_cast<HapticsDataHD*>(data);
	hapticsData->node->setHapticsState(hapticsData->renderState, hapticsData->renderHandler);

    // execute this only once.
    return HD_CALLBACK_DONE;
}

HDCallbackCode HDCALLBACK VROpenHapticsNodeHD::renderHaptics(void *data) {
	VROpenHapticsNodeHD* node = static_cast<VROpenHapticsNodeHD*>(data);
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

VRDisplayNode* VROpenHapticsNodeHD::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {

	return new VROpenHapticsNodeHD(nameSpace, *vrMain);
}

} /* namespace MinVR */
