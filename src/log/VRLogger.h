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
#include <typeinfo>

namespace MinVR {

namespace level {
typedef enum
{
    Verbose = 0,
    Debug = 1,
    Info = 2,
    Warn = 3,
    Error = 4,
    Critical = 5,
    Off = 6
} VRLogLevel;
}

struct VRLogString {
	std::string val;
};

struct VRLogObject : public VRLogString {
	template<typename T> VRLogObject(const T& obj) {
		std::stringstream ss;
		ss << obj;
		val = ss.str();
	}
};

namespace VRLog {
typedef enum {
	endl,
	flush
} VRLogAction;
}

class VRLoggerStreamInterface {
public:
	virtual ~VRLoggerStreamInterface() {}
	virtual void log(level::VRLogLevel lvl, const std::string& str) = 0;
	virtual void log(level::VRLogLevel lvl, const int& i) = 0;
	virtual void log(level::VRLogLevel lvl, const float& f) = 0;
	virtual void log(level::VRLogLevel lvl, const double& d) = 0;
	virtual void log(level::VRLogLevel lvl, const long& l) = 0;
	virtual void log(level::VRLogLevel lvl, const char& c) = 0;
	virtual void flush(level::VRLogLevel lvl) = 0;

	virtual void log(level::VRLogLevel lvl, const VRLog::VRLogAction& e) {
		if (e == VRLog::endl)
		{
			log(lvl, "\n");
			flush(lvl);
		}
		else if (e == VRLog::flush)
		{
			flush(lvl);
		}
	}

	virtual void log(level::VRLogLevel lvl, const VRLogString& obj) {
		log(lvl, obj.val);
	}
};

class VRLoggerStream {
public:
	VRLoggerStream(level::VRLogLevel lvl, VRLoggerStreamInterface* innerStream, bool shouldLog) : lvl(lvl), innerStream(innerStream), shouldLog(shouldLog) {}
	virtual ~VRLoggerStream() {}

	template <typename T> VRLoggerStream operator<<(const T& val) {
		if (shouldLog) {
			innerStream->log(lvl, val);
		}

		return *this;
	}

	void flush() {
		innerStream->flush(lvl);
	}

private:
	VRLoggerStreamInterface* innerStream;
	level::VRLogLevel lvl;
	bool shouldLog;
};

class VRLogger {
public:
	virtual ~VRLogger() {}

	virtual void setLevel(level::VRLogLevel lvl) { loggerLevel = lvl; }
	virtual level::VRLogLevel getLevel() { return loggerLevel; }
	virtual bool shouldLog(level::VRLogLevel lvl) {
		return this->getLevel() < level::Off && lvl >= this->getLevel() && lvl < level::Off;
	}

	VRLoggerStream getStream(level::VRLogLevel lvl) {
		return VRLoggerStream(lvl, getStream(), shouldLog(lvl));
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
	VRLogger() : loggerLevel(level::Info) {}
	virtual VRLoggerStreamInterface* getStream() = 0;

private:

	struct VRLoggerMap {
		VRLoggerMap();
		~VRLoggerMap() {
			std::cout << "First " << this << std::endl;
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

} /* namespace DSP */


#endif /* VRLOGGER_H_ */
