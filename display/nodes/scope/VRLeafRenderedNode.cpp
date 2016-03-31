/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/nodes/scope/VRLeafRenderedNode.h>

namespace MinVR {

VRLeafRenderedNode::VRLeafRenderedNode() {
	// TODO Auto-generated constructor stub

}

VRLeafRenderedNode::~VRLeafRenderedNode() {
	// TODO Auto-generated destructor stub
}

void VRLeafRenderedNode::renderAtLeaf(VRRenderer& renderer) {
	VRLeafRenderer leafRender(&renderer, this);
	VRDisplayNode::render(leafRender);
}

void VRLeafRenderedNode::renderContextAtLeaf(VRRenderer& renderer) {
	renderer.getRenderHandler().renderContextCallback(renderer.getState());
}

void VRLeafRenderedNode::renderSceneAtLeaf(VRRenderer& renderer) {
	renderer.getRenderHandler().renderSceneCallback(renderer.getState());
}

void VRLeafRenderer::renderContextCallback(VRRenderState& state) {
	m_node->renderContextAtLeaf(*m_renderer);
}

void VRLeafRenderer::renderSceneCallback(VRRenderState& state) {
	m_node->renderSceneAtLeaf(*m_renderer);
}

} /* namespace MinVR */

