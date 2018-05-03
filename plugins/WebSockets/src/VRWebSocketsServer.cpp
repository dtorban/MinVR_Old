/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRWebSocketsServer.h"
#include <iostream>
#include <string>
#include <vector>

namespace MinVR {

struct per_session_data_input {
};

struct vr_server_context {
	std::string dataToSend;
	std::vector<WSReceiveCallback*> receiveCallbacks;
};


int callback_input(
		struct lws *wsi,
		enum lws_callback_reasons reason, void *user,
		void *in, size_t len)
{

	vr_server_context* serverContext = (vr_server_context*)(lws_context_user(lws_get_context(wsi)));

	struct per_session_data_input *pss =
			(struct per_session_data_input *)user;

	switch (reason) {
	case LWS_CALLBACK_ESTABLISHED: {
		std::cout << "Connection established" << std::endl;
		break;
	}
	case LWS_CALLBACK_CLOSED: {
		std::cout << "Connection closed" << std::endl;
		break;
	}
	case LWS_CALLBACK_RECEIVE: {
		std::cout << len << std::endl;
		//printf("received data: %s\n", (char *) in);
		//printf("received data: %s\n", std::string((char *) in,len).c_str());
		for (int f = 0; f < serverContext->receiveCallbacks.size(); f++) {
			serverContext->receiveCallbacks[f]->receive("input", 0, std::string((char *) in,len));
		}
		//FILE* pipe = popen((char *) in, "r");
		//pss->p = pipe;
		break;
	}
	case LWS_CALLBACK_SERVER_WRITEABLE: {
		std::string val = serverContext->dataToSend;//pixels;

		int newLen = val.length();
		unsigned char *buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + newLen +
				LWS_SEND_BUFFER_POST_PADDING);
		memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], val.c_str(), newLen);
		lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], newLen, LWS_WRITE_TEXT);
		free(buf);

		//std::cout << "write" << std::endl;
		/*FILE* pipe = (FILE*)pss->p;
		if (pipe) {
			if (!feof(pipe)) {
				//if (fscanf (pipe, "%s", pss->buffer) != 0) {
				if(fgets(pss->buffer, 128, pipe) != NULL){
					pss->buffer[strlen(pss->buffer)-1] = '\0';
					std::string val = std::string(pss->buffer);
					//std::cout << val << std::endl;

					int newLen = val.length();
					unsigned char *buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + newLen +
						LWS_SEND_BUFFER_POST_PADDING);
					memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], val.c_str(), newLen);
					lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], newLen, LWS_WRITE_TEXT);
				}
			}
			else {
				pclose(pipe);
				pss->p = 0;
			}
		}*/
		break;
	}
	}
	return 0;
}

struct lws_protocols protocols[] = {
		/* first protocol must always be HTTP handler */
		{
				"input",   // name
				callback_input, // callback
				sizeof (struct per_session_data_input)              // per_session_data_size
		},
		{
				NULL, NULL, 0   /* End of list */
		}
};

VRWebSocketsServer::VRWebSocketsServer(int port) {
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
	serverContext = new vr_server_context();
	info.user = serverContext;

	protocols[0].rx_buffer_size = 1024*1024;

	context = lws_create_context(&info);
	std::cout << "Buffer Size" << protocols[0].rx_buffer_size << std::endl;

	if (context == NULL) {
		std::cout << "libwebsocket init failed\n" << std::endl;
	}

	std::cout << "starting server..." << std::endl;

	/*while (true) {
		//std::cout << "loop" << std::endl;
		lws_service(context, 0);
		lws_callback_on_writable_all_protocol(context, &protocols[0]);
	}*/
}

VRWebSocketsServer::~VRWebSocketsServer() {
	lws_context_destroy(context);
	delete static_cast<vr_server_context*>(serverContext);
}

void VRWebSocketsServer::service() {
	lws_service(context, 0);
}

void VRWebSocketsServer::sendData(const std::string& data) {
	static_cast<vr_server_context*>(serverContext)->dataToSend = data;
	lws_callback_on_writable_all_protocol(context, &protocols[0]);
}

void VRWebSocketsServer::addReceiveCallback(WSReceiveCallback& callback) {
	static_cast<vr_server_context*>(serverContext)->receiveCallbacks.push_back(&callback);
}

} /* namespace MinVR */
