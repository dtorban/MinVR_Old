/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/renderers/VRRenderHandler.h>
#include <sstream>

namespace MinVR {

VRRenderHandler::VRRenderHandler() {
	resetState();
}

VRRenderHandler::~VRRenderHandler() {
}

void VRRenderHandler::renderContextCallback() {
	renderContextCallback(getState());
}

void VRRenderHandler::renderSceneCallback() {
	renderSceneCallback(getState());
}

VRRenderState& VRRenderHandler::getState() {
	return m_state;
}

void VRRenderHandler::pushState() {
	m_nodeNum++;
	std::stringstream ss;
	ss << m_nameSpaces[m_nameSpaces.size() - 1] << "/" << "node" << m_nodeNum;
	m_nameSpaces.push_back(ss.str());
	m_state.setNameSpace(m_nameSpaces[m_nameSpaces.size() - 1]);
}

void VRRenderHandler::popState() {
	m_nameSpaces.pop_back();
	m_state.setNameSpace(m_nameSpaces[m_nameSpaces.size() - 1]);
}

void VRRenderHandler::resetState() {
	m_nodeNum = 0;
	m_state = VRRenderState();
	m_nameSpaces.push_back("/display");
	m_state.setNameSpace("/display");
}

} /* namespace MinVR */
