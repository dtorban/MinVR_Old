/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <threading/VRThreadBarrier.h>

namespace MinVR {

VRThreadBarrier::VRThreadBarrier(unsigned int numThreads, VRThreadToolkit* threadToolkit) : m_numThreads(numThreads), m_count(numThreads), m_generation(0) {
	m_mutex = threadToolkit->createMutex();
	m_cond = threadToolkit->createConditionVariable();
}

VRThreadBarrier::~VRThreadBarrier() {
	 delete m_mutex;
	 delete m_cond;
}

bool VRThreadBarrier::wait() {
    VRUniqueMutexLock* lock = m_mutex->createUniqueLock();
    unsigned int gen = m_generation;

    if (--m_count == 0)
    {
    	m_generation++;
		m_count = m_numThreads;
		m_cond->notifyAll();
		return true;
    }

    while (gen == m_generation) {
        m_cond->wait(lock);
    }

    delete lock;

    return false;
}

} /* namespace MinVR */
