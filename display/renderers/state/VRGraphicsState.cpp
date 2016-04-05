/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/renderers/state/VRGraphicsState.h>

namespace MinVR {

VRGraphicsState::VRGraphicsState(VRRenderState& state) : m_state(state) {
}

VRGraphicsState::~VRGraphicsState() {
}

VRRenderState& VRGraphicsState::getState() const {
	return m_state;
}

const VRMatrix4 VRGraphicsState::getCameraFrame() const {
	return VRMatrix4();
}

void VRGraphicsState::setCameraFrame(const VRMatrix4& frame) {
}

const std::string VRGraphicsState::getContextType() const {
	return m_state.getValue<std::string>("graphicsContextType", "none");
}

void VRGraphicsState::setContextType(const std::string& contextType) const {
	m_state.setValue("graphicsContextType", contextType);
}

const VRTile VRGraphicsState::getTile() const {
	VRTile tile;
	m_state.readValue("tile", tile);
	return tile;
}

void VRGraphicsState::setTile(const VRTile& tile) {
	m_state.writeValue("tile", tile);
}

const VRRect VRGraphicsState::getViewport() const {
	VRRect viewport;
	m_state.readValue("viewport", viewport);
	return viewport;
}

const VRMatrix4 VRGraphicsState::getProjectionMatrix() const {
	return VRMatrix4();
}

void VRGraphicsState::setProjectionMatrix(const VRMatrix4& mat) {
}

const VRMatrix4 VRGraphicsState::getViewMatrix() const {
	return VRMatrix4();
}

void VRGraphicsState::setViewMatrix(const VRMatrix4& mat) {
}

void VRGraphicsState::setViewport(const VRRect& rect) {
	m_state.writeValue("viewport", rect);
}

} /* namespace MinVR */
