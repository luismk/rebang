// Original: d:\build\custom\temp\code\client\projectg\type_convert.h
#ifndef PG_CLIENT_PROJECTG_TYPE_CONVERT_H
#define PG_CLIENT_PROJECTG_TYPE_CONVERT_H

#include <map>
#include <string>
#include <typeinfo>

#include "..\..\defines.h"

// The key of a registered converter: the type_info names of the source and
// destination types.
struct typename_pair {
	typename_pair(void) {}

	// type_convert.h:29
	bool operator<(const typename_pair &other) const {
		return std::string(m_srcName) + m_dstName < std::string(other.m_srcName) + other.m_dstName;
	}

	const char *m_srcName; // 0x0
	const char *m_dstName; // 0x4
};
CHECK_SIZE(typename_pair, 0x8);
CHECK_FIELD(typename_pair, m_srcName, 0x0);
CHECK_FIELD(typename_pair, m_dstName, 0x4);

// A registry of conversion functions between two structure types, keyed by
// their type names.
class type_convert {
public:
	typedef void (*converter)(const void *src, void *dst);

	// type_convert.h:41
	// Converts *src into *dst with the converter registered for S and D;
	// m_failed is set when none is.
	template <class S, class D>
	type_convert(const S *src, D *dst) : m_failed(false) {
		typename_pair key;
		key.m_srcName = typeid(S).name();
		key.m_dstName = typeid(D).name();

		std::map<typename_pair, converter>::iterator it = convmap().find(key);
		if (it != convmap().end()) {
			it->second(src, dst);
		} else {
			m_failed = true;
		}
	}

	// type_convert.h:58
	template <class S, class D>
	static bool insert_converter(converter conv) {
		typename_pair key;
		key.m_srcName = typeid(S).name();
		key.m_dstName = typeid(D).name();
		std::map<typename_pair, converter>::iterator it = convmap().find(key);

		if (it == convmap().end()) {
			convmap().insert(std::make_pair(key, conv));
		}

		return true;
	}

private:
	// type_convert.h:76
	static std::map<typename_pair, converter> &convmap(void) {
		static std::map<typename_pair, converter> _cvmap;
		return _cvmap;
	}

	bool m_failed; // 0x0
};

#endif // PG_CLIENT_PROJECTG_TYPE_CONVERT_H
