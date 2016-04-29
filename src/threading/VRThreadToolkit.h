/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRTHREADTOOLKIT_H_
#define VRTHREADTOOLKIT_H_

#include "VRThread.h"

namespace MinVR {

class VRThreadToolkit {
public:
	virtual ~VRThreadToolkit() {}

	virtual VRThread* createThread(VRThreadHandler& threadHandler) = 0;
	virtual VRMutex* createMutex() = 0;
	virtual VRConditionVariable* createConditionVariable() = 0;
};

} /* namespace MinVR */

#endif /* VRTHREADTOOLKIT_H_ */
