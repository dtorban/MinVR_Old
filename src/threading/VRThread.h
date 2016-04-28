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

} /* namespace MinVR */

#endif /* VRTHREAD_H_ */
