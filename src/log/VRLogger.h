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

struct VRLog {
	static VRLog endl;
	static VRLog flush;
};

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

	virtual void log(level::VRLogLevel lvl, const VRLog& e) {
		if (&e == &(VRLog::endl))
		{
			log(lvl, "\n");
			flush(lvl);
		}
		else if (&e == &(VRLog::flush))
		{
			flush(lvl);
		}
	}

	virtual void log(level::VRLogLevel lvl, const VRLogString& obj) {
		std::cout << "---specific---";
		log(lvl, obj.val);
	}
};

template<typename T> struct VRLogHelper {
	static void logValue(level::VRLogLevel lvl, const T& val, VRLoggerStreamInterface* innerStream) {
		std::stringstream ss;
		ss << val;
		innerStream->log(lvl, ss.str());
	}
};

template <typename T> struct VRSimpleLogHelper {
	static void logValue(level::VRLogLevel lvl, const T& val, VRLoggerStreamInterface* innerStream) {
		innerStream->log(lvl, val);
	}
};

template <> struct VRLogHelper<std::string> : public VRSimpleLogHelper<std::string> {};
template <> struct VRLogHelper<char*> : public VRSimpleLogHelper<char*> {};
template <> struct VRLogHelper<int> : public VRSimpleLogHelper<int> {};
template <> struct VRLogHelper<VRLog> : public VRSimpleLogHelper<VRLog> {};
template <> struct VRLogHelper<char> : public VRSimpleLogHelper<char> {};
template <> struct VRLogHelper<float> : public VRSimpleLogHelper<float> {};
template <> struct VRLogHelper<double> : public VRSimpleLogHelper<double> {};

class VRLoggerStream {
public:
	VRLoggerStream(level::VRLogLevel lvl, VRLoggerStreamInterface* innerStream) : lvl(lvl), innerStream(innerStream) {}
	virtual ~VRLoggerStream() {}

	template <typename T> VRLoggerStream operator<<(const T& val) {
		//VRLogHelper<T>::logValue(lvl, val, innerStream);
		innerStream->log(lvl, val);
		return *this;
	}

private:
	VRLoggerStreamInterface* innerStream;
	level::VRLogLevel lvl;
};

class VRLogger {
public:
	virtual ~VRLogger() {}

	virtual void setLevel(level::VRLogLevel lvl) { loggerLevel = lvl; }
	virtual level::VRLogLevel getLevel() { return loggerLevel; }

	virtual void logMessage(level::VRLogLevel lvl, const std::string& msg) = 0;

	virtual VRLoggerStream operator<<(level::VRLogLevel lvl) {
		return VRLoggerStream(lvl, getStream());
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
