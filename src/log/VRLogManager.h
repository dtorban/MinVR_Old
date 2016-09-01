/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRLOGMANAGER_H_
#define VRLOGMANAGER_H_

#include <string>
#include <map>
#include <iostream>

namespace MinVR {

class VRLogger;

class VRLogManager {
public:
	VRLogManager();
	virtual ~VRLogManager();

	void set(const std::string& name, VRLogger* logger);
	void set(VRLogger* logger);
	VRLogger& get(const std::string& name);
	VRLogger& get();

	static VRLogManager* getInstance() {
		if (!instance) {
			std::cerr << "Log Manager has not been initialized by VRMain." << std::endl;
		}

		return instance;
	}

	static void setInstance(VRLogManager* inst) {
		instance = inst;
	}

private:
	VRLogger* currentLogger;
	std::map<std::string, VRLogger*> loggers;

	static VRLogManager* instance;
};

} /* namespace MinVR */

#endif /* VRLOGMANAGER_H_ */
