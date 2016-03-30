/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRDISPLAYFACTORY_H_
#define VRDISPLAYFACTORY_H_

#include "display/VRDisplayNode.h"
#include "config/VRDataIndex.h"

namespace MinVR {

class VRDisplayFactory {
public:
	virtual ~VRDisplayFactory() {}

	virtual VRDisplayNode* create(VRDataIndex& config, const std::string nameSpace, VRDisplayFactory& baseFactory) = 0;

	static void createChildren(VRDisplayNode* display, VRDisplayFactory& baseFactory, VRDataIndex& config, const std::string nameSpace);
};

inline void VRDisplayFactory::createChildren(VRDisplayNode* display, VRDisplayFactory& baseFactory, VRDataIndex& config, const std::string nameSpace)
{
	if (display)
	{
		VRContainer item = config.getValue(nameSpace);

		std::cout << nameSpace << std::endl;
		for (VRContainer::iterator f = item.begin(); f != item.end(); f++)
		{
			if (config.getType(*f) == VRCORETYPE_CONTAINER)
			{
				std::cout << *f << std::endl;
				VRDisplayNode* subDisplay = baseFactory.create(config, *f, baseFactory);
				if (subDisplay)
				{
					display->addChild(subDisplay);
				}
			}
		}
	}
}

} /* namespace MinVR */

#endif /* VRDISPLAYFACTORY_H_ */
