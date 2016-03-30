/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/factories/VRCompositeDisplayFactory.h>

namespace MinVR {

VRCompositeDisplayFactory::VRCompositeDisplayFactory() {
}

VRCompositeDisplayFactory::~VRCompositeDisplayFactory() {
	for (int f = 0; f < m_displayFactories.size(); f++)
	{
		delete m_displayFactories[f];
	}
}

VRDisplayNode* VRCompositeDisplayFactory::create(VRDataIndex& config,
		const std::string nameSpace, VRDisplayFactory& baseFactory) {
	for (int f = 0; f < m_displayFactories.size(); f++)
	{
		VRDisplayNode* display = m_displayFactories[f]->create(config, nameSpace, baseFactory);
		if (display != NULL)
		{
			return display;
		}
	}

	return NULL;
}

void VRCompositeDisplayFactory::addFactory(VRDisplayFactory* factory) {
	m_displayFactories.push_back(factory);
}

} /* namespace MinVR */
