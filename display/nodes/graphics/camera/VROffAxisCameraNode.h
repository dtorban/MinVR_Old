/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef OFFAXISCAMERANODE_H_
#define OFFAXISCAMERANODE_H_

#include "display/VRDisplayNode.h"
#include "display/factories/VRTypedDisplayFactory.h"

namespace MinVR {

class VROffAxisCameraNode : public VRDisplayNode {
public:
	VROffAxisCameraNode();
	virtual ~VROffAxisCameraNode();

	void render(VRRenderHandler& renderer);
};

class VROffAxisRenderHandler : public VRRenderHandler {
public:
	VROffAxisRenderHandler(VRRenderHandler* renderer) : m_renderer(renderer) {}
	virtual ~VROffAxisRenderHandler() {}

	void renderContextCallback(VRRenderState& state);
	void renderSceneCallback(VRRenderState& state);
	VRRenderState& getState() { return m_renderer->getState(); }
	void pushState() { m_renderer->pushState(); }
	void popState() { m_renderer->popState(); }
	void resetState() { m_renderer->resetState(); }

private:
	VRRenderHandler* m_renderer;
};

class VRCameraDisplayFactory : public VRTypedDisplayFactory {
public:
	VRCameraDisplayFactory() {}
	virtual ~VRCameraDisplayFactory() {}

protected:
	VRDisplayNode* create(VRDataIndex& config, const std::string nameSpace, std::string type, VRDisplayFactory& baseFactory);
};


} /* namespace MinVR */

#endif /* OFFAXISCAMERANODE_H_ */
