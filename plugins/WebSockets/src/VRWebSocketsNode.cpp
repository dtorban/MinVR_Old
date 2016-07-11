/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <VRWebSocketsNode.h>

namespace MinVR {

static int callback_http(
                         struct lws *wsi,
                         enum lws_callback_reasons reason, void *user,
                         void *in, size_t len)
{
	return 0;
}

static int callback_dumb_increment(struct lws *wsi,
        enum lws_callback_reasons reason, void *user,
        void *in, size_t len)
{
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED: // just log message that someone is connecting
            printf("connection established\n");
            break;
        case LWS_CALLBACK_RECEIVE: { // the funny part
            // create a buffer to hold our response
            // it has to have some pre and post padding. You don't need to care
            // what comes there, libwebsockets will do everything for you. For more info see
            // http://git.warmcat.com/cgi-bin/cgit/libwebsockets/tree/lib/libwebsockets.h#n597

	    char json[] = "{\"name\": \"James Devilson\", \"message\": \"Hello World!\"}";
	    int newLen = strlen(json);
            unsigned char *buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + newLen +
                                                         LWS_SEND_BUFFER_POST_PADDING);
	    memcpy(&buf[LWS_SEND_BUFFER_PRE_PADDING], &json[0], newLen);
/*            int i;

            // pointer to `void *in` holds the incomming request
            // we're just going to put it in reverse order and put it in `buf` with
            // correct offset. `len` holds length of the request.
            for (i=0; i < len; i++) {
                buf[LWS_SEND_BUFFER_PRE_PADDING + (len - 1) - i ] = ((char *) in)[i];
            }*/

            // log what we recieved and what we're going to send as a response.
            // that disco syntax `%.*s` is used to print just a part of our buffer
            // http://stackoverflow.com/questions/5189071/print-part-of-char-array
            printf("received data: %s, replying: %.*s\n", (char *) in, (int) newLen,
                 buf + LWS_SEND_BUFFER_PRE_PADDING);

            // send response
            // just notice that we have to tell where exactly our response starts. That's
            // why there's `buf[LWS_SEND_BUFFER_PRE_PADDING]` and how long it is.
            // we know that our response has the same length as request because
            // it's the same message in reverse order.
            lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], newLen, LWS_WRITE_TEXT);

            // release memory back into the wild
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
    		"dumb-increment-protocol", // protocol name - very important!
    		        callback_dumb_increment,
        0              // per_session_data_size
    },
    {
        NULL, NULL, 0   /* End of list */
    }
};


VRWebSocketsNode::VRWebSocketsNode(const std::string &name, int port) : VRDisplayNode(name) {
	std::cout << "render web create" << std::endl;

    struct lws_context_creation_info info;
    //struct lws_protocols protocols[2];

    /*protocols[0].name = "http-only";
    protocols[0].callback = callback_http;
    protocols[0].per_session_data_size = 0;

    protocols[1].name = NULL;
    protocols[1].callback = NULL;
    protocols[1].per_session_data_size = 0;*/

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
	VRDisplayNode::render(renderState, renderHandler);
}

void VRWebSocketsNode::waitForRenderToComplete(VRDataIndex* renderState) {
	//std::cout << "waitForRenderToComplete web sockets" << std::endl;
	VRDisplayNode::waitForRenderToComplete(renderState);
}

void VRWebSocketsNode::displayFinishedRendering(VRDataIndex* renderState) {
	//std::cout << "displayFinishedRendering web sockets" << std::endl;
	VRDisplayNode::displayFinishedRendering(renderState);
}

VRDisplayNode* VRWebSocketsNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	//std::cout << "Register ws" << std::endl;
	int port = config->getValue("WebSocketPort", nameSpace);
	return new VRWebSocketsNode(nameSpace, port);
}

} /* namespace MinVR */
