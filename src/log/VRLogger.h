/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRLOGGER_H_
#define VRLOGGER_H_

#include <string>
#include <map>
#include <sstream>
#include <iostream>

namespace MinVR {

namespace level {
typedef enum
{
    Trace = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Critical = 5,
    Off = 6
} VRLogLevel;
}

template<typename T> struct VRLogHelper;
template <> struct VRLogHelper<std::string>;

class VRLogger {
public:
	virtual ~VRLogger() {}

	virtual void setLevel(level::VRLogLevel lvl) { loggerLevel = lvl; }
	virtual level::VRLogLevel getLevel() { return loggerLevel; }

	template <typename T> void log(level::VRLogLevel lvl, const T& val) {
		VRLogHelper<T>::logValue(lvl, val, this);
	}

	virtual void logMessage(level::VRLogLevel lvl, const std::string& msg) = 0;

	static std::string getAttributeName(){ return "loggerType"; };

	static void set(const std::string& name, VRLogger* logger) {
		std::map<std::string, VRLogger*>::iterator it = loggerMap.loggers.find(name);

		if (it != loggerMap.loggers.end()) {
			delete it->second;
		}

		loggerMap.loggers[name] = logger;

		if (name == "Default") {
			loggerMap.currentLogger = logger;
		}
	}

	static void set(VRLogger* logger) {
		set("Default", logger);
	}

	static VRLogger& get(const std::string& name) {
		if (name == "Default") {
			return get();
		}

		return *(loggerMap.loggers[name]);
	}

	static VRLogger& get() {
		return *(loggerMap.currentLogger);
	}

protected:
	VRLogger() : loggerLevel(level::Info) {
	}

private:

	struct VRLoggerMap {
		VRLoggerMap();
		~VRLoggerMap() {
			for (std::map<std::string, VRLogger*>::iterator it = loggers.begin(); it != loggers.end(); it++) {
				delete it->second;
			}
		}

		VRLogger* currentLogger;
		std::map<std::string, VRLogger*> loggers;
	};

	static VRLoggerMap loggerMap;

	level::VRLogLevel loggerLevel;
};

template<typename T> struct VRLogHelper {
	static void logValue(level::VRLogLevel lvl, const T& val, VRLogger* logger) {
		std::stringstream ss;
		ss << val;
		logger->logMessage(lvl, ss.str());

	}
};

template <> struct VRLogHelper<std::string> {
	static void logValue(level::VRLogLevel lvl, const std::string& val, VRLogger* logger) {
		logger->logMessage(lvl, val);
	}
};

} /* namespace DSP */


#endif /* VRLOGGER_H_ */
