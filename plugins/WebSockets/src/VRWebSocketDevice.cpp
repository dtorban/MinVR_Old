/*
 * Copyright Regents of the University of Minnesota, 2015.  This software is released under the following license: http://opensource.org/licenses/GPL-2.0.
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */


#include "VRWebSocketDevice.h"
#include <libwebsockets.h>


namespace MinVR {

static const char * const ws_device_event_tok[] = {
	"dummy___"
};


static signed char
ws_device_event_parse(struct lejp_ctx *ctx, char reason)
{
	VRDataIndex& di = *(VRDataIndex *)ctx->user;

	if (reason & LEJP_FLAG_CB_IS_VALUE) {
		//std::cout << ctx->path << " " << ctx->buf << std::endl;
		std::string path(ctx->path); 
		if (path == "Name") {
			di.setName(std::string(ctx->buf));
		}
		else {
			di.addData(path, std::string(ctx->buf));
		}
		return 0;
	}

	switch (reason) {
	case LEJPCB_COMPLETE:
		break;
	case LEJPCB_PAIR_NAME:
		break;
	}

	return 0;
}

VRWebSocketDevice::VRWebSocketDevice(VRMainInterface* vrMain, int port) : server(port) {
	vrMain->addEventHandler(this);
	server.addReceiveCallback(*this);
}

VRWebSocketDevice::~VRWebSocketDevice()
{
}

void VRWebSocketDevice::receive(const std::string& protocol, int sessionId, const std::string& data) {
	VRDataIndex di;

	struct lejp_ctx ctx;

	lws_set_log_level(7, NULL);

	lejp_construct(&ctx, ws_device_event_parse, &di, ws_device_event_tok, ARRAY_SIZE(ws_device_event_tok));

	int m = lejp_parse(&ctx, (const unsigned char*)data.c_str(), data.size());
	if (m < 0 && m != LEJP_CONTINUE) {
		goto bail;
	}

	events.push_back(di);
bail:
	lejp_destruct(&ctx);

}

void VRWebSocketDevice::appendNewInputEventsSinceLastCall(VRDataQueue *inputEvents)
{
	server.service();
	//server.sendData("abc");

    for (int f = 0; f < events.size(); f++) {
        inputEvents->push(events[f].serialize());
    }

    events.clear();   
}


void VRWebSocketDevice::onVREvent(const VRDataIndex &event) {
	server.sendData("{\"Name\":\"" + event.getName() + "\"}");
}

VRInputDevice* VRWebSocketDevice::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	int port = WS_SERVER_PORT;
	if (config->exists("Port", nameSpace)) {
		port = config->getValue("Port", nameSpace);
	}

	return new VRWebSocketDevice(vrMain, port);
}

};         // end namespace
