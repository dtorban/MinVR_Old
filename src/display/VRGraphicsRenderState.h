/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRRENDERHELPER_H_
#define VRRENDERHELPER_H_

#include <config/VRDataIndex.h>
#include <math/VRMath.h>

namespace MinVR {

class VRGraphicsRenderState {
public:
	VRGraphicsRenderState(VRDataIndex& index);
	virtual ~VRGraphicsRenderState();

	bool firstRenderCall();
	bool isConsole();
	VRMatrix4 getProjectionMatrix();
	VRMatrix4 getViewMatrix();
	int getWindowX();
	int getWindowY();
	int getWindowWidth();
	int getWindowHeight();
	int getSharedContextGroupID();
	int getWindowID();

	template<typename T>
	T getValue(const std::string& key, const T& defaultValue, const std::string& nameSpace = "/") {
		if (index.exists(key, nameSpace)) {
			return index.getValue(key, nameSpace);
		}

		return defaultValue;
	}

	VRDataIndex& getDataIndex() { return index; }

private:
	VRDataIndex& index;
};

} /* namespace MinVR */

#endif /* VRRENDERHELPER_H_ */
