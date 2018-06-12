#include <string>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <zlib.h>
#include <assert.h>


static std::vector<char> readall(const std::string &path)
{
	std::vector<char> buffer;

	std::ifstream io(path, std::ios::binary);
	if (io) {
		io.seekg(0, std::ios::end);
		buffer.resize(io.tellg());
		io.seekg(0, std::ios::beg);
		io.read(buffer.data(), buffer.size());
	}
	return buffer;
}


#pragma pack(push, 1)
struct LocalFileHeader
{
	int32_t Signature; //0x04034b50
	uint16_t VersionNeededToExtrac;
	uint16_t GeneralPurposeBitFlag;
	uint16_t CompressionMethod;
	uint16_t FileLastModificationTime;
	uint16_t FileLastModificationDate;
	int32_t CRC32;
	int32_t CompressedSize;
	int32_t UncompressedSize;
	uint16_t FileNameLength;
	uint16_t ExtraFieldLength;
};
#pragma pack(pop)
static_assert(sizeof(LocalFileHeader) == 30, "invalid size");


// https://wlog.flatlib.jp/index.php?virtualpath=item/1653
// [4] Uncompress (zlib)
size_t Uncompress_zlib(void* dest_memory, size_t dest_size, const void* src_memory, size_t src_size)
{
	z_stream  stream;
	memset(&stream, 0, sizeof(z_stream));
	stream.next_in = (Bytef*)src_memory;
	stream.avail_in = src_size;
	stream.next_out = (Bytef*)dest_memory;
	stream.avail_out = dest_size;

	// https://wiz-code.net/blog/2009/09/zlibdll-zlibnet-deflatestream.html
	inflateInit2(&stream, -MAX_WBITS);

	int result = inflate(&stream, Z_FINISH);
	if (result != Z_STREAM_END && result != Z_OK) {
		// error
		return  0;
	}
	inflateEnd(&stream);

	assert(dest_size == stream.total_out);

	return  stream.total_out;  // uncompressed_size
}


int main(int argc, char **argv)
{
	if (argc < 2) {
		return 1;
	}

	auto src = readall(argv[1]);
	if (src.empty()) {
		return 2;
	}

	auto header = *((LocalFileHeader*)src.data());
	if (header.CompressionMethod != 8) {
		return 3;
	}

	std::vector<char> dst(header.UncompressedSize);
	auto pos = sizeof(header) + header.FileNameLength + header.ExtraFieldLength;

#if 0
	auto srcSize = (unsigned long)header.CompressedSize;
	auto dstSize = (unsigned long)dst.size();
	auto result = uncompress((unsigned char*)dst.data(), &dstSize,
		(unsigned char*)(src.data()+pos), srcSize);
#else
	auto result = Uncompress_zlib(dst.data(), dst.size(),
		src.data() + pos, header.CompressedSize);
#endif

	return 0;
}
