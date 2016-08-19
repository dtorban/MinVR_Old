/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRSIMPLELOGGER_H_
#define VRSIMPLELOGGER_H_

#include "log/VRLogger.h"
#include "main/VRMainInterface.h"

namespace MinVR {

class VRBasicLogger : public VRLogger {
public:
	VRBasicLogger();
	virtual ~VRBasicLogger();

	void logMessage(level::VRLogLevel lvl, const std::string& msg);

	static VRBasicLogger* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);
};

} /* namespace MinVR */

#endif /* VRSIMPLELOGGER_H_ */
