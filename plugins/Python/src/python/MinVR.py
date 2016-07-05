from ctypes import cdll
import ctypes
lib = cdll.LoadLibrary('../../../../build/Release/plugins/MinVR_Python/lib/libMinVR_Python.so')

eventcallback_type = ctypes.CFUNCTYPE(None, ctypes.c_char_p)

class VRMain(object):
	def __init__(self, config):
		self.obj = lib.VRMain_init(config)
		self.eventHandlers = []
		self.eventCB = self.getEventCallbackFunc()
		self.handler = lib.VRMain_registerEventCallback(self.obj, self.eventCB)
	def addEventHandler(handler):
		self.eventHandlers.append(handler)
	def getEventCallbackFunc(self):
		def func(eventName):
			self.handleEvent(eventName)
		return eventcallback_type(func)
	def handleEvent(self, eventName):
		print eventName

vrmain = VRMain("desktop2-local.xml")


