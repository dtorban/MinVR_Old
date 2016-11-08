/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRLeapMotionDevice.h"
#include <math/VRMath.h>
#include <iostream>

using namespace Leap;

namespace MinVR {

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

Controller VRLeapMotionDevice::controller;

VRLeapMotionDevice::VRLeapMotionDevice(const std::string& name) : name(name) {
}

VRLeapMotionDevice::~VRLeapMotionDevice() {
	controller.removeListener(*this);
}

void VRLeapMotionDevice::appendNewInputEventsSinceLastCall(
		VRDataQueue* inputEvents) {
    for (int f = 0; f < events.size(); f++)
    {
    	inputEvents->push(events[f]);
    }

    events.clear();
}

void VRLeapMotionDevice::onInit(const Controller&) {
	std::cout << "Initialized" << std::endl;
}

void VRLeapMotionDevice::onConnect(const Controller&) {
	std::cout << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
}

void VRLeapMotionDevice::onDisconnect(const Controller&) {
	std::cout << "Disconnected" << std::endl;
}

void VRLeapMotionDevice::onExit(const Controller&) {
	std::cout << "Exited" << std::endl;
}

void VRLeapMotionDevice::onFrame(const Controller&) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();

	if (frame.hands().isEmpty()) {
		return;
	}

	std::string event = name;

	dataIndex.addData(name + "/Hands/NumHands", frame.hands().count());

	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left" : "Right";
		std::string handNameSpace = name + "/Hands/" + handType;
		dataIndex.addData(handNameSpace + "/Id", hand.id());
		std::string palmNameSpace = handNameSpace + "/Palm";
		const Vector palmPos = hand.palmPosition();
		VRPoint3 pos(palmPos.x, palmPos.y, palmPos.z);
		dataIndex.addData(palmNameSpace + "/Pos", pos);
		const Vector palmNormal = hand.palmNormal();
		VRVector3 normal(palmNormal.x, palmNormal.y, palmNormal.z);
		dataIndex.addData(palmNameSpace + "/Normal", normal);
		const Vector direction = hand.direction();
		VRVector3 dir(direction.x, direction.y, direction.z);
		dataIndex.addData(handNameSpace + "/Dir", normal);

		dataIndex.addData(handNameSpace + "/Pitch", direction.pitch() * RAD_TO_DEG);
		dataIndex.addData(handNameSpace + "/Roll", palmNormal.roll() * RAD_TO_DEG);
		dataIndex.addData(handNameSpace + "/Yaw", direction.yaw() * RAD_TO_DEG);

		std::string armNameSpace = handNameSpace + "/Arm";
		Arm arm = hand.arm();
		const Vector armDirection = arm.direction();
		VRVector3 armDir(armDirection.x, armDirection.y, armDirection.z);
		dataIndex.addData(armNameSpace + "/Dir", armDir);

		const Vector wristPosition = arm.wristPosition();
		VRVector3 wristPos(wristPosition.x, wristPosition.y, wristPosition.z);
		dataIndex.addData(armNameSpace + "/WristPos", wristPos);

		const Vector elbowPosition = arm.elbowPosition();
		VRVector3 elbowPos(elbowPosition.x, elbowPosition.y, elbowPosition.z);
		dataIndex.addData(armNameSpace + "/ElbowPos", wristPos);

		/*std::cout << std::string(2, ' ') <<  "Arm direction: " << arm.direction()
	            		  << " wrist position: " << arm.wristPosition()
	            		  << " elbow position: " << arm.elbowPosition() << std::endl;*/
	}

	events.push_back(dataIndex.serialize(name));

	if (true) {
		return;
	}

	/*std::cout << "Frame id: " << frame.id()
	            		<< ", timestamp: " << frame.timestamp()
	            		<< ", hands: " << frame.hands().count()
	            		<< ", extended fingers: " << frame.fingers().extended().count()
	            		<< ", tools: " << frame.tools().count()
	            		<< ", gestures: " << frame.gestures().count() << std::endl;

	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
	            		  << ", palm position: " << hand.palmPosition() << std::endl;
		// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();

		// Calculate the hand's pitch, roll, and yaw angles
		std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
				<< "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
				<< "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;

		// Get the Arm bone
		Arm arm = hand.arm();
		std::cout << std::string(2, ' ') <<  "Arm direction: " << arm.direction()
	            		  << " wrist position: " << arm.wristPosition()
	            		  << " elbow position: " << arm.elbowPosition() << std::endl;

		// Get fingers
		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			std::cout << std::string(4, ' ') <<  fingerNames[finger.type()]
			                                                 << " finger, id: " << finger.id()
			                                                 << ", length: " << finger.length()
			                                                 << "mm, width: " << finger.width() << std::endl;

			// Get finger bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				std::cout << std::string(6, ' ') <<  boneNames[boneType]
				                                               << " bone, start: " << bone.prevJoint()
				                                               << ", end: " << bone.nextJoint()
				                                               << ", direction: " << bone.direction() << std::endl;
			}
		}
	}

	// Get tools
	const ToolList tools = frame.tools();
	for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
		const Tool tool = *tl;
		std::cout << std::string(2, ' ') <<  "Tool, id: " << tool.id()
	            		  << ", position: " << tool.tipPosition()
	            		  << ", direction: " << tool.direction() << std::endl;
	}

	// Get gestures
	const GestureList gestures = frame.gestures();
	for (int g = 0; g < gestures.count(); ++g) {
		Gesture gesture = gestures[g];

		switch (gesture.type()) {
		case Gesture::TYPE_CIRCLE:
		{
			CircleGesture circle = gesture;
			std::string clockwiseness;

			if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
				clockwiseness = "clockwise";
			} else {
				clockwiseness = "counterclockwise";
			}

			// Calculate angle swept since last frame
			float sweptAngle = 0;
			if (circle.state() != Gesture::STATE_START) {
				CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
				sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
			}
			std::cout << std::string(2, ' ')
			<< "Circle id: " << gesture.id()
			<< ", state: " << stateNames[gesture.state()]
			                             << ", progress: " << circle.progress()
			                             << ", radius: " << circle.radius()
			                             << ", angle " << sweptAngle * RAD_TO_DEG
			                             <<  ", " << clockwiseness << std::endl;
			break;
		}
		case Gesture::TYPE_SWIPE:
		{
			SwipeGesture swipe = gesture;
			std::cout << std::string(2, ' ')
			<< "Swipe id: " << gesture.id()
			<< ", state: " << stateNames[gesture.state()]
			                             << ", direction: " << swipe.direction()
			                             << ", speed: " << swipe.speed() << std::endl;
			break;
		}
		case Gesture::TYPE_KEY_TAP:
		{
			KeyTapGesture tap = gesture;
			std::cout << std::string(2, ' ')
			<< "Key Tap id: " << gesture.id()
			<< ", state: " << stateNames[gesture.state()]
			                             << ", position: " << tap.position()
			                             << ", direction: " << tap.direction()<< std::endl;
			break;
		}
		case Gesture::TYPE_SCREEN_TAP:
		{
			ScreenTapGesture screentap = gesture;
			std::cout << std::string(2, ' ')
			<< "Screen Tap id: " << gesture.id()
			<< ", state: " << stateNames[gesture.state()]
			                             << ", position: " << screentap.position()
			                             << ", direction: " << screentap.direction()<< std::endl;
			break;
		}
		default:
			std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
			break;
		}
	}

	if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
		std::cout << std::endl;
	}*/
}

void VRLeapMotionDevice::onFocusGained(const Controller&) {
	std::cout << "Focus Gained" << std::endl;
}

void VRLeapMotionDevice::onFocusLost(const Controller&) {
	std::cout << "Focus Lost" << std::endl;
}

void VRLeapMotionDevice::onDeviceChange(const Controller&) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void VRLeapMotionDevice::onServiceConnect(const Controller&) {
	std::cout << "Service Connected" << std::endl;
}

void VRLeapMotionDevice::onServiceDisconnect(const Controller&) {
	std::cout << "Service Disconnected" << std::endl;
}

VRInputDevice* VRLeapMotionDevice::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	VRLeapMotionDevice* listener = new VRLeapMotionDevice("Leap");
	controller.addListener(*listener);
	return listener;
}

} /* namespace MinVR */
