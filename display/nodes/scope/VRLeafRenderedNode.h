/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef LEAFRENDEREDNODE_H_
#define LEAFRENDEREDNODE_H_

#include "display/VRDisplayNode.h"

namespace MinVR {

class VRLeafRenderedNode : public VRDisplayNode {
public:
	VRLeafRenderedNode();
	virtual ~VRLeafRenderedNode();

	void renderAtLeaf(VRRenderer& renderer);

	virtual void renderContextAtLeaf(VRRenderer& renderer);
	virtual void renderSceneAtLeaf(VRRenderer& renderer);
};

class VRLeafRenderer : public VRRenderer, public VRRenderHandler {
public:
	VRLeafRenderer(VRRenderer* renderer, VRLeafRenderedNode* node) : VRRenderer(this), m_renderer(renderer), m_node(node) {}
	virtual ~VRLeafRenderer() {}

	void renderContextCallback(VRRenderState& state);
	void renderSceneCallback(VRRenderState& state);
	VRRenderState& getState() { return m_renderer->getState(); }
	void pushState() { m_renderer->pushState(); }
	void popState() { m_renderer->popState(); }
	void resetState() { m_renderer->resetState(); }

private:
	VRRenderer* m_renderer;
	VRLeafRenderedNode* m_node;
};

} /* namespace MinVR */

#endif /* LEAFRENDEREDNODE_H_ */
