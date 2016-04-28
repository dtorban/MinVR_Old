/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRCpp11ThreadToolkit.h"

namespace MinVR {

VRCpp11ThreadToolkit::VRCpp11ThreadToolkit() {
}

VRCpp11ThreadToolkit::~VRCpp11ThreadToolkit() {
}

VRCpp11Thread::VRCpp11Thread(VRThreadHandler& threadHandler) {
	m_thread = new std::thread(&VRThreadHandler::onVRThreadRun, &threadHandler);
}

VRCpp11Thread::~VRCpp11Thread() {
	if (m_thread && m_thread->joinable())
	{
		m_thread->join();
		delete m_thread;
	}
}

void VRCpp11Thread::join() {
	m_thread->join();
}

VRCpp11UniqueMutexLock::VRCpp11UniqueMutexLock(std::mutex& mutex) : m_uniqueLock(mutex) {
}

void VRCpp11UniqueMutexLock::unlock() {
	m_uniqueLock.unlock();
}

void VRCpp11Mutex::lock() {
	m_mutex.lock();
}

void VRCpp11Mutex::unlock() {
	m_mutex.unlock();
}

VRUniqueMutexLock* VRCpp11Mutex::createUniqueLock() {
	return new VRCpp11UniqueMutexLock(m_mutex);
}


VRThread* VRCpp11ThreadToolkit::createThread(VRThreadHandler& threadHandler) {
	return new VRCpp11Thread(threadHandler);
}

VRMutex* VRCpp11ThreadToolkit::createMutex() {
	return new VRCpp11Mutex();
}

VRConditionVariable* VRCpp11ThreadToolkit::createConditionVariable() {
	return NULL;
}

void VRCpp11ConditionVariable::notifyAll() {
	m_cond.notify_all();
}

void VRCpp11ConditionVariable::wait(VRUniqueMutexLock* lock) {
	VRCpp11UniqueMutexLock* ulock = dynamic_cast<VRCpp11UniqueMutexLock*>(lock);
	if (ulock != NULL)
	{
		m_cond.wait(ulock->getUniqueLock());
	}
}


} /* namespace MinVR */
