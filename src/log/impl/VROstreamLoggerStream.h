/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#ifndef VROSTREAMLOGGERSTREAM_H_
#define VROSTREAMLOGGERSTREAM_H_

#include <ostream>
#include "log/VRLogger.h"

namespace MinVR {

class VROstreamLoggerStream : public VRLoggerStreamInterface {
public:
	VROstreamLoggerStream();
	virtual ~VROstreamLoggerStream();

	void log(level::VRLogLevel lvl, const std::string& str);
	void log(level::VRLogLevel lvl, const int& i);
	void log(level::VRLogLevel lvl, const float& f);
	void log(level::VRLogLevel lvl, const double& d);
	void log(level::VRLogLevel lvl, const long& l);
	void log(level::VRLogLevel lvl, const char& c);
	void flush(level::VRLogLevel lvl);

	virtual bool canLog(level::VRLogLevel lvl) = 0;
	virtual std::ostream& getStream(level::VRLogLevel lvl) = 0;
};

} /* namespace DSP */

#endif /* VROSTREAMLOGGERSTREAM_H_ */
