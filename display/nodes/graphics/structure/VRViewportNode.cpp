/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/nodes/graphics/structure/VRViewportNode.h>
#include "VRTile.h"
#include "display/nodes/graphics/structure/VRTileNode.h"
#include "display/nodes/scope/VRStateScopeNode.h"

namespace MinVR {

VRViewportNode::VRViewportNode(const VRRect& rect) : m_rect(rect) {
}

VRViewportNode::~VRViewportNode() {
}

void VRViewportNode::render(VRRenderHandler& renderer) {
	calculate("viewport", renderer.getState(), m_rect, true);

	VRDisplayNode::render(renderer);
}

VRRect VRViewportNode::calculate(std::string inName, std::string outName,
		VRRenderState& state, const VRRect& rect, bool modifyTile) {
	const VRRect* currentRect = &rect;

	VRRect oldRect;
	VRRect modifiedRect;
	if (state.readValue(inName, oldRect))
	{
		modifiedRect = oldRect.generateChild(rect);
		currentRect = &modifiedRect;
	}

	if (modifyTile)
	{
		VRTile tile;
		if (state.readValue("tile", tile))
		{
			tile = tile.modifyWithRect(oldRect, *currentRect);
			state.writeValue("tile", tile);
		}
	}

	state.writeValue(outName, *currentRect);

	return *currentRect;
}

VRRect VRViewportNode::calculate(std::string inName, VRRenderState& state,
		const VRRect& rect, bool modifyTile) {
	calculate(inName, inName, state, rect, modifyTile);
}

VRDisplayNode* VRViewportFactory::create(VRDataIndex& config,
		const std::string nameSpace, VRDisplayFactory& baseFactory) {

	VRDisplayNode* display = m_innerFactory->create(config, nameSpace, baseFactory);
	bool createdScope = false;
	if (!display)
	{
		std::cout << "Created scope" << std::endl;
		VRStateScopeNode* scope = new VRStateScopeNode();
		createChildren(scope, baseFactory, config, nameSpace);
		display = scope;
		createdScope = true;
	}

	VRDisplayNode* displayNode = display;

	if (displayNode)
	{
		int startChildren = displayNode->getChildren().size();
		std::vector<VRDisplayNode*> children = displayNode->getChildren();
		displayNode->clearChildren();

		VRRect viewport;
		VRViewportNode* viewportNode = NULL;
		if (viewport.read(config, nameSpace + "/viewport", ""))
		{
			std::cout << "Created viewport" << std::endl;
			viewportNode = new VRViewportNode(viewport);
			displayNode->addChild(viewportNode);
			displayNode = viewportNode;
		}

		VRTile tile;
		VRTileNode* tileNode = NULL;
		if (tile.read(config, nameSpace + "/tile", ""))
		{
			std::cout << "Created tile" << std::endl;
			tileNode = new VRTileNode(tile);
			displayNode->addChild(tileNode);
			displayNode = tileNode;
		}

		for (int f = 0; f < children.size(); f++)
		{
			displayNode->addChild(children[f]);
		}

		if (displayNode->getChildren().size() == 0 && createdScope) {
			std::cout << "destroyed scope" << std::endl;
			delete display;
			return NULL;
		}
	}

	return display;
}

} /* namespace MinVR */
