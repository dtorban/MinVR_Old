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
	if (canLog(lvl)) getStream(lvl) << str;
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl, const int& i) {
	if (canLog(lvl)) getStream(lvl) << i;
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl, const float& f) {
	if (canLog(lvl)) getStream(lvl) << f;
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl, const double& d) {
	if (canLog(lvl)) getStream(lvl) << d;
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl, const long & l) {
	if (canLog(lvl)) getStream(lvl) << l;
}

void VROstreamLoggerStream::log(level::VRLogLevel lvl, const char& c) {
	if (canLog(lvl)) getStream(lvl) << c;
}

void VROstreamLoggerStream::flush(level::VRLogLevel lvl) {
	if (canLog(lvl)) getStream(lvl).flush();
}


} /* namespace DSP */
