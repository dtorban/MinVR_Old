/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRSPDLOGLOGGER_H_
#define VRSPDLOGLOGGER_H_

#include "main/VRMainInterface.h"

namespace MinVR {

class VRspdlogLogger {
public:
	VRspdlogLogger();
	virtual ~VRspdlogLogger();
};

class VRspdlogLoggerImpl : public VRLoggerImpl {
public:
	VRspdlogLoggerImpl(VRMainInterface* vrmain);
	virtual ~VRspdlogLoggerImpl();

	VRLogger* get(const std::string& key);
};

} /* namespace DSP */

#endif /* VRSPDLOGLOGGER_H_ */
