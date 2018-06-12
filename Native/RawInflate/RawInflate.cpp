#include "RawInflate.h"
#include <zlib.h>
#include <string.h>
//#include <stdio.h>
#include <assert.h>


RAWINFLATE_EXPORT int RawInflate(uint8_t* dst, int dst_size, 
        const uint8_t* src, int src_size)
{
	z_stream  stream;
	memset(&stream, 0, sizeof(z_stream));
	stream.next_in = (Bytef*)src;
	stream.avail_in = src_size;
	stream.next_out = dst;
	stream.avail_out = dst_size;

	// https://wiz-code.net/blog/2009/09/zlibdll-zlibnet-deflatestream.html
	inflateInit2(&stream, -MAX_WBITS);

	int result = inflate(&stream, Z_FINISH);
	if (result != Z_STREAM_END && result != Z_OK) {
		// error
		return  0;
	}
	inflateEnd(&stream);

	assert(dst_size == stream.total_out);

	return  stream.total_out;  // uncompressed_size
}

