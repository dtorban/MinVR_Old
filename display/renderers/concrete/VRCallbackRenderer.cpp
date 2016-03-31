/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/renderers/concrete/VRCallbackRenderer.h>

namespace MinVR {

VRCallbackRenderer::VRCallbackRenderer(MethodType sceneCallback, MethodType contextCallback) : VRRenderer(this) {
	m_sceneCallback = sceneCallback;
	m_contextCallback = contextCallback;
}

VRCallbackRenderer::~VRCallbackRenderer() {
}

void VRCallbackRenderer::renderContextCallback(VRRenderState& state) {
	if (m_contextCallback != NULL)
	{
		(*m_contextCallback)(state);
	}
}

void VRCallbackRenderer::renderSceneCallback(VRRenderState& state) {
	(*m_sceneCallback)(state);
}

} /* namespace MinVR */
