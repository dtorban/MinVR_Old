/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRTYPEDDISPLAYFACTORY_H_
#define VRTYPEDDISPLAYFACTORY_H_

#include "display/factories/VRDisplayFactory.h"

namespace MinVR {

class VRTypedDisplayFactory : public VRDisplayFactory {
public:
	VRTypedDisplayFactory();
	virtual ~VRTypedDisplayFactory();

	VRDisplayNode* create(VRDataIndex& config, const std::string nameSpace, VRDisplayFactory& baseFactory);

protected:
	virtual VRDisplayNode* create(VRDataIndex& config, const std::string nameSpace, std::string type, VRDisplayFactory& baseFactory) = 0;
};

} /* namespace MinVR */

#endif /* VRTYPEDDISPLAYFACTORY_H_ */
