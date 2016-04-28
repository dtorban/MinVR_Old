/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <iostream>
#include <plugin/VRPlugin.h>
//#include "VROpenGLGraphicsToolkit.h"
#include "VRCpp11ThreadToolkit.h"

// special: include this only once in one .cpp file per plugin
#include <plugin/VRPluginVersion.h>


namespace MinVR {

class VRCpp11Plugin : public VRPlugin {
public:
	PLUGIN_API VRCpp11Plugin() {
	}
	PLUGIN_API virtual ~VRCpp11Plugin() {
	}

	PLUGIN_API void registerWithMinVR(VRMainInterface *vrMain)
	{
		//vrMain->getFactory()->addSubFactory(new VROpenGLGraphicsToolkitFactory());
	}

	PLUGIN_API void unregisterWithMinVR(VRMainInterface *vrMain)
	{
	}

private:
	VRCpp11ThreadToolkit tk;
};

} // end namespace

extern "C"
{
	PLUGIN_API MinVR::VRPlugin* createPlugin() {
		return new MinVR::VRCpp11Plugin();
	}
}

