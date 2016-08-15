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

class VRLogger;

class VRLoggerImpl {
public:
	virtual ~VRLoggerImpl() {}
	virtual VRLogger* get(const std::string& key) = 0;
};

class VRBasicLoggerImpl;

class VRLogger {
public:
	virtual ~VRLogger() {}

	virtual void log(level::VRLogLevel lvl, const std::string& msg) = 0;
	virtual void setLevel(level::VRLogLevel lvl) = 0;

	void trace(const std::string& msg) { log(level::trace, msg); }
	void debug(const std::string& msg) { log(level::debug, msg); }
	void info(const std::string& msg) { log(level::info, msg); }
	void warn(const std::string& msg) { log(level::warn, msg); }
	void error(const std::string& msg) { log(level::err, msg); }
	void critical(const std::string& msg) { log(level::critical, msg); }

	static void setImpl(VRLoggerImpl* implementation) {
		impl = implementation;
	}

	static VRLogger* get(const std::string& key) {
		if (impl == NULL) {
			impl = reinterpret_cast<VRLoggerImpl*>(&basicImpl);
		}

		return impl->get(key);
	}

	static VRLogger* get() {
		return get("default");
	}

private:

	static VRLoggerImpl* impl;
	static VRBasicLoggerImpl basicImpl;
};


} /* namespace DSP */

#endif /* VRLOGGER_H_ */
