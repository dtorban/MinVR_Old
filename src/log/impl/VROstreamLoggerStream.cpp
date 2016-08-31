/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <log/impl/VROstreamLoggerStream.h>

namespace MinVR {

VROstreamLoggerStream::VROstreamLoggerStream() {
	// TODO Auto-generated constructor stub

}

VROstreamLoggerStream::~VROstreamLoggerStream() {
	// TODO Auto-generated destructor stub
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl,
		const std::string& str) {
	getStream(lvl) << str;
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl, const int& i) {
	getStream(lvl) << i;
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl, const float& f) {
	getStream(lvl) << f;
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl, const double& d) {
	getStream(lvl) << d;
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl, const long & l) {
	getStream(lvl) << l;
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl, const char& c) {
	getStream(lvl) << c;
}

void VROstreamLoggerStream::flush(level::VRLogLevel lvl) {
	getStream(lvl).flush();
}


} /* namespace DSP */
