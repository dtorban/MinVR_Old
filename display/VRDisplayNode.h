/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRDISPLAYNODE_H_
#define VRDISPLAYNODE_H_

#include "display/renderers/VRRenderer.h"
#include <vector>

namespace MinVR {

/*
 * VRDisplayNode is the fundamental class that can be inherited to create a different types of displays.
 */
class VRDisplayNode {
public:
	VRDisplayNode();

	// VRDisplayNode assumes that it owns its children, so it will delete its children when destroyed.  If
	// this functionality is not desired, create an adapter display node with a weak pointer.
	virtual ~VRDisplayNode();

	// Calls render function on each of the display node's children.  If there are no children, then we are
	// at a leaf node and renderer.render() is called.  VRRenderer allows the display to control state and
	virtual void render(VRRenderer& renderer);

	// Waits for rendering to complete on the display node
	virtual void waitForRenderToComplete();

	// Actually displays the rendered results
	virtual void displayTheFinishedRendering();

	// Returns the display node's children
	virtual const std::vector<VRDisplayNode*>& getChildren() const;

	// Adds a display node child
	void addChild(VRDisplayNode* child);

	// Clears the children in a node
	void clearChildren(bool destroyChildren = false);

private:
	std::vector<VRDisplayNode*> m_children;
};

}

#endif /* VRSCOPENODE_H_ */

