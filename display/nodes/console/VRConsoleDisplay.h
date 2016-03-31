/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef FRCONSOLEDISPLAY_H_
#define VRCONSOLEDISPLAY_H_

#include "display/VRDisplayNode.h"
#include <iostream>

namespace MinVR {

class VRConsoleDisplay : public VRDisplayNode {
public:
	VRConsoleDisplay(std::ostream *stream = &std::cout);
	virtual ~VRConsoleDisplay();

	void render(VRRenderer& renderer);
	void waitForRenderToComplete();
	void displayTheFinishedRendering();

private:
	std::ostream* m_stream;
};

} /* namespace MinVR */

#endif /* CONSOLEDISPLAY_H_ */
