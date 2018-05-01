/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

 // This avoids APIENTRY macro redefinition warnings when including glfw3.h

#include <iostream>
#include <plugin/VRPlugin.h>


// special: include this only once in one .cpp file per plugin
#include <plugin/VRPluginVersion.h>

#include "HD/VROpenHapticsNodeHD.h"

#include <main/VRFactory.h>

namespace MinVR {



class VROpenHapticsPlugin : public VRPlugin {
public:
	PLUGIN_API VROpenHapticsPlugin() {
      //std::cout << "GlfwPlugin created." << std::endl;
	}

	PLUGIN_API virtual ~VROpenHapticsPlugin() {
      //std::cout << "GlfwPlugin destroyed." << std::endl;
	}

	PLUGIN_API void registerWithMinVR(VRMainInterface *vrMain)
	{
      std::cout << "Registering VROpenHapticsPlugin." << std::endl;
		vrMain->getFactory()->registerItemType<VRDisplayNode, VROpenHapticsNodeHD>("VROpenHapticsNodeHD");
	}

	PLUGIN_API void unregisterWithMinVR(VRMainInterface *vrMain)
	{
      //std::cout << "Unregistering GlfwPlugin." << std::endl;
		// TODO
	}
};

} // end namespace


extern "C"
{
	PLUGIN_API MinVR::VRPlugin* createPlugin() {
		return new MinVR::VROpenHapticsPlugin();
	}
}

