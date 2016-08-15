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

VRBasicLogger VRBasicLoggerImpl::logger;

VRBasicLogger::VRBasicLogger() : lvl(level::info) {
}

VRBasicLogger::~VRBasicLogger() {
}

void VRBasicLogger::log(level::VRLogLevel lvl, const std::string& msg) {
	if (this->lvl < level::off && lvl >= this->lvl) {
		std::ostream* stream = lvl < level::info ? &std::cerr : &std::cout;
		*stream << msg << std::endl;
	}
}

void VRBasicLogger::setLevel(level::VRLogLevel lvl) {
	this->lvl = lvl;
}

} /* namespace MinVR */
