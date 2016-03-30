/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRVIEWPORTNODE_H_
#define VRVIEWPORTNODE_H_

#include "display/VRDisplayNode.h"
#include "math/VRRect.h"
#include "display/factories/VRDisplayFactory.h"
#include "main/VRSystem.h"

namespace MinVR {

class VRViewportNode : public VRDisplayNode {
public:
	VRViewportNode(const VRRect& rect);
	virtual ~VRViewportNode();

	void render(VRRenderHandler& renderer);

	static VRRect calculate(std::string inName, std::string outName, VRRenderState& state, const VRRect& rect, bool modifyTile = false);
	static VRRect calculate(std::string inName, VRRenderState& state, const VRRect& rect, bool modifyTile = false);

private:
	VRRect m_rect;
};

class VRViewportFactory : public VRDisplayFactory {
public:
	VRViewportFactory(VRDisplayFactory* innerFactory) : m_innerFactory(innerFactory) {}
	virtual ~VRViewportFactory() {}

	VRDisplayNode* create(VRDataIndex& config, const std::string nameSpace, VRDisplayFactory& baseFactory);

private:
	VRDisplayFactory* m_innerFactory;
};

} /* namespace MinVR */

#endif /* VRVIEWPORTNODE_H_ */
