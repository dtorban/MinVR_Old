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

	device = new VROpenHapticsDevice("Omni");
	vrMain.addInputDevice(device);
}

VROpenHapticsNode::~VROpenHapticsNode() {
#ifdef WITH_HAPTICS
    hdUnschedule(hRenderHaptics);
#endif
	delete device;
}

struct HapticsData {
	VROpenHapticsNode* node;
	VRDataIndex* renderState;
	VRRenderHandler* renderHandler;
};

void VROpenHapticsNode::render(VRDataIndex* renderState, VRRenderHandler* renderHandler) {

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

void VROpenHapticsNode::setHapticsState(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {
	renderHandlers = vrMain.getRenderHandlers();
	renderHandler->onVRRenderContext(*renderState);
}

void VROpenHapticsNode::renderHaptics() {
	device->beginUpdate();

	for (int f = 0; f < renderHandlers.size(); f++) {
		renderHandlers[f]->onVRRenderScene(hapticsState);
	}

	device->endUpdate();
}

HDCallbackCode HDCALLBACK VROpenHapticsNode::setHapticsState(void *data) {
	HapticsData* hapticsData = static_cast<HapticsData*>(data);
	hapticsData->node->setHapticsState(hapticsData->renderState, hapticsData->renderHandler);

    // execute this only once.
    return HD_CALLBACK_DONE;
}

HDCallbackCode HDCALLBACK VROpenHapticsNode::renderHaptics(void *data) {
	VROpenHapticsNode* node = static_cast<VROpenHapticsNode*>(data);
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

VRDisplayNode* VROpenHapticsNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {

	return new VROpenHapticsNode(nameSpace, *vrMain);
}

} /* namespace MinVR */
