/*
 * AsyncHandler.h
 *
 *  Created on: Sep 27, 2018
 *      Author: dan
 */

#ifndef VRCALLBACKHANDLER_H_
#define VRCALLBACKHANDLER_H_

#include <config/VRDataIndex.h>
#include <string>

namespace MinVR {

class VRCallbackHandler {
public:
	virtual ~VRCallbackHandler() {}

	virtual void onRun(const std::string& callbackName, const VRDataIndex &stateData) = 0;
};

} /* namespace MinVR */

#endif /* VRCALLBACKHANDLER_H_ */
