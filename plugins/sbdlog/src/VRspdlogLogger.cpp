/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRspdlogLogger.h"
#include <sstream>

namespace MinVR {

VRspdlogLogger::VRspdlogLogger(std::shared_ptr<spdlog::logger> logger) : logger(logger) {
}

VRspdlogLogger::~VRspdlogLogger() {
}

class VRspdlogLoggerImpl : public VRLoggerStreamInterface {
public:
	VRspdlogLoggerImpl(std::shared_ptr<spdlog::logger> logger) : logger(logger), hasValue(false) {}
	virtual ~VRspdlogLoggerImpl() {}

	void log(VRLog::VRLogLevel lvl, const std::string& str);
	void log(VRLog::VRLogLevel lvl, const int& i);
	void log(VRLog::VRLogLevel lvl, const float& f);
	void log(VRLog::VRLogLevel lvl, const double& d);
	void log(VRLog::VRLogLevel lvl, const long& l);
	void log(VRLog::VRLogLevel lvl, const char& c);
	void log(VRLog::VRLogLevel lvl, const VRLog::VRLogAction& e);
	void flush(VRLog::VRLogLevel lvl);
	void finish(VRLog::VRLogLevel lvl);

private:
	std::shared_ptr<spdlog::logger> logger;
	std::stringstream ss;
	bool hasValue;
};

void VRspdlogLoggerImpl::log(VRLog::VRLogLevel lvl, const std::string& str) {
	ss << str;
	hasValue = true;
}

void VRspdlogLoggerImpl::log(VRLog::VRLogLevel lvl, const int& i) {
	ss << i;
	hasValue = true;
}

void VRspdlogLoggerImpl::log(VRLog::VRLogLevel lvl, const float& f) {
	ss << f;
	hasValue = true;
}

void VRspdlogLoggerImpl::log(VRLog::VRLogLevel lvl, const double& d) {
	ss << d;
	hasValue = true;
}

void VRspdlogLoggerImpl::log(VRLog::VRLogLevel lvl, const long & l) {
	ss << l;
	hasValue = true;
}

void VRspdlogLoggerImpl::log(VRLog::VRLogLevel lvl, const char& c) {
	ss << c;
	hasValue = true;
}

void VRspdlogLoggerImpl::flush(VRLog::VRLogLevel lvl) {
	if (hasValue) {
		logger->log((spdlog::level::level_enum)lvl, ss.str());
		ss.clear();
	}

	hasValue = false;
}

void VRspdlogLoggerImpl::finish(VRLog::VRLogLevel lvl) {
	flush(lvl);
	delete this;
}

void VRspdlogLoggerImpl::log(VRLog::VRLogLevel lvl, const VRLog::VRLogAction& e) {
	if (e != VRLog::endl) {
		VRLoggerStreamInterface::log(lvl, e);
	}
	else {
		flush(lvl);
	}
}

VRLoggerStreamInterface* VRspdlogLogger::getStream() {
	return new VRspdlogLoggerImpl(logger);
}

VRLogger* VR_stdout_logger_mt::create(VRMainInterface* vrMain,
	VRDataIndex* config, const std::string& nameSpace) {

	std::string name = config->getDatum(nameSpace)->getAttributeValue("name");
	bool color = (int)config->getValue("Color", nameSpace);

	std::shared_ptr<spdlog::logger> logger = spdlog::stdout_logger_mt(name, true);
	return new VRspdlogLogger(logger);
}

VRLogger* VR_basic_logger_mt::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {

	std::string name = config->getDatum(nameSpace)->getAttributeValue("name");
	std::string fileName = config->getValue("FileName", nameSpace);

	std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt(name, fileName);
	return new VRspdlogLogger(logger);
}

} /* namespace DSP */


