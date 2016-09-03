/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <iostream>
#include <plugin/VRPlugin.h>
#include "log/VRLogger.h"

// special: include this only once in one .cpp file per plugin
#include <plugin/VRPluginVersion.h>

namespace MinVR {

/** sbdlog Plugin for MinVR System.  Relies on c++11 for implementation.
 */
class VRspdlogPlugin : public MinVR::VRPlugin {
public:
	PLUGIN_API VRspdlogPlugin() {
	}

	PLUGIN_API virtual ~VRspdlogPlugin() {
	}

	PLUGIN_API void registerWithMinVR(VRMainInterface *vrMain)
	{
		VRLogger::get().getStream(VRLog::Info) << "Registering spdlog plugin." << VRLog::endl;
	}

	PLUGIN_API void unregisterWithMinVR(VRMainInterface *vrMain)
	{
	}

};

}

extern "C"
{
	PLUGIN_API MinVR::VRPlugin* createPlugin() {
		return new MinVR::VRspdlogPlugin();
	}
}

