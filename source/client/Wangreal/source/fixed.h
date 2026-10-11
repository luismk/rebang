// Original: d:\build\custom\temp\code\client\wangreal\source\fixed.h
#ifndef PG_CLIENT_WANGREAL_SOURCE_FIXED_H
#define PG_CLIENT_WANGREAL_SOURCE_FIXED_H

// 16.16 fixed-point helpers of the software shadow rasteriser. Only
// wshadowview.cpp includes this header; its object holds the out-of-line
// copies.

// fixed.h:13
inline long FloatToFixed16_16(float Value) {
	return (long)(Value * 65536.0f + 32768.0f);
}

// fixed.h:28
inline short Fixed16_16ToShort(long Value) {
	return (short)(Value >> 16);
}

#endif // PG_CLIENT_WANGREAL_SOURCE_FIXED_H
