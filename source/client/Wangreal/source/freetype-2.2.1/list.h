// Original: d:\build\custom\temp\code\client\wangreal\source\freetype-2.2.1\list.h
#ifndef PG_CLIENT_WANGREAL_SOURCE_FREETYPE_2_2_1_LIST_H
#define PG_CLIENT_WANGREAL_SOURCE_FREETYPE_2_2_1_LIST_H

#include <mbstring.h>
#include <functional>
#include <map>

namespace list {

// Orders C strings by their multibyte comparison.
template <class T>
struct less_str : public std::binary_function<T, T, bool> {
	// list.h:12
	bool operator()(const T& a, const T& b) const {
		T rightText = b;
		T leftText = a;
		return _mbscmp((const unsigned char*)leftText, (const unsigned char*)rightText) < 0 ? true : false;
	}
};

// A multimap keyed by C strings.
template <class T>
class multimap_str : public std::multimap<const char*, T, less_str<const char*> > {
};

} // namespace list

#endif // PG_CLIENT_WANGREAL_SOURCE_FREETYPE_2_2_1_LIST_H
