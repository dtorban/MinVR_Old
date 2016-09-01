/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <log/VRLogManager.h>
#include "impl/VRBasicLogger.h"

namespace MinVR {

VRLogManager* VRLogManager::instance = NULL;

VRLogManager::VRLogManager() {
	currentLogger = new VRBasicLogger();
	loggers["Default"] = currentLogger;
}

VRLogManager::~VRLogManager() {
	for (std::map<std::string, VRLogger*>::iterator it = loggers.begin(); it != loggers.end(); it++) {
		delete it->second;
	}
}

void VRLogManager::set(const std::string& name, VRLogger* logger) {
	std::map<std::string, VRLogger*>::iterator it = loggers.find(name);

	if (it != loggers.end()) {
		delete it->second;
	}

	loggers[name] = logger;

	if (name == "Default") {
		currentLogger = logger;
	}
}

void VRLogManager::set(VRLogger* logger) {
	set("Default", logger);
}

VRLogger& VRLogManager::get(const std::string& name) {
	if (name == "Default") {
		return get();
	}

	return *(loggers[name]);
}

VRLogger& VRLogManager::get() {
	return *(currentLogger);
}

} /* namespace MinVR */
