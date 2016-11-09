/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <iostream>
#include <plugin/VRPlugin.h>
#include "main/VRFactory.h"
#include "VRSphinxDevice.h"

// special: include this only once in one .cpp file per plugin
#include <plugin/VRPluginVersion.h>

namespace MinVR {

/** Threading Plugin for MinVR System.  Relies on c++11 threading or Boost for implementation.
 */
class VRSphinxPlugin : public MinVR::VRPlugin {
public:
	PLUGIN_API VRSphinxPlugin() {
	}

	PLUGIN_API virtual ~VRSphinxPlugin() {
	}

	PLUGIN_API void registerWithMinVR(VRMainInterface *vrMain)
	{
		// Register VRThreadGroupNode
		std::cout << "Registering VRSphinxPlugin." << std::endl;
		vrMain->getFactory()->registerItemType<VRInputDevice, VRSphinxDevice>("VRSphinxDevice");
	}

	PLUGIN_API void unregisterWithMinVR(VRMainInterface *vrMain)
	{
	}

};

}

extern "C"
{
	PLUGIN_API MinVR::VRPlugin* createPlugin() {
		return new MinVR::VRSphinxPlugin();
	}
}



