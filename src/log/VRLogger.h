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

	virtual void log(level::VRLogLevel lvl, const std::string& msg) = 0;
	virtual void setLevel(level::VRLogLevel lvl) = 0;

	void trace(const std::string& msg) { log(level::trace, msg); }
	void debug(const std::string& msg) { log(level::debug, msg); }
	void info(const std::string& msg) { log(level::info, msg); }
	void warn(const std::string& msg) { log(level::warn, msg); }
	void error(const std::string& msg) { log(level::err, msg); }
	void critical(const std::string& msg) { log(level::critical, msg); }

	static void set(const std::string& key, VRLogger* logger) {
		if (key == "Default") {
			currentLogger.setLogger(logger);
		}

		std::map<std::string, VRLogger*>::iterator it = loggerMap.loggers.find(key);

		if (it != loggerMap.loggers.end()) {
			delete it->second;
		}

		loggerMap.loggers[key] = logger;
	}

	static void set(VRLogger* logger) {
		currentLogger.setLogger(logger);
	}

	static VRLogger* get(const std::string& key) {
		if (key == "Default") {
			return get();
		}

		return loggerMap.loggers[key];
	}

	static VRLogger* get() {
		return currentLogger.getLogger();
	}

	private:

		class LoggerPtr {
		public:
			LoggerPtr(VRLogger* logger) : logger(logger) {
			}

			~LoggerPtr() {
				if (logger) {
					delete logger;
				}
				logger = NULL;
			}

			VRLogger* getLogger() const {
				return logger;
			}

			void setLogger(VRLogger* logger) {
				if (this->logger) {
					delete this->logger;
				}

				this->logger = logger;
			}
		private:
			VRLogger* logger;
	};

	struct VRLoggerMap {
		~VRLoggerMap() {
			for (std::map<std::string, VRLogger*>::iterator it = loggers.begin(); it != loggers.end(); it++) {
				delete it->second;
			}
		}

		std::map<std::string, VRLogger*> loggers;
	};

	static LoggerPtr currentLogger;
	static VRLoggerMap loggerMap;
};


} /* namespace DSP */

#endif /* VRLOGGER_H_ */
