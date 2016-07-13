/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <VRReadPixelsNode.h>
#include "VROpenGLHeaders.h"
#include "config/base64/base64.h"

namespace MinVR {

VRReadPixelsNode::VRReadPixelsNode(const std::string& name) : VRDisplayNode(name) {
}

VRReadPixelsNode::~VRReadPixelsNode() {
}

void VRReadPixelsNode::render(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {

	int width = renderState->getValue("WindowWidth","/");
	int height = renderState->getValue("WindowHeight","/");

	VRDisplayNode::render(renderState, renderHandler);

	GLubyte* pixels = (GLubyte*) malloc(width * height * sizeof(GLubyte) * 4);
	glFinish();
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	unsigned char* p = (reinterpret_cast<unsigned char *>(pixels));

	std::string pixels64 = base64_encode(p, width*height*4);
	free(pixels);

	renderState->addData("pixels", pixels64);

}

void VRReadPixelsNode::waitForRenderToComplete(VRDataIndex* renderState) {
}

VRDisplayNode* VRReadPixelsNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VRReadPixelsNode(nameSpace);
}

} /* namespace MinVR */
