/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRTHREAD_H_
#define VRTHREAD_H_

#include "VRThreadHandler.h"

namespace MinVR {

class VRThread {
public:
	virtual ~VRThread() {}

	virtual void join() = 0;
};

class VRUniqueMutexLock {
public:
	virtual ~VRUniqueMutexLock() {}

	virtual void unlock() = 0;
};

class VRMutex {
public:
	virtual ~VRMutex() {}

	virtual void lock() = 0;
	virtual void unlock() = 0;

	virtual VRUniqueMutexLock* createUniqueLock() = 0;
};

class VRConditionVariable {
public:
	virtual ~VRConditionVariable() {}

	virtual void notifyAll() = 0;
	virtual void wait(VRUniqueMutexLock* lock) = 0;
};

} /* namespace MinVR */

#endif /* VRTHREAD_H_ */
