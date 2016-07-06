#!/usr/bin/python

import sys, os, inspect

fileDir = os.path.dirname(os.path.abspath(inspect.getsourcefile(lambda:0)))
os.chdir(fileDir)

sys.path.append("../../plugins/Python/src/python")

from MinVR import *
from OpenGL.GL import *

loop = True
rot = 0.0

class App(VREventHandler, VRRenderHandler):
	def onVREvent(self, eventName):
		print eventName
		if eventName == "/KbdEsc_Down":
			global loop
			loop = False
		elif eventName == "/KbdRight_Down" or eventName == "/KbdRight_Repeat":
			global rot
			rot += 0.1
		elif eventName == "/KbdLeft_Down" or eventName == "/KbdLeft_Repeat":
			global rot
			rot -= 0.1

	def onVRRenderScene(self, renderState):
		glClear(GL_COLOR_BUFFER_BIT)
		width = renderState.getValue("WindowWidth","/")
		height = renderState.getValue("WindowHeight","/")
		ratio = width / height;
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.0, 1.0, 1.0, -1.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(rot*50.0, 0.0, 0.0, 1.0);
		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex3f(-0.6, -0.4, 0.0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.6, -0.4, 0.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.60, 0.0);
		glEnd();

app = App()
vrmain = VRMain("desktop.xml")
vrmain.addRenderHandler(app)
vrmain.addEventHandler(app)
while loop:
	vrmain.mainloop()
vrmain.shutdown()

