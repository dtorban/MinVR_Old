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

class VRspdlogLogger : public VRLogger, public VRLoggerStreamInterface {
public:
	VRspdlogLogger(spdlog::logger* logger);
	virtual ~VRspdlogLogger();

	void log(VRLog::VRLogLevel lvl, const std::string& str);
	void log(VRLog::VRLogLevel lvl, const int& i);
	void log(VRLog::VRLogLevel lvl, const float& f);
	void log(VRLog::VRLogLevel lvl, const double& d);
	void log(VRLog::VRLogLevel lvl, const long& l);
	void log(VRLog::VRLogLevel lvl, const char& c);
	void flush(VRLog::VRLogLevel lvl);

	VRLoggerStreamInterface* getStream();

private:
	spdlog::logger* logger;
};

} /* namespace DSP */

#endif /* VRSPDLOGLOGGER_H_ */
