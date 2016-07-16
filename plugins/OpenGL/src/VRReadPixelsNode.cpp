/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <VRReadPixelsNode.h>
#include "VROpenGLHeaders.h"
#include "config/base64/base64.h"

#include <stdlib.h>
#include <stdint.h>
#include <cstddef>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <fstream>
#include "config/base64/base64.h"

#include "jpeglib.h"


using namespace std;

namespace MinVR {

int compressImage(uint8_t * in_buffer, JOCTET * out_buffer, int width, int height);

VRReadPixelsNode::VRReadPixelsNode(const std::string& name) : VRDisplayNode(name) {
}

VRReadPixelsNode::~VRReadPixelsNode() {
}

void VRReadPixelsNode::render(VRDataIndex* renderState,
		VRRenderHandler* renderHandler) {

	int width = renderState->getValue("WindowWidth","/");
	int height = renderState->getValue("WindowHeight","/");

	VRDisplayNode::render(renderState, renderHandler);

	GLubyte* pixels = (GLubyte*) malloc(width * height * sizeof(GLubyte) * 3);
	glFinish();
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	unsigned char* p = (reinterpret_cast<unsigned char *>(pixels));

	JOCTET * out_buffer   = new JOCTET[width * height *3];
	int size = compressImage(p, out_buffer, width, height);

	std::string pixels64 = base64_encode(out_buffer, size);
	//std::string pixels64 = base64_encode(p, width*height*3);

	free(pixels);
	delete[] out_buffer;

	renderState->addData("pixels", pixels64);

}

void VRReadPixelsNode::waitForRenderToComplete(VRDataIndex* renderState) {
}

VRDisplayNode* VRReadPixelsNode::create(VRMainInterface* vrMain,
		VRDataIndex* config, const std::string& nameSpace) {
	return new VRReadPixelsNode(nameSpace);
}




/* setup the buffer but we did that in the main function */
void init_buffer(jpeg_compress_struct* cinfo) {}

/* what to do when the buffer is full; this should almost never
 * happen since we allocated our buffer to be big to start with
 */
boolean empty_buffer(jpeg_compress_struct* cinfo) {
	return TRUE;
}

/* finalize the buffer and do any cleanup stuff */
void term_buffer(jpeg_compress_struct* cinfo) {}

int compressImage(uint8_t * in_buffer, JOCTET * out_buffer, int width, int height) {
	/* load the image and note I assume this is a RGB 24 bit image
	 * you should do more checking/conversion if possible
	 */
	//FILE        *outfile  = fopen("/tmp/test.jpeg", "wb");

	//if (!outfile) throw runtime_error("Problem opening output file");

	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr       jerr;
	struct jpeg_destination_mgr dmgr;

	/* create our in-memory output buffer to hold the jpeg */
	//JOCTET * out_buffer   = new JOCTET[width * height *3];

	/* here is the magic */
	dmgr.init_destination    = init_buffer;
	dmgr.empty_output_buffer = empty_buffer;
	dmgr.term_destination    = term_buffer;
	dmgr.next_output_byte    = out_buffer;
	dmgr.free_in_buffer      = width * height *3;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	/* make sure we tell it about our manager */
	cinfo.dest = &dmgr;

	cinfo.image_width      = width;
	cinfo.image_height     = height;
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality (&cinfo, 75, true);
	jpeg_start_compress(&cinfo, true);

	JSAMPROW row_pointer;
	uint8_t *buffer    = new uint8_t[width * height*3];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int idindex = 3*((height-y-1)*width+x);
			int index = 3*(y*width+x);
			buffer[idindex+0]=in_buffer[index+0];
			buffer[idindex+1]=in_buffer[index+1];
			buffer[idindex+2]=in_buffer[index+2];
		}
	}

	/* main code to write jpeg data */
	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer = (JSAMPROW) &buffer[cinfo.next_scanline * width*3];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}
	jpeg_finish_compress(&cinfo);

	/* write the buffer to disk so you can see the image */
	//fwrite(out_buffer, cinfo.dest->next_output_byte - out_buffer,1 ,outfile);
	//std::cout << (cinfo.dest->next_output_byte - out_buffer) << std::endl;

	//std::string ebuffer = base64_encode(out_buffer, cinfo.dest->next_output_byte - out_buffer);
	//std::cout << ebuffer << std::endl;

	delete[] buffer;

	return  cinfo.dest->next_output_byte - out_buffer;

}


} /* namespace MinVR */
