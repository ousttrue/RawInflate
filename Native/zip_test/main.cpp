#include "../RawInflate/RawInflate.h"
#include <string>
#include <fstream>
#include <vector>
/*
#include <stdint.h>
#include <zlib.h>
#include <assert.h>
*/


static std::vector<uint8_t> readall(const std::string &path)
{
	std::vector<uint8_t> buffer;

	std::ifstream io(path, std::ios::binary);
	if (io) {
		io.seekg(0, std::ios::end);
		buffer.resize(io.tellg());
		io.seekg(0, std::ios::beg);
		io.read((char*)buffer.data(), buffer.size());
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

	std::vector<uint8_t> dst(header.UncompressedSize);
	auto pos = sizeof(header) + header.FileNameLength + header.ExtraFieldLength;

#if 0
	auto srcSize = (unsigned long)header.CompressedSize;
	auto dstSize = (unsigned long)dst.size();
	auto result = uncompress((unsigned char*)dst.data(), &dstSize,
		(unsigned char*)(src.data()+pos), srcSize);
#else
	auto result = RawInflate(dst.data(), dst.size(),
		src.data() + pos, header.CompressedSize);
#endif

	return 0;
}
