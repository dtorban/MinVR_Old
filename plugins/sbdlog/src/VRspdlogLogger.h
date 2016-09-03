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
#include "spdlog/spdlog.h"
#include "log/VRLogger.h"

namespace MinVR {

class VRspdlogLogger : public VRLogger {
public:
	VRspdlogLogger(std::shared_ptr<spdlog::logger> logger);
	virtual ~VRspdlogLogger();

	VRLoggerStreamInterface* getStream();

private:
	std::shared_ptr<spdlog::logger> logger;
};

class VR_stdout_logger_mt : public VRspdlogLogger {
public:
	VR_stdout_logger_mt(std::shared_ptr<spdlog::logger> logger) : VRspdlogLogger(logger) {}
	static VRLogger* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);
};

class VR_basic_logger_mt : public VRspdlogLogger {
public:
	VR_basic_logger_mt(std::shared_ptr<spdlog::logger> logger) : VRspdlogLogger(logger) {}
	static VRLogger* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);
};

} /* namespace DSP */

#endif /* VRSPDLOGLOGGER_H_ */
