/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <log/impl/VRBasicLogger.h>
#include "log/VRLogger.h"
#include <iostream>

namespace MinVR {

VRBasicLogger::VRBasicLogger() {
}

VRBasicLogger::~VRBasicLogger() {
}

void VRBasicLogger::logMessage(level::VRLogLevel lvl, const std::string& msg) {
	if (this->getLevel() < level::Off && lvl >= this->getLevel()) {
		std::ostream* stream = lvl == level::Error ? &std::cerr : &std::cout;
		*stream << msg << std::endl;
	}
}

bool VRBasicLogger::canLog(level::VRLogLevel lvl) {
	return this->getLevel() < level::Off && lvl >= this->getLevel() && lvl < level::Off;
}

VRBasicLogger* VRBasicLogger::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VRBasicLogger();
}

VRLoggerStreamInterface* VRBasicLogger::getStream() {
	return this;
}

std::ostream& VRBasicLogger::getStream(level::VRLogLevel lvl) {
	return lvl == level::Error ? std::cerr : std::cout;
}

} /* namespace MinVR */


