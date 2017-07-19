/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include "VRWebSocketsServer.h"
#include <iostream>

namespace MinVR {



int callback_input(
		struct lws *wsi,
		enum lws_callback_reasons reason, void *user,
		void *in, size_t len)
{
	//VRWebSocketsServer *server = (VRWebSocketsServer *)user;

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
			printf("received data: %s\n", (char *) in);
			std::cout << "Recieved" << std::endl;
			/*FILE* pipe = popen((char *) in, "r");
			pss->p = pipe;*/
			break;
		}
		case LWS_CALLBACK_SERVER_WRITEABLE: {
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
}


struct lws_protocols protocols[] = {
		/* first protocol must always be HTTP handler */
		{
				"input",   // name
				callback_input, // callback
				0//sizeof (VRWebSocketsServer*)              // per_session_data_size
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
	//info.user = this;

	//protocols[1].rx_buffer_size = 1024*1024;
	//std::cout << "Buffer Size" << protocols[1].rx_buffer_size << std::endl;

	context = lws_create_context(&info);

	if (context == NULL) {
		std::cout << "libwebsocket init failed\n" << std::endl;
	}

	std::cout << "starting server..." << std::endl;

	while (true) {
		//std::cout << "loop" << std::endl;
		lws_service(context, 0);
		lws_callback_on_writable_all_protocol(context, &protocols[0]);
	}
}

VRWebSocketsServer::~VRWebSocketsServer() {
}

void VRWebSocketsServer::service() {
	lws_service(context, 0);
}

} /* namespace MinVR */
