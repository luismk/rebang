// Original: d:\build\custom\temp\code\client\projectg\tinystr.h
// TinyXML 2.0-era string (cstring + allocated, no cached length).
#ifndef PG_CLIENT_PROJECTG_TINYSTR_H
#define PG_CLIENT_PROJECTG_TINYSTR_H

#include <assert.h>
#include <string.h>

#include "../../defines.h"

class TiXmlString {
	LAYOUT_CHECK_FRIEND;
public:
	TiXmlString(const char* instring);
	TiXmlString() {
		allocated = 0;
		cstring = NULL;
	}
	TiXmlString(const TiXmlString& copy);
	~TiXmlString() {
		empty_it();
	}

	const char* c_str() const {
		if (allocated)
			return cstring;
		return "";
	}
	unsigned length() const;

	void operator=(const char* content);
	void operator=(const TiXmlString& copy);

	TiXmlString& operator+=(const char* suffix) {
		append(suffix);
		return *this;
	}
	TiXmlString& operator+=(char single) {
		append(single);
		return *this;
	}
	TiXmlString& operator+=(TiXmlString& suffix) {
		append(suffix);
		return *this;
	}

	bool operator==(const TiXmlString& compare) const;
	bool operator<(const TiXmlString& compare) const;
	bool operator>(const TiXmlString& compare) const;

	bool empty() const {
		return length() ? false : true;
	}

	const char& at(unsigned index) const {
		assert(index < length());
		return cstring[index];
	}

	unsigned find(char lookup) const {
		return find(lookup, 0);
	}
	unsigned find(char tofind, unsigned offset) const;

	void reserve(unsigned size) {
		empty_it();
		if (size) {
			allocated = size;
			cstring = new char[size];
			cstring[0] = 0;
		}
	}

	char& operator[](unsigned index) const {
		assert(index < length());
		return cstring[index];
	}

	enum {
		notfound = 0xffffffff,
		npos = notfound
	};

	void append(const char* str, int len);

protected:
	char* cstring; // 0x0
	unsigned allocated; // 0x4

	unsigned assign_new_size(unsigned minimum_to_allocate) {
		return minimum_to_allocate * 2;
	}

	void empty_it() {
		if (cstring)
			delete[] cstring;
		cstring = NULL;
		allocated = 0;
	}

	void append(const char* suffix);
	void append(const TiXmlString& suffix) {
		append(suffix.c_str());
	}
	void append(char single) {
		char smallstr[2];
		smallstr[0] = single;
		smallstr[1] = 0;
		append(smallstr);
	}
};
CHECK_SIZE(TiXmlString, 0x8);
BEGIN_LAYOUT_CHECK(TiXmlString)
	CHECK_FIELD(TiXmlString, cstring, 0x0);
	CHECK_FIELD(TiXmlString, allocated, 0x4);
END_LAYOUT_CHECK

class TiXmlOutStream : public TiXmlString {
public:
	TiXmlOutStream() : TiXmlString() {
	}

	TiXmlOutStream& operator<<(const char* in) {
		*this += in;
		return (*this);
	}

	TiXmlOutStream& operator<<(const TiXmlString& in) {
		*this += in.c_str();
		return (*this);
	}
};
CHECK_SIZE(TiXmlOutStream, 0x8);

#endif // PG_CLIENT_PROJECTG_TINYSTR_H
