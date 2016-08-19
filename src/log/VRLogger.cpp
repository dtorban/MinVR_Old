/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRLogger.h"
#include "log/impl/VRBasicLogger.h"

namespace MinVR {

VRLogger::VRLoggerMap VRLogger::loggerMap;

VRLogger::VRLoggerMap::VRLoggerMap() {
	currentLogger = new VRBasicLogger();
	loggers["Default"] = currentLogger;
}

VRLoggerStream& VRLogger::getStream(level::VRLogLevel lvl) {
	static VRLoggerStream defaultStream;
	return defaultStream;
}

} /* namespace MinVR */

