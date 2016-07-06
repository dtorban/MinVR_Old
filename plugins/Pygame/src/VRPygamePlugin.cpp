///*
// * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0.
// * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
// *
// * Code author(s):
// * 		Dan Orban (dtorban)
// */
//
#include <plugin/VRPlugin.h>

// special: include this only once in one .cpp file per plugin
#include <plugin/VRPluginVersion.h>


namespace MinVR {

class VRPygamePlugin : public VRPlugin {
public:
	PLUGIN_API VRPygamePlugin() {
	}
	PLUGIN_API virtual ~VRPygamePlugin() {
	}
	PLUGIN_API void registerWithMinVR(VRMainInterface *vrMain) {
		std::cout << "Register pygame plugin." << std::endl;
		vrMain->getConfig()->addData("/PythonPlugins/MinVR_Pygame", "MinVR_Pygame");
	}
	PLUGIN_API void unregisterWithMinVR(VRMainInterface *vrMain) {
	}
};

} // end namespace

extern "C"
{
	PLUGIN_API MinVR::VRPlugin* createPlugin() {
		return new MinVR::VRPygamePlugin();
	}
}

