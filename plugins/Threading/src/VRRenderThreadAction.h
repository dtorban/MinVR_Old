/*
 * VRRenderThreadAction.h
 *
 *  Created on: Sep 26, 2018
 *      Author: dan
 */

#ifndef VRRENDERTHREADACTION_H_
#define VRRENDERTHREADACTION_H_

namespace MinVR {
/**
 * VRRenderThreadAction enables notification of what type of action
 * is being called for the whole thread group
 */
enum VRRenderThreadAction {
	THREADACTION_None,
	THREADACTION_Init,
	THREADACTION_Render,
	THREADACTION_WaitForRenderToComplete,
	THREADACTION_DisplayFinishedRendering,
	THREADACTION_Terminate };
}

#endif /* CMAKE_FRAMEWORK_EXTERNAL_MINVR_SRC_PLUGINS_THREADING_SRC_VRRENDERTHREADACTION_H_ */
