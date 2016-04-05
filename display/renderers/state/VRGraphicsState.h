/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRGRAPHICSSTATE_H_
#define VRGRAPHICSSTATE_H_

#include "display/renderers/VRRenderState.h"
#include "math/VRRect.h"
#include "math/VRMath.h"
#include "display/nodes/graphics/structure/VRTile.h"

namespace MinVR {

class VRGraphicsState {
public:
	VRGraphicsState(VRRenderState& state);
	virtual ~VRGraphicsState();

	VRRenderState& getState() const;

	const VRMatrix4 getCameraFrame() const;
	void setCameraFrame(const VRMatrix4& frame);

	const VRMatrix4 getProjectionMatrix() const;
	void setProjectionMatrix(const VRMatrix4& mat);
	const VRMatrix4 getViewMatrix() const;
	void setViewMatrix(const VRMatrix4& mat);

	const std::string getContextType() const;
	void setContextType(const std::string& contextType) const;
	const VRTile getTile() const;
	void setTile(const VRTile& tile);
	const VRRect getViewport() const;
	void setViewport(const VRRect& rect);

private:
	VRRenderState& m_state;
};

} /* namespace MinVR */

#endif /* VRGRAPHICSSTATE_H_ */
