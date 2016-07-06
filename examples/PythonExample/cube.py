#!/usr/bin/python

import sys, os, inspect

fileDir = os.path.dirname(os.path.abspath(inspect.getsourcefile(lambda:0)))
os.chdir(fileDir)

sys.path.append("../../plugins/Python/src/python")

from MinVR import *
from OpenGL.GL import *
from OpenGL.GLU import *

loop = True
rot = 0.0

class AppEventHandler(VREventHandler):
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


verticies = (
    (1, -1, -1),
    (1, 1, -1),
    (-1, 1, -1),
    (-1, -1, -1),
    (1, -1, 1),
    (1, 1, 1),
    (-1, -1, 1),
    (-1, 1, 1)
    )

edges = (
    (0,1),
    (0,3),
    (0,4),
    (2,1),
    (2,3),
    (2,7),
    (6,3),
    (6,4),
    (6,7),
    (5,1),
    (5,4),
    (5,7)
    )


def Cube():
    glBegin(GL_LINES)
    for edge in edges:
        for vertex in edge:
            glVertex3fv(verticies[vertex])
    glEnd()

a = 3

class AppRenderHandler(VRRenderHandler):
	def onVRRenderScene(self, renderState):


		#glClearColor(1, 1, 1, 1)
		glClear(GL_COLOR_BUFFER_BIT)
		#float ratio;
		#int width, height;
		width = renderState.getValue("WindowWidth","/")
		height = renderState.getValue("WindowHeight","/")
		#glfwGetFramebufferSize(window, &width, &height);
		ratio = width / height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45, ((1.0*width)/(1.0*height)), 0.1, 50.0)
		glBegin(GL_TRIANGLES);
		glColor3f(1, 0, 0);
		glVertex3f(-0.6, -0.4, 0.0);
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.6, -0.4, 0.0);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.60, 0.0);
		glEnd();

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                glMatrixMode(GL_MODELVIEW);
                glTranslatef(0.0,0.0, -5)
                glRotatef(a/3, a, a/3, a/3)

                Cube()


vrmain = VRMain("desktop.xml")
vrmain.addRenderHandler(AppRenderHandler())
vrmain.addEventHandler(AppEventHandler())
glTranslatef(0.0,0.0, -5)


while loop:
        a += 3
	vrmain.mainloop()
vrmain.shutdown()

