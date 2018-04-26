/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VROPENHAPTICSNODE_H
#define VROPENHAPTICSNODE_H

#include "display/VRDisplayNode.h"
#include <main/VRMainInterface.h>


//#include <HD/hd.h>
//#include <HDU/hduError.h>
//#include <HDU/hduVector.h>

#define HDCallbackCode 
#define HDCALLBACK void
#define HD_CALLBACK_DONE 
#define HD_CALLBACK_CONTINUE 

namespace MinVR {


/**
 * VRThreadGroupNode runs each of the child display nodes in their own thread.  It synchronizes the three main
 * display node functions render, waitForRenderToComplete, and displayFinishedRendering.  This allows multithreading
 * at the display node level, while enforcing that nodes are displayed at the same time.  It also allows for
 * the potential for other threads in MinVR to use the time between render and waitForRenderComplete.  It is possible
 * to nest the VRThreadGroupNodes enableing multi levels of parallel processing.
 */
class VROpenHapticsNode : public VRDisplayNode {
public:
	VROpenHapticsNode(const std::string &name, VRMainInterface &vrMain);
	virtual ~VROpenHapticsNode();

	virtual std::string getType() const { return "VROpenHapticsNode"; }

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

	HHD hHD;
	HDErrorInfo error;
    HDSchedulerHandle hRenderHaptics;
};

} /* namespace MinVR */

#endif /* VRTHREADGROUPNODE_H_ */
