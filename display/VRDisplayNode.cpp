/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/VRDisplayNode.h>

using namespace std;

namespace MinVR {

VRDisplayNode::VRDisplayNode() {

}

VRDisplayNode::~VRDisplayNode() {
	clearChildren(true);
}

void VRDisplayNode::render(VRRenderer& renderer) {
	if (m_children.size() > 0)
	{
		for (vector<VRDisplayNode*>::iterator it = m_children.begin(); it != m_children.end(); it++)
		{
			(*it)->render(renderer);
		}
	}
	else
	{
		renderer.getRenderHandler().renderSceneCallback(renderer.getState());
	}
}

void VRDisplayNode::waitForRenderToComplete() {
	for (vector<VRDisplayNode*>::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		(*it)->waitForRenderToComplete();
	}
}

void VRDisplayNode::displayTheFinishedRendering() {
	for (vector<VRDisplayNode*>::iterator it = m_children.begin(); it != m_children.end(); it++)
	{
		(*it)->displayTheFinishedRendering();
	}
}

const std::vector<VRDisplayNode*>& VRDisplayNode::getChildren() const {
	return m_children;
}

void VRDisplayNode::addChild(VRDisplayNode* child) {
	m_children.push_back(child);
}

void VRDisplayNode::clearChildren(bool destroyChildren) {
	if (destroyChildren) {
		for (vector<VRDisplayNode*>::iterator it = m_children.begin(); it != m_children.end(); it++)
		{
				delete (*it);
		}
	}

	m_children.clear();
}

} /* namespace MinVR */
