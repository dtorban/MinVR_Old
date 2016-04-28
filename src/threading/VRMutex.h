/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRMUTEX_H_
#define VRMUTEX_H_

namespace MinVR {

class VRUniqueMutexLock {
public:
	~VRUniqueMutexLock() {}

	virtual void unlock() = 0;
};

class VRMutex {
public:
	virtual ~VRMutex() {}

	virtual void lock() = 0;
	virtual void unlock() = 0;

	virtual VRUniqueMutexLock* createUniqueLock() = 0;
};

} /* namespace MinVR */

#endif /* VRMUTEX_H_ */
