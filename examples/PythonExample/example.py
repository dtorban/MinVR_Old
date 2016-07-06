#!/usr/bin/python

import sys
sys.path.append("../../plugins/Python/src/python")

from MinVR import *
from OpenGL.GL import *

loop = True

class AppEventHandler(VREventHandler):
	def onVREvent(self, eventName):
		print eventName
		if eventName == "/KbdEsc_Down":
			global loop
			loop = False

class AppRenderHandler(VRRenderHandler):
	def onVRRenderScene(self):
		#glClearColor(1, 1, 1, 1)
		glClear(GL_COLOR_BUFFER_BIT)
		#float ratio;
		#int width, height;
		#glfwGetFramebufferSize(window, &width, &height);
		#ratio = width / (float) height;
		glViewport(0, 0, 100, 100);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		#glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		#glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex3f(-0.6, -0.4, 0.0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.6, -0.4, 0.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.60, 0.0);
		glEnd();



vrmain = VRMain("desktop.xml")
vrmain.addRenderHandler(AppRenderHandler())
vrmain.addEventHandler(AppEventHandler())
while loop:
	vrmain.mainloop()
vrmain.shutdown()

