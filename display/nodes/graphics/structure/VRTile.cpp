/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/nodes/graphics/structure/VRTile.h>

namespace MinVR {

VRTile::VRTile() : m_topLeft(), m_topRight(), m_bottomLeft(), m_bottomRight() {
}

VRTile::~VRTile() {
}

void VRTile::write(VRDataIndex& index, std::string name) const {
	VRPoint3 topLeft(m_topLeft);
	index.addData(name + "/topLeft", topLeft);
	VRPoint3 topRight(m_topRight);
	index.addData(name + "/topRight", topRight);
	VRPoint3 bottomLeft(m_bottomLeft);
	index.addData(name + "/bottomLeft", bottomLeft);
	VRPoint3 bottomRight(m_bottomRight);
	index.addData(name + "/bottomRight", bottomRight);
}

bool VRTile::read(VRDataIndex& index, std::string name, std::string nameSpace) {
	if (!index.exists(name, nameSpace)) {
		return false;
	}

	std::string newNamespace = index.getName(name, nameSpace);

	m_topLeft = index.getValue("topLeft", newNamespace);
	m_topRight = index.getValue("topRight", newNamespace);
	m_bottomLeft = index.getValue("bottomLeft", newNamespace);
	m_bottomRight = index.getValue("bottomRight", newNamespace);

	return true;
}

VRTile VRTile::modifyWithRect(const VRRect& oldRect,
		const VRRect& newRect) {
	VRVector3 u = (m_topRight - m_topLeft).normalize();
	VRVector3 v = (m_topLeft - m_bottomLeft).normalize();

	double widthRatio = newRect.getWidth()/oldRect.getWidth();
	double heightRatio = newRect.getHeight()/oldRect.getHeight();

	double xOffsetRatio = (newRect.getXOffset()-oldRect.getXOffset())/oldRect.getWidth();
	double yOffsetRatio = (newRect.getYOffset()-oldRect.getYOffset())/oldRect.getHeight();

	m_bottomLeft = m_bottomLeft + u*xOffsetRatio + v*yOffsetRatio;

	m_bottomRight = m_bottomLeft + u*widthRatio;
	m_topLeft = m_bottomLeft + v*heightRatio;
	m_topRight = m_bottomRight + v*heightRatio;

	return *this;
}

} /* namespace MinVR */


