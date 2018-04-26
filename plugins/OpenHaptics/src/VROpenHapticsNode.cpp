/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <VROpenHapticsNode.h>
#include <iostream>

namespace MinVR {

VROpenHapticsNode::VROpenHapticsNode(const std::string& name, VRMainInterface &vrMain) : VRDisplayNode(name), vrMain(vrMain) {
	hapticsState.addData("IsHaptics", true);

	 /* Initialize the device, must be done before attempting to call any hd 
       functions. Passing in HD_DEFAULT_DEVICE causes the default device to be 
       initialized. */
	    hHD = hdInitDevice("Omni");
	    if (HD_DEVICE_ERROR(error = hdGetError())) 
	    {
	        hduPrintError(stderr, &error, "Failed to initialize haptic device");
	        fprintf(stderr, "\nPress any key to quit.\n");
	        //getch();
	    }

	    printf("Hello Haptic Device!\n");
	    printf("Found device model: %s.\n\n", hdGetString(HD_DEVICE_MODEL_TYPE));
		
	    hdEnable(HD_FORCE_OUTPUT);
	    hdEnable(HD_MAX_FORCE_CLAMPING);
	    hdStartScheduler();

	    /* Check for errors and abort if so. */
	    if (HD_DEVICE_ERROR(error = hdGetError()))
	    {
	        hduPrintError(stderr, &error, "Failed to start scheduler");
	        fprintf(stderr, "\nPress any key to quit.\n");
	    }
}

VROpenHapticsNode::~VROpenHapticsNode() {
	hdStopScheduler();
    hdUnschedule(hRenderHaptics);

    /* Disable the device. */
    hdDisableDevice(hHD);
}

struct HapticsData {
	VROpenHapticsNode* node;
	VRDataIndex* renderState;
	VRRenderHandler* renderHandler;
};

void VROpenHapticsNode::render(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {

    bool initRender = (int)renderState->getValue("InitRender","/") == 1;

    hdMakeCurrentDevice(hHD);

	renderState->pushState();
	renderState->addData("IsHaptics", true);
    HapticsData hapticsData;
    hapticsData.node = this;
    hapticsData.renderState = renderState;
    hapticsData.renderHandler = renderHandler;
    // schedule haptics synchronous
    hdScheduleSynchronous(setHapticsState, &hapticsData,
                          HD_MIN_SCHEDULER_PRIORITY);
	renderState->popState();

    if (initRender) {
    	// schedule haptics asynchronous
	    hRenderHaptics = hdScheduleAsynchronous(
	        renderHaptics, this, 
	        HD_MAX_SCHEDULER_PRIORITY);
    }

}

void VROpenHapticsNode::setHapticsState(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {
	renderHandlers = vrMain.getRenderHandlers();
	renderHandler->onVRRenderContext(*renderState);
}

void VROpenHapticsNode::renderHaptics() {
	for (int f = 0; f < renderHandlers.size(); f++) {
		renderHandlers[f]->onVRRenderScene(hapticsState);
	}
}

HDCallbackCode HDCALLBACK VROpenHapticsNode::setHapticsState(void *data) {
	HapticsData* hapticsData = static_cast<HapticsData*>(data);
	hapticsData->node->setHapticsState(hapticsData->renderState, hapticsData->renderHandler);

    // execute this only once.
    return HD_CALLBACK_DONE;
}

HDCallbackCode HDCALLBACK VROpenHapticsNode::renderHaptics(void *data) {
	VROpenHapticsNode* node = static_cast<VROpenHapticsNode*>(data);
	
    /* Begin haptics frame.  ( In general, all state-related haptics calls
       should be made within a frame. ) */
    hdBeginFrame(hHD);

    node->renderHaptics();

	/* End haptics frame. */
    hdEndFrame(hHD);

    /* Check for errors and abort the callback if a scheduler error
       is detected. */
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, 
                      "Error detected while rendering gravity well\n");
        
        if (hduIsSchedulerError(&error))
        {
            return HD_CALLBACK_DONE;
        }
    }

    /* Signify that the callback should continue running, i.e. that
       it will be called again the next scheduler tick. */
    return HD_CALLBACK_CONTINUE;
}

VRDisplayNode* VROpenHapticsNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VROpenHapticsNode(nameSpace, *vrMain);
}

} /* namespace MinVR */
