// Original: d:\build\custom\temp\code\client\projectg\tinystr.cpp
#include "tinystr.h"

#include <stdlib.h>
#include <string.h>

// tinystr.cpp:49
TiXmlString::TiXmlString(const char* instring) {
	unsigned newlen;
	char* newstring;

	if (!instring) {
		allocated = 0;
		cstring = NULL;
		return;
	}
	newlen = strlen(instring) + 1;
	newstring = new char[newlen];
	strcpy(newstring, instring);
	allocated = newlen;
	cstring = newstring;
}

// tinystr.cpp:68
TiXmlString::TiXmlString(const TiXmlString& copy) {
	unsigned newlen;
	char* newstring;

	// Prevent copy to self!
	if (&copy == this)
		return;

	if (!copy.allocated) {
		allocated = 0;
		cstring = NULL;
		return;
	}
	newlen = strlen(copy.cstring) + 1;
	newstring = new char[newlen];
	strcpy(newstring, copy.cstring);
	allocated = newlen;
	cstring = newstring;
}

// tinystr.cpp:91
void TiXmlString::operator=(const char* content) {
	unsigned newlen;
	char* newstring;

	if (!content) {
		empty_it();
		return;
	}
	newlen = strlen(content) + 1;
	newstring = new char[newlen];
	strcpy(newstring, content);
	empty_it();
	allocated = newlen;
	cstring = newstring;
}

// tinystr.cpp:110
void TiXmlString::operator=(const TiXmlString& copy) {
	unsigned newlen;
	char* newstring;

	if (!copy.length()) {
		empty_it();
		return;
	}
	newlen = copy.length() + 1;
	newstring = new char[newlen];
	strcpy(newstring, copy.c_str());
	empty_it();
	allocated = newlen;
	cstring = newstring;
}

// tinystr.cpp:140
void TiXmlString::append(const char* str, int len) {
	char* new_string;
	unsigned new_alloc, new_size;

	new_size = length() + len + 1;
	// check if we need to expand
	if (new_size > allocated) {
		// compute new size
		new_alloc = assign_new_size(new_size);

		// allocate new buffer
		new_string = new char[new_alloc];
		new_string[0] = 0;

		// copy the previous allocated buffer into this one
		if (allocated && cstring)
			strcpy(new_string, cstring);

		// append the suffix. It does exist, otherwise we wouldn't be expanding
		strncat(new_string, str, len);

		// return previously allocated buffer if any
		if (allocated && cstring)
			delete[] cstring;

		// update member variables
		cstring = new_string;
		allocated = new_alloc;
	} else {
		// we know we can safely append the string
		strncat(cstring, str, len);
	}
}

// tinystr.cpp:178
void TiXmlString::append(const char* suffix) {
	char* new_string;
	unsigned new_alloc, new_size;

	new_size = length() + strlen(suffix) + 1;
	// check if we need to expand
	if (new_size > allocated) {
		// compute new size
		new_alloc = assign_new_size(new_size);

		// allocate new buffer
		new_string = new char[new_alloc];
		new_string[0] = 0;

		// copy the previous allocated buffer into this one
		if (allocated && cstring)
			strcpy(new_string, cstring);

		// append the suffix. It does exist, otherwise we wouldn't be expanding
		strcat(new_string, suffix);

		// return previously allocated buffer if any
		if (allocated && cstring)
			delete[] cstring;

		// update member variables
		cstring = new_string;
		allocated = new_alloc;
	} else {
		// we know we can safely append the string
		strcat(cstring, suffix);
	}
}

// tinystr.cpp:220
unsigned TiXmlString::length() const {
	if (allocated)
		return strlen(cstring);
	return 0;
}

// tinystr.cpp:228
unsigned TiXmlString::find(char tofind, unsigned offset) const {
	char* lookup;

	if (offset >= length())
		return (unsigned)notfound;
	for (lookup = cstring + offset; *lookup; lookup++)
		if (*lookup == tofind)
			return (unsigned)(lookup - cstring);
	return (unsigned)notfound;
}

// tinystr.cpp:241
bool TiXmlString::operator==(const TiXmlString& compare) const {
	if (allocated && compare.allocated) {
		assert(cstring);
		assert(compare.cstring);
		return (strcmp(cstring, compare.cstring) == 0);
	}
	return false;
}

// tinystr.cpp:253
bool TiXmlString::operator<(const TiXmlString& compare) const {
	if (allocated && compare.allocated) {
		assert(cstring);
		assert(compare.cstring);
		return (strcmp(cstring, compare.cstring) > 0);
	}
	return false;
}

// tinystr.cpp:265
bool TiXmlString::operator>(const TiXmlString& compare) const {
	if (allocated && compare.allocated) {
		assert(cstring);
		assert(compare.cstring);
		return (strcmp(cstring, compare.cstring) < 0);
	}
	return false;
}
