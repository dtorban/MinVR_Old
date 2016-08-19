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

VRLogger::LoggerPtr VRLogger::currentLogger = VRLogger::LoggerPtr(new VRBasicLogger());
VRLogger::VRLoggerMap VRLogger::loggerMap;

} /* namespace MinVR */
