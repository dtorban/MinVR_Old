/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VRREADPIXELSNODE_H_
#define VRREADPIXELSNODE_H_

#include "display/VRDisplayNode.h"

namespace MinVR {

class VRReadPixelsNode : public VRDisplayNode {
public:
	VRReadPixelsNode(const std::string &name);
	virtual ~VRReadPixelsNode();

	std::string getType() { return "VRReadPixelsNode"; }

	void render(VRDataIndex *renderState, VRRenderHandler *renderHandler);
	void waitForRenderToComplete(VRDataIndex *renderState);
	/*GLubyte* pixels = (GLubyte*) malloc(60 * 60 * sizeof(GLubyte) * 4);

void WebDisplay::drawGraphics(AbstractMVRAppRef app, int threadId,
		AbstractCameraRef camera, WindowRef window) {

	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	app->drawGraphics(threadId, camera, window);

	glReadPixels(0, 0, 60, 60, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	next(reinterpret_cast<unsigned char *>(pixels));*/

	static VRDisplayNode* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);
};

} /* namespace MinVR */

#endif /* VRREADPIXELSNODE_H_ */
