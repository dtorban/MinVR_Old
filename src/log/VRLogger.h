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
#include "VRLogManager.h"

namespace MinVR {

namespace VRLog {
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

typedef enum {
	endl,
	flush
} VRLogAction;
}

class VRLogValue {
public:
	virtual ~VRLogValue() {}
	virtual void logValue(std::ostream& stream) const = 0;
};

template <typename T>
struct VRLogObject : public VRLogValue {
public:
	VRLogObject(const T& obj) : obj(obj) {
	}
	virtual ~VRLogObject() {}

	void logValue(std::ostream& stream) const {
		stream << obj;
	}

private:
	const T& obj;
};

class VRLoggerStreamInterface {
public:
	virtual ~VRLoggerStreamInterface() {}
	virtual void log(VRLog::VRLogLevel lvl, const std::string& str) = 0;
	virtual void log(VRLog::VRLogLevel lvl, const int& i) = 0;
	virtual void log(VRLog::VRLogLevel lvl, const float& f) = 0;
	virtual void log(VRLog::VRLogLevel lvl, const double& d) = 0;
	virtual void log(VRLog::VRLogLevel lvl, const long& l) = 0;
	virtual void log(VRLog::VRLogLevel lvl, const char& c) = 0;
	virtual void flush(VRLog::VRLogLevel lvl) = 0;

	virtual void log(VRLog::VRLogLevel lvl, const VRLog::VRLogAction& e) {
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

	virtual void log(VRLog::VRLogLevel lvl, const VRLogValue& obj) {
		std::stringstream ss;
		obj.logValue(ss);
		log(lvl, ss.str());
	}
};

class VRLoggerStream {
public:
	VRLoggerStream(VRLog::VRLogLevel lvl, VRLoggerStreamInterface* innerStream, bool shouldLog) : lvl(lvl), innerStream(innerStream), shouldLog(shouldLog) {}
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
	VRLog::VRLogLevel lvl;
	bool shouldLog;
};

class VRLogger {
public:
	virtual ~VRLogger() {}

	virtual void setLevel(VRLog::VRLogLevel lvl) { loggerLevel = lvl; }
	virtual VRLog::VRLogLevel getLevel() { return loggerLevel; }
	virtual bool shouldLog(VRLog::VRLogLevel lvl) {
		return this->getLevel() < VRLog::Off && lvl >= this->getLevel() && lvl < VRLog::Off;
	}

	VRLoggerStream getStream(VRLog::VRLogLevel lvl) {
		return VRLoggerStream(lvl, getStream(), shouldLog(lvl));
	}

	VRLoggerStream operator<<(VRLog::VRLogLevel lvl) {
		return getStream(lvl);
	}

	static std::string getAttributeName(){ return "loggerType"; };

	static void set(const std::string& name, VRLogger* logger) {
		VRLogManager::getInstance()->set(name, logger);
	}

	static void set(VRLogger* logger) {
		VRLogManager::getInstance()->set(logger);
	}

	static VRLogger& get(const std::string& name) {
		return VRLogManager::getInstance()->get(name);
	}

	static VRLogger& get() {
		return VRLogManager::getInstance()->get();
	}

protected:
	VRLogger() : loggerLevel(VRLog::Info) {}
	virtual VRLoggerStreamInterface* getStream() = 0;

private:
	VRLog::VRLogLevel loggerLevel;
};

} /* namespace DSP */


#endif /* VRLOGGER_H_ */
