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

class VRLogger {
public:
	virtual ~VRLogger() {}

	void log(level::VRLogLevel lvl, const std::string& msg) = 0;
	void setLevel(level::VRLogLevel lvl) = 0;

	void trace(const std::string& msg) { log(level::trace, msg); }
	void debug(const std::string& msg) { log(level::debug, msg); }
	void info(const std::string& msg) { log(level::info, msg); }
	void warn(const std::string& msg) { log(level::warn, msg); }
	void error(const std::string& msg) { log(level::err, msg); }
	void critical(const std::string& msg) { log(level::critical, msg); }

	static VRLogger* get(const std::string& key) {
		return loggers[key];
	}

	static void set(const std::string& key, VRLogger* logger) {
		std::map<std::string, VRLogger*>::iterator it = loggers.find(key);
		if (it != loggers.end()) {
			delete it->second;
		}

		loggers[key] = logger;
	}

	static VRLogger* get() { return get("default"); }
	static void set(VRLogger* logger) { set("default", logger); }

private:
	std::map<std::string, VRLogger*> loggers;
};

} /* namespace DSP */

#endif /* VRLOGGER_H_ */
