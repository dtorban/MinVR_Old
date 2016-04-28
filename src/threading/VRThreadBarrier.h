/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRTHREADBARRIER_H_
#define VRTHREADBARRIER_H_

#include "VRThreadToolkit.h"

namespace MinVR {

class VRThreadBarrier {
public:
	VRThreadBarrier(unsigned int numThreads, VRThreadToolkit* threadToolkit);
	virtual ~VRThreadBarrier();

	bool wait();

private:
    unsigned int m_numThreads;
    VRMutex* m_mutex;
    VRConditionVariable* m_cond;
    unsigned int m_count;
    unsigned int m_generation;
};



} /* namespace MinVR */

#endif /* VRTHREADBARRIER_H_ */
