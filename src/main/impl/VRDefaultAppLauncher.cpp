/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <main/impl/VRDefaultAppLauncher.h>

namespace MinVR {

VRDefaultAppLauncher::VRDefaultAppLauncher(const std::string& cmd) : cmd(cmd) {
	// TODO Auto-generated constructor stub

}

VRDefaultAppLauncher::~VRDefaultAppLauncher() {
	// TODO Auto-generated destructor stub
}

std::string VRDefaultAppLauncher::generateCommandLine(
		const std::string& minvrData) const  {
	return cmd + " MINVR_DATA=" + minvrData;
}

} /* namespace MinVR */

