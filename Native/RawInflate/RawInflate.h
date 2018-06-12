#pragma once
#include <stdint.h>

#ifdef RAWINFLATE_BUILD
#define RAWINFLATE_EXPORT extern "C" __declspec (dllexport)
#else
#define RAWINFLATE_EXPORT extern "C" __declspec (dllimport)
#endif

RAWINFLATE_EXPORT int RawInflate(uint8_t* dst, int dst_offset, int dst_size, 
        const uint8_t* src, int src_offset, int src_size);
