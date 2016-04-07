#include "VRMain.h"
#include <stdio.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include "net/VRNetClient.h"
#include "net/VRNetServer.h"
#include "display/nodes/console/VRConsoleDisplay.h"
#include "display/nodes/graphics/stereo/VRStereoNode.h"
#include "display/nodes/graphics/structure/VRViewportNode.h"
#include "display/nodes/graphics/camera/VROffAxisCameraNode.h"

void emptyEventCallbackMVR(const std::string &eventName, VRDataIndex *dataIndex) {}
void emptyRenderCallbackMVR(VRDataIndex* index) {}
void emptyRenderSwapMVR() {}

VRMain::VRMain() : initialized(false),_vrNet(NULL), _display(NULL)
{
	  registerEventCallback(&emptyEventCallbackMVR);
	  registerRenderCallback(&emptyRenderCallbackMVR);
	  registerSwapCallback(&emptyRenderSwapMVR);
	  _displayFactories = new MinVR::VRCompositeDisplayFactory();
	  _displayFactory = _displayFactories;
	  _displayFactory = new MinVR::VRViewportFactory(_displayFactory);
	  _displayFactories->addFactory(new MinVR::VRStereoFactory());
	  _displayFactories->addFactory(new MinVR::VRCameraDisplayFactory());
}


VRMain::~VRMain()
{
  delete _index;
  delete _vrNet;
  if (_display != NULL)
  {
	  delete _display;
  }
  delete _displayFactory;
}


void 
VRMain::initialize(const std::string settingsFile)
{
  initialize("", settingsFile);
}

void VRMain::initialize(VRDataIndex& index, const std::string valName)
{
	  initialize("", index, valName);
}

void
VRMain::initialize(const std::string processName, const std::string settingsFile) 
{
  _name = processName;
  _index = new VRDataIndex();
  _index->processXMLFile(settingsFile, "/");
  initialize();
}

void VRMain::initialize(const std::string processName, VRDataIndex& index, const std::string valName)
{
	_name = processName;
	_index = new VRDataIndex();
	_index->addSerializedValue(index.serialize(valName));
	initialize();
}

void VRMain::initialize()
{

	// Initialize processId to 0 which represents the main process
  _index->addData("/ProcessId", 0);

  // If there is a server tag in the configuration, use server logic
  if (_index->exists("Server", "/MVR"))
  {
	  int numProcesses = 1;
	  bool createServer = true;

	  int numClients = ((VRInt)_index->getValue("/MVR/Server/NumClients"));

	  if (_index->exists("NumProcesses", "/MVR/Server"))
	  {
		  numProcesses = _index->getValue("/MVR/Server/NumProcesses");
		  if (numProcesses > numClients) { numProcesses = numClients; }
	  }

	  if (_index->exists("CreateServer", "/MVR/Server"))
	  {
		  createServer  = ((VRInt)_index->getValue("/MVR/Server/CreateServer"));
	  }

	  int currentProcess = 0;

	  // If the configuration specifies that there are more than one process, processes
	  // will be forked to facilitate multiple processes.
#ifndef WIN32
	  while (currentProcess < numProcesses-1)
	  {
		  pid_t pid = fork();
		  if (pid == 0)
		  {
			  break;
		  }

		  currentProcess++;

		  if (currentProcess == 1)
		  {
			  sleep(3);
		  }

	  }
#else
	  // TODO: Add windows implementation of forking a process
#endif

	  // Set the current processId after forking
	  _index->addData("/ProcessId", currentProcess);

	  // If the application doesn't create the server, it assumes process is a network client
	  if (numClients > 1 || !createServer)
	  {
		  if (currentProcess == 0 && createServer)
		  {
			  // Create VR server which uses the same interface as a VR net client.
			  _vrNet = new VRNetServer((VRString)_index->getValue("/MVR/Server/Port"), numClients-1);
		  }
		  else
		  {
			  // Set Network Synchronization mode based on settings.  Note that
			  // these names are hard-wired in place. This is meant to be not
			  // configurable, though doubtless these names will change during
			  // development.
			  _vrNet = new VRNetClient((VRString)_index->getValue("/MVR/Server/Host"),
									   (VRString)_index->getValue("/MVR/Server/Port"));
		  }
	  }
  }

  _index->printStructure();
  
  // Load plugins from the plugin directory.  Once the available
  // plugins are identified, we can call their InputDeviceFactory and
  // DisplayDeviceFactory methods with the current settings.
  _pluginManager.addInterface(dynamic_cast<VRMain*>(this));
  if (_index->exists("MinVRDefaultPlugins", "/MVR/VRPlugins"))
  {
	  string buildType = "";
#ifdef MinVR_DEBUG
    buildType = "d";
#endif
	  std::vector<std::string> pluginNames = _index->getValue("Names", "/MVR/VRPlugins/MinVRDefaultPlugins");
	  for (int f = 0; f < pluginNames.size(); f++)
	  {
		 if (!_pluginManager.loadPlugin(std::string(PLUGINPATH) + "/" + pluginNames[f], pluginNames[f] + buildType))
		 {
			 _pluginManager.loadPlugin(std::string(PLUGININSTALLPATH) + "/" + pluginNames[f], pluginNames[f] + buildType);
		 }
	  }
  }

  // Create display
  _display = _displayFactory->create(*_index, "/MVR/VRDisplayDevices", *_displayFactory);

 // exit(0);

  if (_display == NULL)
  {
	  _display = new MinVR::VRConsoleDisplay();
  }

  // Create input devices
  for (int f = 0; f < _inputDeviceFactories.size(); f++)
  {
	  std::vector<VRInputDevice*> devices = _inputDeviceFactories[f]->create(*_index);
	  for (int i = 0; i < devices.size(); i++)
	  {
		  _inputDevices.push_back(devices[i]);
	  }
  }

  initialized = true;
}

void 
VRMain::synchronizeAndProcessEvents() {

  if (!initialized) {
    std::cerr << "VRMain not initialized." << std::endl;
    return;
  }
  
  VRDataQueue::serialData eventData;

  // std::vector<VREvent> inputEvents;
  // for (std::vector<InputDevice*>::iterator id = _inputDevices.begin();
  //     id != _inputDevices.end(); id++) {
  //  (*id)->appendNewInputEventsSinceLastCall(inputEvents);
  // }

  VRDataQueue eventsFromDevices;
  for (int f = 0; f < _inputDevices.size(); f++)
  {
	  _inputDevices[f]->appendNewInputEventsSinceLastCall(eventsFromDevices);
  }
  //eventsFromDevices.printQueue();

  // SYNCHRONIZATION POINT #1: When this function returns, we know
  // that all MinVR nodes have the same list of input events generated
  // since the last call to synchronizeAndProcessEvents(..).  So,
  // every node will process the same set of input events this frame.
  if (_vrNet != NULL) {
    eventData = _vrNet->syncEventDataAcrossAllNodes(eventsFromDevices.serialize());
  }
  else if (eventsFromDevices.notEmpty())
  {
	  eventData = eventsFromDevices.serialize();
  }

  VRDataQueue *events = new VRDataQueue(eventData);

  // After MinVR's internal state is updated above, now the events are
  // passed on to the application programmer's event callback
  // function.
  while (events->notEmpty()) {
    // Step 2: Unpack the next item from the queue.
    std::string event =
      _index->addSerializedValue( events->getSerializedObject() );

    // The only internal responding to events done by MinVR is here.
    // This hook is typicaly used for displayDevices to update thier
    // states based upon the most recent head position from tracking
    // data.  It might also be useful for some other things, e.g., a 3D
    // audio display might also want to update its settings based on the
    // current position of the user.

    //  for (std::vector<DisplayDevice*>::iterator dd = _displayDevices.begin();
    //       dd != _displayDevices.end(); dd++) {  
    //    (*dd)->handleUserInput(event, _index);
    //  }

    // 2.5 Invoke the user's callback on the new event
    //(*_eventCB)(event, _index);
    for (int f = 0; f < _eventHandlers.size(); f++)
    {
    	_eventHandlers[f]->handleEvent(event, _index);
    }

    // Get the next item from the queue.
    events->pop();

  }

  delete events;
}

void 
VRMain::renderEverywhere() {
	  if (!initialized) {
	    std::cerr << "VRMain not initialized." << std::endl;
	    return;
	  }
	  // for (std::vector<DisplayDevice*>::iterator dd = _displayDevices.begin();
	  //      dd != _displayDevices.end(); dd++) {
	  //   // Stereo displays will have two passes, one per eye; some
	  //   // displays might require more
	  //   for (int pass=0; pass < (*dd)->getNumRenderingPasses(); pass++) {

	  //     // The display handles setting up the correct drawing buffer and
	  //     // projection and view matrices
	  //     (*dd)->startRenderingPass(pass, curState);

	  //     // The application programmer fills in the
	  //     // renderCallbackFunction to draw his/her scene
	  (*_renderCB)(_index);

	  //     (*dd)->endRenderingPass(pass, curState);

	  //   }
	  // }

	  // SYNCHRONIZATION POINT #2: When this function returns we know that
	  // all nodes have finished rendering on all their attached display
	  // devices.  So, after this, we will be ready to "swap buffers",
	  // simultaneously displaying these new renderings on all nodes.
	  if (_vrNet != NULL) {
	    _vrNet->syncSwapBuffersAcrossAllNodes();
	  }

	  (*_swapCB)();
}

void
VRMain::renderEverywhere(MinVR::VRRenderer& renderer) {

  if (!initialized) {
    std::cerr << "VRMain not initialized." << std::endl;
    return;
  }

  _display->render(renderer);
  _display->waitForRenderToComplete();

  // SYNCHRONIZATION POINT #2: When this function returns we know that
  // all nodes have finished rendering on all their attached display
  // devices.  So, after this, we will be ready to "swap buffers",
  // simultaneously displaying these new renderings on all nodes.
  if (_vrNet != NULL) {
    _vrNet->syncSwapBuffersAcrossAllNodes();
  }

  _display->displayTheFinishedRendering();
}

// Adds the display factories for all plugins who use this interface
void VRMain::addVRDisplayFactory(MinVR::VRDisplayFactory* factory)
{
	_displayFactories->addFactory(factory);
}

MinVR::VRDisplayFactory* VRMain::getBaseDisplayFactory() {
	return _displayFactory;
}

void VRMain::setBaseDisplayFactory(MinVR::VRDisplayFactory* displayFactory) {
	_displayFactory = displayFactory;
}

// Adds the input device factories for all plugins who use this interface
void VRMain::addVRInputDeviceFactory(VRInputDeviceFactory* factory)
{
	_inputDeviceFactories.push_back(factory);
}

// Used for timing (i.e. for animation, etc...)
void VRMain::addVRTimer(MinVR::VRTimer* timer) {
	_timers.push_back(timer);
}

void 
VRMain::shutdown()
{

}

void VRMain::addEventHandler(MinVR::VREventHandler* eventHandler) {
	_eventHandlers.push_back(eventHandler);
}

void VRMain::removeEventHandler(MinVR::VREventHandler* eventHandler) {
	for (int f = 0; f < _eventHandlers.size(); f++)
	{
		if (_eventHandlers[f] == eventHandler)
		{
			_eventHandlers.erase(_eventHandlers.begin()+f);
			break;
		}
	}
}
