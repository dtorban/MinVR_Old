/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VROpenHapticsNodeHL_H
#define VROpenHapticsNodeHL_H

#include "display/VRDisplayNode.h"
#include <main/VRMainInterface.h>


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

#include "VROpenHapticsDeviceHL.h"

namespace MinVR {


/**
 * VRThreadGroupNode runs each of the child display nodes in their own thread.  It synchronizes the three main
 * display node functions render, waitForRenderToComplete, and displayFinishedRendering.  This allows multithreading
 * at the display node level, while enforcing that nodes are displayed at the same time.  It also allows for
 * the potential for other threads in MinVR to use the time between render and waitForRenderComplete.  It is possible
 * to nest the VRThreadGroupNodes enableing multi levels of parallel processing.
 */
class VROpenHapticsNodeHL : public VRDisplayNode {
public:
	VROpenHapticsNodeHL(const std::string &name, VRMainInterface &vrMain);
	virtual ~VROpenHapticsNodeHL();

	virtual std::string getType() const { return "VROpenHapticsNodeHL"; }

	virtual void render(VRDataIndex *renderState, VRRenderHandler *renderHandler);

	static VRDisplayNode* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);

private:
	void setHapticsState(VRDataIndex* renderState, VRRenderHandler* renderHandler);
	void renderHaptics();
	static HDCallbackCode HDCALLBACK setHapticsState(void *data);
	static HDCallbackCode HDCALLBACK renderHaptics(void *data);

	VRMainInterface &vrMain;
	std::vector<VRRenderHandler*> renderHandlers;
	VRDataIndex hapticsState;
	VROpenHapticsDeviceHL* device;

#ifdef WITH_HAPTICS
	HDErrorInfo error;
    HDSchedulerHandle hRenderHaptics;
#endif
};

} /* namespace MinVR */

#endif /* VRTHREADGROUPNODE_H_ */
