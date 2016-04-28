/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRCONDITIONVARIABLE_H_
#define VRCONDITIONVARIABLE_H_

#include "VRMutex.h"

namespace MinVR {

class VRConditionVariable {
public:
	virtual ~VRConditionVariable() {}

	virtual void notifyAll() = 0;
	virtual void wait(VRUniqueMutexLock* lock) = 0;
};

} /* namespace MinVR */

#endif /* VRCONDITIONVARIABLE_H_ */
