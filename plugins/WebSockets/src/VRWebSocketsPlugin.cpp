/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <iostream>
#include <plugin/VRPlugin.h>
#include "VRWebSocketsNode.h"
#include "main/VRFactory.h"
#include "VRWebSocketsServer.h"
#include "VRWebSocketsInputDevice.h"

// special: include this only once in one .cpp file per plugin
#include <plugin/VRPluginVersion.h>


namespace MinVR {

class VRWebSocketsPlugin : public VRPlugin {
public:
	PLUGIN_API VRWebSocketsPlugin() {
      //std::cout << "WebSocketsPlugin created." << std::endl;
	}
	PLUGIN_API virtual ~VRWebSocketsPlugin() {
      //std::cout << "WebSocketsPlugin destroyed." << std::endl;
	}

	PLUGIN_API void registerWithMinVR(VRMainInterface *vrMain)
	{
		//vrMain->getFactory()->registerItemType<VRDisplayNode, VRWebSocketsNode>("VRWebSocketsNode");
		server = new VRWebSocketsServer(8081);
		vrMain->addInputDevice(new VRWebSocketsInputDevice(*server));
	}

	PLUGIN_API void unregisterWithMinVR(VRMainInterface *vrMain)
	{
      //std::cout << "Unregistering GlfwPlugin." << std::endl;
		// TODO

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

