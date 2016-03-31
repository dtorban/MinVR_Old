/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRTILENODE_H_
#define VRTILENODE_H_

#include "VRTile.h"
#include "display/VRDisplayNode.h"

namespace MinVR {

class VRTileNode : public VRDisplayNode {
public:
	VRTileNode(const VRTile& tile);
	virtual ~VRTileNode();

	void render(VRRenderer& renderer);

private:
	VRTile m_tile;
};

} /* namespace MinVR */

#endif /* VRTILENODE_H_ */
