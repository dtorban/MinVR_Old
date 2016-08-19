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

class VRLoggerStream;

class VRLogger {
public:
	virtual ~VRLogger() {}

	virtual void log(level::VRLogLevel lvl, const std::string& msg) = 0;

	virtual void setLevel(level::VRLogLevel lvl) { loggerLevel = lvl; }
	virtual level::VRLogLevel getLevel() { return loggerLevel; }

	void log(const std::string& msg) { log(loggerLevel, msg); }
	void trace(const std::string& msg) { log(level::trace, msg); }
	void debug(const std::string& msg) { log(level::debug, msg); }
	void info(const std::string& msg) { log(level::info, msg); }
	void warn(const std::string& msg) { log(level::warn, msg); }
	void error(const std::string& msg) { log(level::err, msg); }
	void critical(const std::string& msg) { log(level::critical, msg); }

	virtual VRLoggerStream& getStream(level::VRLogLevel lvl);
	virtual VRLoggerStream& getStream() {
		return getStream(loggerLevel);
	}

	template<typename T>
	VRLoggerStream& operator<<(T val) {
		return getStream(loggerLevel);
	}

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

class VRLoggerStream {
public:
	virtual ~VRLoggerStream() {}

	template<typename T>
	VRLoggerStream& operator<<(T val) {
		std::ostream* stream = getStream();
		if(stream) {
			*stream << val;
		}

		return *this;
	}

	virtual void flush() {}

protected:
	virtual std::ostream* getStream() { return NULL; }
};



} /* namespace DSP */


#endif /* VRLOGGER_H_ */
