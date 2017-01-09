/*
 * Small jpeg decoder library - testing application
 *
 * Copyright (c) 2006, Luc Saillard <luc@saillard.org>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *
 * - Neither the name of the author nor the names of its contributors may be
 *  used to endorse or promote products derived from this software without
 *  specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "tinyjpeg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define RGB565(r, g, b) (unsigned short)((r << 11) + (g << 5) + b)

static int filesize(FILE *fp)
{
	long pos;
	fseek(fp, 0, SEEK_END);
	pos = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return pos;
}

int Get_JPEG_size(const char *infilename, unsigned int* wid, unsigned int* hei)
{
	FILE *fp;
	unsigned int length_of_file;
	unsigned int width, height;
	unsigned char* buf;
	struct jdec_private *jdec;

	/* Load the Jpeg into memory */
	fp = fopen(infilename, "rb");
	length_of_file = filesize(fp);
	buf = (unsigned char *)malloc(length_of_file + 4);
	fread(buf, length_of_file, 1, fp);
	fclose(fp);

	/* Decompress it */
	jdec = tinyjpeg_init();
	tinyjpeg_parse_header(jdec, buf, length_of_file);
	tinyjpeg_get_size(jdec, &width, &height);

	*wid = width;
	*hei = height;
	
	if (jdec != NULL) tinyjpeg_free(jdec);
	if (buf != NULL) free(buf);
	
    return ((width * height) * 2);
}



/**
 * Load one jpeg image, and decompress it, and save the result.
 */
void Load_JPEG(const char *infilename, uint16_t* copy)
{
	FILE *fp;
	unsigned int length_of_file;
	unsigned int width, height;
	unsigned char *components[1];
	unsigned char *buf;
	unsigned int i;
	unsigned int bufferlen;
	struct jdec_private *jdec;
  
	/* Load the Jpeg into memory */
	fp = fopen(infilename, "rb");
	length_of_file = filesize(fp);
	buf = (unsigned char *)malloc(length_of_file + 4);
	if (!buf) return;
	fread(buf, length_of_file, 1, fp);
	fclose(fp);

	/* Decompress it */
	jdec = tinyjpeg_init();
	tinyjpeg_parse_header(jdec, buf, length_of_file);
	tinyjpeg_get_size(jdec, &width, &height);
	tinyjpeg_decode(jdec);
	tinyjpeg_get_components(jdec, components);

    bufferlen = (width * height) * 2;
    
	uint16_t* pixels;
	pixels = malloc(bufferlen);
	if (!pixels) return;
	
	for (i = 0; i < width*height; i += 1) 
		pixels[i] = RGB565( components[0][i*3]/8,  components[0][i*3+1]/4,  components[0][i*3+2]/8);
		
	memcpy((uint16_t*)copy, (uint16_t*)pixels, bufferlen);

	if (jdec) tinyjpeg_free(jdec);
	if (buf) free(buf);
	if (pixels) free(pixels);
}
