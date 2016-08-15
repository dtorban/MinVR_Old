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

namespace MinVR {

class VRBasicLogger : public VRLogger {
public:
	VRBasicLogger();
	virtual ~VRBasicLogger();

	void log(level::VRLogLevel lvl, const std::string& msg);
	void setLevel(level::VRLogLevel lvl);

private:
	level::VRLogLevel lvl;
};

class VRBasicLoggerImpl : public VRLoggerImpl {
public:
	VRBasicLoggerImpl() {}
	virtual ~VRBasicLoggerImpl() {}

	VRLogger* get(const std::string& key) {
		return &logger;
	}

private:
	static VRBasicLogger logger;
};

} /* namespace MinVR */

#endif /* VRSIMPLELOGGER_H_ */
