from ctypes import cdll
import ctypes
import xml.etree.ElementTree

libName = 'MinVR_Python'

from sys import platform as _platform
if _platform == "linux" or _platform == "linux2":
    libFilePath = 'lib/lib' + libName + '.so'
elif _platform == "darwin":
    libFilePath = 'lib/lib' + libName + '.dylib'
elif _platform == "win32":
    libFilePath = 'bin/' + libName + '.dll'

e = xml.etree.ElementTree.parse('desktop.xml').getroot()
pluginpath = e.findall('PluginPath')[0].text
print pluginpath

lib = cdll.LoadLibrary(pluginpath + '/' + libName + '/' + libFilePath)

eventcallback_type = ctypes.CFUNCTYPE(None, ctypes.c_char_p)
rendercallback_type = ctypes.CFUNCTYPE(None)

class VRMain(object):
	def __init__(self, config):
		self.obj = lib.VRMain_init(config)
		self.eventHandlers = []
		self.renderHandlers = []
		self.eventCB = self.getEventCallbackFunc()
		self.eventhandler = lib.VRMain_registerEventCallback(self.obj, self.eventCB)
		self.renderCB = self.getRenderCallbackFunc()
		self.renderhandler = lib.VRMain_registerRenderCallback(self.obj, self.renderCB)
	def shutdown(self):
		lib.VRMain_shutdown(self.obj, self.eventhandler, self.renderhandler)
	def addEventHandler(self, handler):
		self.eventHandlers.append(handler)
	def addRenderHandler(self, handler):
		self.renderHandlers.append(handler)
	def getEventCallbackFunc(self):
		def func(eventName):
			self.handleEvent(eventName)
		return eventcallback_type(func)
	def handleEvent(self, eventName):
		eName = eventName
		for handler in self.eventHandlers:
			handler.onVREvent(eName)
	def getRenderCallbackFunc(self):
		def func():
			self.handleRender()
		return rendercallback_type(func)
	def handleRender(self):
		for handler in self.renderHandlers:
			handler.onVRRenderScene()
	def mainloop(self):
		lib.VRMain_mainloop(self.obj)

class VREventHandler(object):
	def __init__(self):
		pass
	def onVREvent(self, eventName):
		print "Event"

class VRRenderHandler(object):
	def __init__(self):
		pass
	def onVRRenderScene(self):
		print "Rendering Scene"
	def onVRRenderContext(self):
		print "Rendering Context"








