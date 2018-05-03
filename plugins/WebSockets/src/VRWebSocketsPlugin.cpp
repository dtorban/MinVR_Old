/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <plugin/VRPlugin.h>


// special: include this only once in one .cpp file per plugin
#include <plugin/VRPluginVersion.h>
#include <main/VRFactory.h>

#include "libwebsockets.h"
#include "VRWebSocketDevice.h"

namespace MinVR {



class VRWebSocketsPlugin : public VRPlugin {
public:
	PLUGIN_API VRWebSocketsPlugin() {
      //std::cout << "GlfwPlugin created." << std::endl;
	}

	PLUGIN_API virtual ~VRWebSocketsPlugin() {
      //std::cout << "GlfwPlugin destroyed." << std::endl;
	}
	PLUGIN_API void registerWithMinVR(VRMainInterface *vrMain)
	{
		//server = new VRWebSocketsServer(8081);
		//vrMain->addInputDevice(new VRWebSocketsInputDevice(*server));
		vrMain->getFactory()->registerItemType<VRInputDevice, VRWebSocketDevice>("VRWebSocketDevice");
		//while(true) {
		//	server->service();
		//}
	}

	PLUGIN_API void unregisterWithMinVR(VRMainInterface *vrMain)
	{
		delete server;
	}

private:
	VRWebSocketsServer* server;
};

} // end namespace


extern "C"
{
	PLUGIN_API MinVR::VRPlugin* createPlugin() {
		return new MinVR::VRWebSocketsPlugin();
	}
}

