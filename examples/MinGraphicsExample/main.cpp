#include <iostream>
#include <cmath>

#if defined(WIN32)
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#include <gl/GLU.h>
#elif defined(__APPLE__)
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#endif

// MinVR header (needed for VRGraphicsApp)
#include <api/MinVR.h>

using namespace MinVR;

/** MyVRApp changes the clear color as frames progress. */
class MyVRApp : public VRGraphicsApp {
public:
	MyVRApp(int argc, char** argv, const std::string& configFile) : VRGraphicsApp(argc, argv, configFile) {}

	void onVREvent(const std::string &eventName, VRDataIndex *eventData) {
		// Set time since application began
		if (eventName == "/Time") {
			time = eventData->getValue(eventName);
			return;
		}

		std::cout << eventName << std::endl;

		// Quit if the escape button is pressed
		if (eventName == "/KbdEsc_Down") {
			shutdown();
		}
	}

	void onVRRenderGraphicsContext(VRGraphicsState& renderState) {
		// Print out when the window was opened and closed
		if (renderState.isInitialRenderCall()) {
			std::cout << "Window opened." << std::endl;
		}

		if (!isRunning()) {
			std::cout << "Window closed." << std::endl;
		}
	}

	void onVRRenderGraphics(VRGraphicsState& renderState) {
		// Show gradient of red color over four seconds then restart
		float red = std::fmod(time/4.0,1.0);
		glClearColor(red, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

private:
	double time;
};

int main(int argc, char **argv) {
	// The second argument is the config file path for this example
	MyVRApp app(argc, argv, argv[1]);
	app.run();
	return 0;
}
