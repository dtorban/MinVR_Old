/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRCPP11THREADTOOLKIT_H_
#define VRCPP11THREADTOOLKIT_H_

#include <plugin/VRPlugin.h>
#include <threading/VRThreadToolkit.h>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace MinVR {

class VRCpp11ThreadToolkit : public VRThreadToolkit {
public:
	PLUGIN_API VRCpp11ThreadToolkit();
	PLUGIN_API virtual ~VRCpp11ThreadToolkit();

	PLUGIN_API VRThread* createThread(VRThreadHandler& threadHandler);
	PLUGIN_API VRMutex* createMutex();
	PLUGIN_API VRConditionVariable* createConditionVariable();
};

class VRCpp11Thread : public VRThread {
public:
	PLUGIN_API VRCpp11Thread(VRThreadHandler& threadHandler);
	PLUGIN_API virtual ~VRCpp11Thread();

	PLUGIN_API void join();

private:
	std::thread* m_thread;
};

class VRCpp11UniqueMutexLock : public VRUniqueMutexLock {
public:
	VRCpp11UniqueMutexLock(std::mutex& mutex);
	virtual ~VRCpp11UniqueMutexLock() {}

	void unlock();

	std::unique_lock<std::mutex>& getUniqueLock() {
		return m_uniqueLock;
	}

private:
	std::unique_lock<std::mutex> m_uniqueLock;
};

class VRCpp11Mutex : public VRMutex {
public:
	PLUGIN_API VRCpp11Mutex() {}
	PLUGIN_API virtual ~VRCpp11Mutex() {}

	PLUGIN_API void lock();
	PLUGIN_API void unlock();
	PLUGIN_API VRUniqueMutexLock* createUniqueLock();

private:
	std::mutex m_mutex;
};

class VRCpp11ConditionVariable : public VRConditionVariable {
public:
	PLUGIN_API VRCpp11ConditionVariable() {}
	PLUGIN_API virtual ~VRCpp11ConditionVariable() {}

	PLUGIN_API void notifyAll();
	PLUGIN_API void wait(VRUniqueMutexLock* lock);

private:
	std::condition_variable m_cond;
};

} /* namespace MinVR */

#endif /* VRCPP11THREADTOOLKIT_H_ */
