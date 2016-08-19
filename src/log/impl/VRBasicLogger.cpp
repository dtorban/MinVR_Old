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

void VRBasicLogger::log(level::VRLogLevel lvl, const std::string& msg) {
	if (this->getLevel() < level::off && lvl >= this->getLevel()) {
		//std::ostream* stream = lvl == level::err ? &std::cerr : &std::cout;
		std::ostream* stream = &std::cout;
		*stream << msg << std::endl;
	}
}

VRLoggerStream& VRBasicLogger::getStream(level::VRLogLevel lvl) {
	if (this->getLevel() < level::off && lvl >= this->getLevel()) {
		return stream;
	}

	return VRLogger::getStream(lvl);
}

VRBasicLogger* VRBasicLogger::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VRBasicLogger();
}

VRBasicLogger::VRBasicLoggerStream::~VRBasicLoggerStream() {
}

void VRBasicLogger::VRBasicLoggerStream::flush() {
	std::cout.flush();
}

std::ostream* VRBasicLogger::VRBasicLoggerStream::getStream() {
	return &std::cout;
}

} /* namespace MinVR */


