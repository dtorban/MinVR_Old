/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRTILE_H_
#define VRTILE_H_

#include "config/VRWritable.h"
#include "math/VRMath.h"
#include "math/VRRect.h"

namespace MinVR {

class VRTile : public VRWritable {
public:
	VRTile();
	virtual ~VRTile();

	void write(VRDataIndex& index, std::string name) const;
	bool read(VRDataIndex& index, std::string name, std::string nameSpace);

	VRTile modifyWithRect(const VRRect& oldViewport, const VRRect& newViewport);

	const VRPoint3& getBottomLeft() const {
		return m_bottomLeft;
	}

	void setBottomLeft(const VRPoint3& bottomLeft) {
		m_bottomLeft = bottomLeft;
	}

	const VRPoint3& getBottomRight() const {
		return m_bottomRight;
	}

	void setBottomRight(const VRPoint3& bottomRight) {
		m_bottomRight = bottomRight;
	}

	const VRPoint3& getTopLeft() const {
		return m_topLeft;
	}

	void setTopLeft(const VRPoint3& topLeft) {
		m_topLeft = topLeft;
	}

	const VRPoint3& getTopRight() const {
		return m_topRight;
	}

	void setTopRight(const VRPoint3& topRight) {
		m_topRight = topRight;
	}

private:
	VRPoint3 m_topLeft;
	VRPoint3 m_topRight;
	VRPoint3 m_bottomLeft;
	VRPoint3 m_bottomRight;
};

} /* namespace MinVR */

#endif /* VRTILE_H_ */
