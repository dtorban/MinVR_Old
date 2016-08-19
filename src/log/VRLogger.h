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
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    err = 4,
    critical = 5,
    off = 6
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

	template <typename T> void trace(const T& val) { log(level::trace,  val); }
	template <typename T> void debug(const T& val) { log(level::debug,  val); }
	template <typename T> void info(const T& val) { log(level::info,  val); }
	template <typename T> void warn(const T& val) { log(level::warn,  val); }
	template <typename T> void error(const T& val) { log(level::err,  val); }
	template <typename T> void critical(const T& val) { log(level::critical,  val); }

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
	VRLogger() : loggerLevel(level::info) {
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
