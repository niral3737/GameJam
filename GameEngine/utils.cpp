///*
//==================================================================================================================================================================
//|Copyright 2018 Oscar Lara	- scarlara@hotmail.com																												   |
//|																																								   |
//|See individual libraries separate legal notices																												   |
//|																																								   |
//|Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"),				   |
//|to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,				   |
//|and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :					   |
//|																																								   |
//|The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.									   |
//|																																								   |
//|THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			   |
//|FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,	   |
//|WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.		   |
//==================================================================================================================================================================
//*/
//
#include "utils.h"


int little_endian_to_int(unsigned char *input_bytes, int byte_size) {

	//little-endian  = (input_bytes[0]<<0) | (input_bytes[1]<<8) | (input_bytes[2]<<16) | (input_bytes[3]<<24 ....);

	int value = 0;
	int byte_offset = 0;

	for (int i = 0; i < byte_size; i++)
	{
		value |= input_bytes[i] << byte_offset;
		byte_offset += 8;
	}
	

	return value;
}

int big_endian_to_int(unsigned char* input_bytes, int byte_size) {

	//big-endian = (input_bytes[3]<<0) | (input_bytes[2]<<8) | (input_bytes[1]<<16) | (input_bytes[0]<<24 .... );

	int value = 0;
	int byte_offset = (byte_size -1) * 8;

	for (int i = (byte_size - 1); i >= 0; i--)
	{
		value |= input_bytes[i] << byte_offset;
		byte_offset -= 8;
	}


	return value;

}

