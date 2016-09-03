/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRspdlogLogger.h"

namespace MinVR {

VRspdlogLogger::VRspdlogLogger(spdlog::logger* logger) : logger(logger) {
}

VRspdlogLogger::~VRspdlogLogger() {
	delete logger;
}

void VRspdlogLogger::log(VRLog::VRLogLevel lvl, const std::string& str) {
	logger->log((spdlog::level::level_enum)lvl, str);
}

void VRspdlogLogger::log(VRLog::VRLogLevel lvl, const int& i) {
	logger->log((spdlog::level::level_enum)lvl, i);
}

void VRspdlogLogger::log(VRLog::VRLogLevel lvl, const float& f) {
	logger->log((spdlog::level::level_enum)lvl, f);
}

void VRspdlogLogger::log(VRLog::VRLogLevel lvl, const double& d) {
	logger->log((spdlog::level::level_enum)lvl, d);
}

void VRspdlogLogger::log(VRLog::VRLogLevel lvl, const long & l) {
	logger->log((spdlog::level::level_enum)lvl, l);
}

void VRspdlogLogger::log(VRLog::VRLogLevel lvl, const char& c) {
	logger->log((spdlog::level::level_enum)lvl, c);
}

void VRspdlogLogger::flush(VRLog::VRLogLevel lvl) {
	logger->flush();
}

VRLoggerStreamInterface* VRspdlogLogger::getStream() {
	return this;
}

} /* namespace DSP */

