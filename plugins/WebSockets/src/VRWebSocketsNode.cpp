/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <VRWebSocketsNode.h>
#include <sstream>      // std::stringstream
#include <string>
#include "lws_config.h"
#include "config/base64/base64.h"

namespace MinVR {

static int callback_http(
		struct lws *wsi,
		enum lws_callback_reasons reason, void *user,
		void *in, size_t len)
{
	return 0;
}


static int callback_display(struct lws *wsi,
		enum lws_callback_reasons reason, void *user,
		void *in, size_t len)
{
	VRWebSocketsNode* node = (VRWebSocketsNode*)(lws_context_user(lws_get_context(wsi)));

	switch (reason) {
	case LWS_CALLBACK_ESTABLISHED:
		printf("connection established\n");
		node->numClients++;
		break;
	case LWS_CALLBACK_CLOSED: {
		printf("connection closed\n");
		node->numClients--;
		node->numClientsCompleted++;
		if (node->numClientsCompleted >= node->numClientsStarted)
		{
			node->setCurrentRenderMethod(VRWS_none);
		}
		break;
	}
	case LWS_CALLBACK_RECEIVE: {
/*
		char json[] = "{\"name\": \"James Devilson\", \"message\": \"Hello World!\"}";
		int newLen = strlen(json);
		unsigned char *buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + newLen +
				LWS_SEND_BUFFER_POST_PADDING);
		memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], &json[0], newLen);

		printf("received data: %s, replying: %.*s\n", (char *) in, (int) newLen,
				buf + LWS_SEND_BUFFER_PRE_PADDING);

		//lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], newLen, LWS_WRITE_TEXT);

		free(buf);*/
		node->numClientsCompleted++;
		//std::cout << node->numClientsCompleted << " " << node->numClients << std::endl;
		if (node->numClientsCompleted >= node->numClientsStarted)
		{
			node->setCurrentRenderMethod(VRWS_none);
		}
		break;
	}
	case LWS_CALLBACK_SERVER_WRITEABLE: {

		std::stringstream ss;

		//std::cout << node->getCurrentRenderState()->serializeJSON("/") << std::endl;

		ss << "{\"renderMethod\":\"" << node->getCurrentRenderMethod() << "\",\"renderState\":" << node->getCurrentRenderState()->serializeJSON("/") << "}";

		std::string output = ss.str();

		if (node->getCurrentRenderMethod() == VRWS_render) {
			output = (std::string)node->getCurrentRenderState()->getValue("pixels","/");
			output = base64_decode(output);
		}
		std::cout << node->getCurrentRenderMethod() << output.length() << std::endl;

		int newLen = output.length();
		unsigned char *buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + newLen +
				LWS_SEND_BUFFER_POST_PADDING);
		memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], output.c_str(), newLen);

		if (node->getCurrentRenderMethod() == VRWS_render) {
			lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], newLen, LWS_WRITE_BINARY);
		}
		else {
			lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], newLen, LWS_WRITE_TEXT);
		}
		//std::cout << node->getFrame() << std::endl;
		//std::cout << output << std::endl;
		//std::cout << output.size() << std::endl;

		free(buf);
		break;
	}
	default:
		break;
	}

	return 0;
}

static struct lws_protocols protocols[] = {
		/* first protocol must always be HTTP handler */
		{
				"http-only",   // name
				callback_http, // callback
				0              // per_session_data_size
		},
		{
				"display-protocol", // protocol name - very important!
				callback_display,
				0              // per_session_data_size
		},
		{
				NULL, NULL, 0   /* End of list */
		}
};


VRWebSocketsNode::VRWebSocketsNode(const std::string &name, int port) : VRDisplayNode(name), frame(0), numClients(0) {
	std::cout << "render web create" << std::endl;

	struct lws_context_creation_info info;

	memset(&info, 0, sizeof info);

	info.port = port;
	info.iface = NULL;
	info.protocols = protocols;
	info.extensions = NULL;
	info.ssl_cert_filepath = NULL;
	info.ssl_private_key_filepath = NULL;
	info.ssl_ca_filepath = NULL;
	info.gid = -1;
	info.uid = -1;
	info.options = 0;
	info.ka_time = 0;
	info.ka_probes = 0;
	info.ka_interval = 0;
	info.user = this;

	protocols[1].rx_buffer_size = 1024*1024;
	std::cout << "Buffer Size" << protocols[1].rx_buffer_size << std::endl;

	context = lws_create_context(&info);

	if (context == NULL) {
		std::cout << "libwebsocket init failed\n" << std::endl;
	}

	std::cout << "starting server..." << std::endl;
}

VRWebSocketsNode::~VRWebSocketsNode() {
}

void VRWebSocketsNode::render(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {
	//std::cout << "render web sockets" << std::endl;
	lws_service(context, 0);

	frame++;
	renderState->addData("frame",(int)frame);
	VRDisplayNode::render(renderState, renderHandler);
	currentRenderState = renderState;
	currentRenderHanlder = renderHandler;
	currentRenderMethod = VRWS_render;

	numClientsCompleted = 0;
	lws_callback_on_writable_all_protocol(context, &protocols[1]);
	numClientsStarted = numClients;
	if (numClientsStarted > 0) {
		while (currentRenderMethod != VRWS_none) {
			lws_service(context, 0);
		}
	}
	//std::cout << "finish render web sockets" << std::endl;

}

void VRWebSocketsNode::waitForRenderToComplete(VRDataIndex* renderState) {
	renderState->addData("frame",(int)frame);
	VRDisplayNode::waitForRenderToComplete(renderState);
	currentRenderState = renderState;
	currentRenderMethod = VRWS_waitForRenderToComplete;

	numClientsCompleted = 0;
	lws_callback_on_writable_all_protocol(context, &protocols[1]);
	numClientsStarted = numClients;
	if (numClientsStarted > 0) {
		while (currentRenderMethod != VRWS_none) {
			lws_service(context, 0);
		}
	}
}

void VRWebSocketsNode::displayFinishedRendering(VRDataIndex* renderState) {
	//std::cout << "displayFinishedRendering web sockets" << std::endl;
	renderState->addData("frame",(int)frame);
	VRDisplayNode::displayFinishedRendering(renderState);
	currentRenderState = renderState;
	currentRenderMethod = VRWS_displayFinishedRendering;

	numClientsCompleted = 0;
	lws_callback_on_writable_all_protocol(context, &protocols[1]);
	numClientsStarted = numClients;
	if (numClientsStarted > 0) {
		while (currentRenderMethod != VRWS_none) {
			lws_service(context, 0);
		}
	}
}

VRDisplayNode* VRWebSocketsNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	//std::cout << "Register ws" << std::endl;
	int port = config->getValue("WebSocketPort", nameSpace);
	return new VRWebSocketsNode(nameSpace, port);
}

} /* namespace MinVR */
