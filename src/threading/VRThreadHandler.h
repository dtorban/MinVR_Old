/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRTHREADHANDLER_H_
#define VRTHREADHANDLER_H_

namespace MinVR {

class VRThreadHandler {
public:
	virtual ~VRThreadHandler() {}

	virtual void onVRThreadRun() = 0;
};

} /* namespace MinVR */

#endif /* VRTHREADHANDLER_H_ */
