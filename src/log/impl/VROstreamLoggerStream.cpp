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

void VROstreamLoggerStream::log(VRLog::VRLogLevel lvl,
		const std::string& str) {
	getStream(lvl) << str;
}

void VROstreamLoggerStream::log(VRLog::VRLogLevel lvl, const int& i) {
	getStream(lvl) << i;
}

void VROstreamLoggerStream::log(VRLog::VRLogLevel lvl, const float& f) {
	getStream(lvl) << f;
}

void VROstreamLoggerStream::log(VRLog::VRLogLevel lvl, const double& d) {
	getStream(lvl) << d;
}

void VROstreamLoggerStream::log(VRLog::VRLogLevel lvl, const long & l) {
	getStream(lvl) << l;
}

void VROstreamLoggerStream::log(VRLog::VRLogLevel lvl, const char& c) {
	getStream(lvl) << c;
}

void VROstreamLoggerStream::log(VRLog::VRLogLevel lvl, const VRLogValue& obj) {
	obj.logValue(getStream(lvl));
}

void VROstreamLoggerStream::flush(VRLog::VRLogLevel lvl) {
	getStream(lvl).flush();
}


} /* namespace DSP */
