/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRDEFAULTAPPLANCHER_H_
#define VRDEFAULTAPPLANCHER_H_

#include <main/VRAppLauncher.h>

namespace MinVR {

class VRDefaultAppLauncher : public VRAppLauncher {
public:
	VRDefaultAppLauncher(const std::string& cmd);
	virtual ~VRDefaultAppLauncher();

	std::string generateCommandLine(const std::string& minvrData) const;

private:
	std::string cmd;
};

} /* namespace MinVR */

#endif /* VRDEFAULTAPPLANCHER_H_ */
