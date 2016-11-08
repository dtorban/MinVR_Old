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

	VRVector3 vec;

	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left" : "Right";
		std::string handNameSpace = name + "/Hands/" + handType;
		dataIndex.addData(handNameSpace + "/Id", hand.id());
		std::string palmNameSpace = handNameSpace + "/Palm";

		vec = convertFromLeap(hand.palmPosition());
		dataIndex.addData(palmNameSpace + "/Pos", vec);

		vec = convertFromLeap(hand.palmNormal());
		dataIndex.addData(palmNameSpace + "/Normal", vec);

		vec = convertFromLeap(hand.direction());
		dataIndex.addData(handNameSpace + "/Dir", vec);

		dataIndex.addData(handNameSpace + "/Pitch", hand.direction().pitch() * RAD_TO_DEG);
		dataIndex.addData(handNameSpace + "/Roll", hand.palmNormal().roll() * RAD_TO_DEG);
		dataIndex.addData(handNameSpace + "/Yaw", hand.direction().yaw() * RAD_TO_DEG);

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

		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			std::string fingerType = fingerNames[finger.type()];
			std::string fingerNamespace = handNameSpace + "/Fingers/" + fingerType;
			dataIndex.addData(fingerNamespace + "/Id", finger.id());
			dataIndex.addData(fingerNamespace + "/Len", finger.length());
			dataIndex.addData(fingerNamespace + "/Width", finger.width());

			// Get finger bones
			for (int b = 0; b < 4; ++b) {
				Bone::Type boneType = static_cast<Bone::Type>(b);
				Bone bone = finger.bone(boneType);
				std::string boneNamespace = fingerNamespace + "/Bones/" + boneNames[boneType];

				vec = convertFromLeap(bone.prevJoint());
				dataIndex.addData(boneNamespace + "/Start", vec);

				vec = convertFromLeap(bone.nextJoint());
				dataIndex.addData(boneNamespace + "/End", vec);

				vec = convertFromLeap(bone.direction());
				dataIndex.addData(boneNamespace + "/Dir", vec);
			}
		}
	}

	events.push_back(dataIndex.serialize(name));
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

VRVector3 VRLeapMotionDevice::convertFromLeap(const Leap::Vector vec) {
	VRVector3 v(vec.z/30.0, vec.y/30.0, -1.0*vec.x/30.0);
	return v;
}

} /* namespace MinVR */
