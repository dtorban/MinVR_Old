/*
 * VRCallbackNode.h
 *
 *  Created on: Sep 27, 2018
 *      Author: dan
 */

#ifndef VRCALLBACKNODE_H_
#define VRCALLBACKNODE_H_

#include "VRDisplayNode.h"
#include <main/VRFactory.h>

namespace MinVR {

class VRCallbackNode : public VRDisplayNode {
public:
	VRCallbackNode(const std::string &name, const std::string &callbackName, VRMainInterface *vrMain);
	virtual ~VRCallbackNode();

	void render(VRDataIndex *renderState, VRRenderHandler* renderHandler);

	virtual std::string getType() const { return "VRCallbackNode"; }
	static VRDisplayNode* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);

private:
	std::string callbackName;
	VRMainInterface *vrMain;
};

} /* namespace MinVR */

#endif /* CMAKE_FRAMEWORK_EXTERNAL_MINVR_SRC_SRC_DISPLAY_VRCALLBACKNODE_H_ */
