#include <string>
#include <iostream>
#include <math.h>

#include <display/VRConsoleNode.h>
#include <main/VRMain.h>
#include <main/VREventHandler.h>
#include <main/VRRenderHandler.h>
#include <math/VRMath.h>
#include <main/impl/VRDefaultAppLauncher.h>


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

using namespace MinVR;

struct Bone {
	VRVector3 start;
	VRVector3 end;

	void draw() {
		glBegin(GL_LINES);
		glVertex3f(start.x, start.y, start.z);
		glVertex3f(end.x, end.y, end.z);
		glEnd();
	}
};

struct Hand {
	Bone bones[20];

	void draw() {
		for (int f = 0; f < 20; f++) {
			bones[f].draw();
		}
	}
};

class MyVRApp : public VREventHandler, public VRRenderHandler {
public:
  MyVRApp(int argc, char** argv) : _vrMain(NULL), _quit(false), showHands(false), handColor(1.0, 1.0, 0.0), talking(0) {
		_vrMain = new VRMain();

		std::string configFile = argv[1];
		_vrMain->initialize(argc, argv, configFile);

		//_vrMain->initialize(argc, argv);
      	_vrMain->addEventHandler(this);
		_vrMain->addRenderHandler(this);
        _horizAngle = 0.0;
        _vertAngle = 0.0;
		_radius =  15.0;
        _incAngle = -0.1f;
	}

	virtual ~MyVRApp() {
		_vrMain->shutdown();
		delete _vrMain;
	}

	void handleHand(std::string handName, VRDataIndex *eventData, int handIndex) {
		VRContainer fingers = eventData->getValue(handName + "/Fingers");
		int boneNum = 0;
		for (VRContainer::iterator it = fingers.begin(); it != fingers.end(); it++) {
			VRContainer bones = eventData->getValue(handName + "/Fingers/" + *it + "/Bones");
			for (VRContainer::iterator boneIt = bones.begin(); boneIt != bones.end(); boneIt++) {
				hands[handIndex].bones[boneNum].start = eventData->getValue(handName + "/Fingers/" + *it + "/Bones/" + *boneIt + "/Start");
				hands[handIndex].bones[boneNum].end = eventData->getValue(handName + "/Fingers/" + *it + "/Bones/" + *boneIt + "/End");
				boneNum++;
			}
		}

	}

	// Callback for event handling, inherited from VREventHandler
	virtual void onVREvent(const std::string &eventName, VRDataIndex *eventData) {
		//std::cout << "Event: " << eventName << std::endl;
		if (eventName == "/KbdEsc_Down") {
			_quit = true;
		}
		else if (eventName == "/KbdSpace_Down") {
			showHands = true;
		}
    	else if (eventName == "/Leap") {
    		if (eventData->exists(eventName + "/Hands/Left")) {
    			handleHand(eventName + "/Hands/Left", eventData, 0);
    		}
    		if (eventData->exists(eventName + "/Hands/Right")) {
    			handleHand(eventName + "/Hands/Right", eventData, 1);
    		}
    	}
    	else if (eventName == "/Words") {
    		std::string words = eventData->getValue("/Words");
    		if (talking > 0) {
    			if (words.find("hands") != std::string::npos) {
    			     showHands = true;
    			     talking = 0;
    			}
    			else if (words.find("red") != std::string::npos && words.find("read") != std::string::npos) {
    				handColor.x = 1.0;
    				handColor.y = 0.0;
    				handColor.z = 0.0;
    				talking = 0;
    			}
    			else if (words.find("blue") != std::string::npos) {
    				handColor.x = 0.0;
    				handColor.y = 0.0;
    				handColor.z = 1.0;
    				talking = 0;
    			}
    			else if (words.find("gre") != std::string::npos) {
    				handColor.x = 0.0;
    				handColor.y = 1.0;
    				handColor.z = 0.0;
    				talking = 0;
    			}
    			else if (words.find("yel") != std::string::npos) {
    				handColor.x = 1.0;
    				handColor.y = 1.0;
    				handColor.z = 0.0;
    				talking = 0;
    			}

    			if (words.find("ex") != std::string::npos) {
    				_quit = true;
    			}
    		}
    		else {
        		if (words.find("prog") != std::string::npos) {
        			talking = 255.0*2.0;
        		}
    		}

    	}

	}

  
    virtual void onVRRenderContext(VRDataIndex *renderState, VRDisplayNode *callingNode) {
        if (!renderState->exists("IsConsole", "/")) {
        }
        if (talking > 0) {
            talking--;
        }
    }

	int count;
  
	// Callback for rendering, inherited from VRRenderHandler
	virtual void onVRRenderScene(VRDataIndex *renderState, VRDisplayNode *callingNode) {
		if (renderState->exists("IsConsole", "/")) {
			VRConsoleNode *console = dynamic_cast<VRConsoleNode*>(callingNode);
			console->println("Console output...");
		}
		else {

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glClearDepth(1.0f);
			count++;
			glClearColor((1.0*talking)/(255.0*2.0)/2.0, (1.0*talking)/(255.0*2.0)/2.0, (1.0*talking)/(255.0*2.0)/2.0, 1.f);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			//glClear(GL_DEPTH_BUFFER_BIT);

            if (renderState->exists("ProjectionMatrix", "/")) {
                // This is the typical case where the MinVR DisplayGraph contains
                // an OffAxisProjectionNode or similar node, which sets the
                // ProjectionMatrix and ViewMatrix based on head tracking data.
            
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
			    VRMatrix4 P = renderState->getValue("ProjectionMatrix", "/");
			    glLoadMatrixd(P.m);
              
			    glMatrixMode(GL_MODELVIEW);
			    glLoadIdentity();
              
                // In the original adaptee.cpp program, keyboard and mouse commands
                // were used to adjust the camera.  Now that we are creating a VR
                // program, we want the camera (Projection and View matrices) to
                // be controlled by head tracking.  So, we switch to having the
                // keyboard and mouse control the Model matrix.  Guideline: In VR,
                // put all of the "scene navigation" into the Model matrix and
                // leave the Projection and View matrices for head tracking.
                VRMatrix4 M = VRMatrix4::translation(VRVector3(0.0, 0.0, -_radius)) *
                              VRMatrix4::rotationX(_vertAngle) *
                              VRMatrix4::rotationY(_horizAngle);
              
			    VRMatrix4 V = renderState->getValue("ViewMatrix", "/");
			    glLoadMatrixd((V*M).m);
            }
            else {
                // If the DisplayGraph does not contain a node that sets the
                // ProjectionMatrix and ViewMatrix, then we must be in a non-headtracked
                // simple desktop mode.  We can just set the projection and modelview
                // matrices the same way as in adaptee.cpp.
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(1.6*45.f, 1.f, 0.1f, 100.0f);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
              
                double cameraPos[3];
                cameraPos[0] = _radius * cos(_horizAngle) * cos(_vertAngle);
                cameraPos[1] = -_radius * sin(_vertAngle);
                cameraPos[2] = _radius * sin(_horizAngle) * cos(_vertAngle);
    
                double cameraAim[3];
                cameraAim[0] = cos(_horizAngle) * sin(_vertAngle);
                cameraAim[1] = cos(_vertAngle);
                cameraAim[2] = sin(_horizAngle) * sin(_vertAngle);
              
                double targetPos[3] = {0.0f, 0.0f, 0.0f};
                gluLookAt (cameraPos[0], cameraPos[1], cameraPos[2],
                           targetPos[0], targetPos[1], targetPos[2],
                           cameraAim[0], cameraAim[1], cameraAim[2]);
            }


            if (showHands) {
            	glColor3f(handColor.x, handColor.y, handColor.z);
                for (int f = 0; f < 2; f++) {
              	  hands[f].draw();
                }
            }

		}
	}

	void run() {
		while (!_quit) {
			_vrMain->mainloop();
		}
	}

protected:
	VRMain *_vrMain;
	bool _quit;
    double _horizAngle, _vertAngle, _radius, _incAngle;
    Hand hands[2];
    bool showHands;
    VRVector3 handColor;
    int talking;
};



int main(int argc, char **argv) {

    MyVRApp app(argc, argv);
  	app.run();

	exit(0);
}

