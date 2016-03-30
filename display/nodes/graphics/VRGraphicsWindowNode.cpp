/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/nodes/graphics/VRGraphicsWindowNode.h>
#include "display/nodes/graphics/structure/VRViewportNode.h"
#include "display/factories/VRTypedDisplayFactory.h"
#include "main/VRSystem.h"

namespace MinVR {

VRGraphicsWindowNode::VRGraphicsWindowNode(const VRRect& rect) : m_rect(rect) {

}

VRGraphicsWindowNode::~VRGraphicsWindowNode() {
}

void VRGraphicsWindowNode::render(VRRenderHandler& renderer) {
	renderer.pushState();
	setCurrentContext();
	renderer.getState().setValue("graphicsContextType", getContextType());
	updateState(renderer.getState());
	renderer.renderContextCallback();
	VRDisplayNode::render(renderer);
	flush();
	renderer.popState();
	clearCurrentContext();
}

void VRGraphicsWindowNode::waitForRenderToComplete() {
	setCurrentContext();
	finish();
	clearCurrentContext();
}

void VRGraphicsWindowNode::displayTheFinishedRendering() {
	setCurrentContext();
	swapBuffers();
	clearCurrentContext();
}

void VRGraphicsWindowNode::updateState(VRRenderState& state) {
	VRRect r = VRViewportNode::calculate("rect", "viewport", state, m_rect, true);
	r.setXOffset(0);
	r.setYOffset(0);
	state.writeValue("viewport", r);
}

const VRRect& VRGraphicsWindowNode::getRect() const {
	return m_rect;
}

void VRGraphicsWindowNode::setRect(const VRRect& rect) {
	m_rect = rect;
}

VRDisplayNode* VRGraphicsWindowFactory::create(VRDataIndex& config,
		const std::string nameSpace, std::string type, VRDisplayFactory& baseFactory) {
	if (config.exists(nameSpace + "/windowType", ""))
	{
		VRGraphicsWindowNode* display = createWindow(config, nameSpace, config.getValue(nameSpace + "/windowType", ""), baseFactory);
		createChildren(display, baseFactory, config, nameSpace);

		return display;
	}

	return NULL;
}

} /* namespace MinVR */
