// Original: d:\build\custom\temp\code\client\projectg\wtemplate.h
#ifndef PG_CLIENT_PROJECTG_WTEMPLATE_H
#define PG_CLIENT_PROJECTG_WTEMPLATE_H

#include "..\..\defines.h"

#include <algorithm>
#include <map>

class FunctionMapper;

// Signature of a FunctionMapper handler (enumerator names not in the PDB):
// V = void, B = bool, P = the Exec parameter.
enum eFunctionType {
	FUNCTION_TYPE_VV = 1,
	FUNCTION_TYPE_BV = 2,
	FUNCTION_TYPE_VP = 3,
	FUNCTION_TYPE_BP = 4
};

// Declared while FunctionMapper is incomplete: cl gives the handler the
// general 16-byte pointer-to-member representation (8-byte aligned).
typedef void (FunctionMapper::*FUNCTION_MAPPER_PFN)(void);

// One registered handler: its id, signature and member function.
struct sFunctionHandler {
	int id;                       // 0x0
	eFunctionType type;           // 0x4
	FUNCTION_MAPPER_PFN pFunc;    // 0x8
};
CHECK_SIZE(sFunctionHandler, 0x18);
CHECK_FIELD(sFunctionHandler, id, 0x0);
CHECK_FIELD(sFunctionHandler, type, 0x4);
CHECK_FIELD(sFunctionHandler, pFunc, 0x8);

// Dispatches to the member function registered under the current type.
class FunctionMapper {
public:
	// wtemplate.h:65
	FunctionMapper(void) {}
	// wtemplate.h:66
	virtual ~FunctionMapper(void) {}

	// wtemplate.h:69
	void AddFunctionType(int id, eFunctionType type, void (FunctionMapper::*pFunc)(void)) {
		sFunctionHandler handler;
		handler.id = id;
		handler.type = type;
		handler.pFunc = pFunc;

		m_functions.insert(std::map<int, sFunctionHandler>::value_type(id, handler));
	}

	// wtemplate.h:79
	void SetFunctionType(int type) {
		m_type = type;
	}

	// wtemplate.h:86
	template <class T, class P>
	bool Exec(T* pObj, P param) {
		std::map<int, sFunctionHandler>::iterator it = m_functions.find(m_type);
		if (it == m_functions.end()) {
			return false;
		}

		sFunctionHandler handler = it->second;

		bool bResult = true;
		switch (handler.type) {
		case FUNCTION_TYPE_VV:
			(pObj->*(void (T::*)(void))handler.pFunc)();
			break;
		case FUNCTION_TYPE_BV:
			bResult = (pObj->*(bool (T::*)(void))(void (T::*)(void))handler.pFunc)();
			break;
		case FUNCTION_TYPE_VP:
			(pObj->*(void (T::*)(P))(void (T::*)(void))handler.pFunc)(param);
			break;
		case FUNCTION_TYPE_BP:
			bResult = (pObj->*(bool (T::*)(P))(void (T::*)(void))handler.pFunc)(param);
			break;
		}
		return bResult;
	}

	std::map<int, sFunctionHandler> m_functions;    // 0x4
	int m_type;                                     // 0x10
};
CHECK_FIELD(FunctionMapper, m_functions, 0x4);
CHECK_FIELD(FunctionMapper, m_type, 0x10);
CHECK_SIZE(FunctionMapper, 0x14);

namespace _util {

// Creates a T with new; a creator callable through a plain function pointer.
template <class T>
struct NewCreator {
	// wtemplate.h:141
	static T* CreateObject(void) {
		return new T;
	}
};

// A member function bound to its object (object at 0x0, member pointer at
// 0x4). Only the members the decompiled units use are transcribed.
template <class T, class R>
struct WMemFun {
	// wtemplate.h:166
	WMemFun(T& obj, R (T::*pFunc)(void))
		: m_pObj(&obj), m_pFunc(pFunc) {
	}

	T* m_pObj;                  // 0x0
	R (T::*m_pFunc)(void);      // 0x4
};

// A one-argument member function bound to its object. The member pointer
// follows the object; its size and alignment depend on T's inheritance
// (0x4 for a single-inheritance class, 0x8-aligned 0x8 for multiple).
template <class T, class R, class A>
struct WMemFun1 {
	// wtemplate.h:186
	WMemFun1(T& obj, R (T::*pFunc)(A))
		: m_pObj(&obj), m_pFunc(pFunc) {
	}

	// wtemplate.h:191
	R operator()(A arg) {
		return (m_pObj->*m_pFunc)(arg);
	}

	T* m_pObj;                  // 0x0
	R (T::*m_pFunc)(A);         // after m_pObj
};

// wtemplate.h:222
template <class T, class R>
WMemFun<T, R> MakeMemFun(T& obj, R (T::*pFunc)(void)) {
	return WMemFun<T, R>(obj, pFunc);
}

// wtemplate.h:229
template <class T, class R, class A>
WMemFun1<T, R, A> MakeMemFun1(T& obj, R (T::*pFunc)(A)) {
	return WMemFun1<T, R, A>(obj, pFunc);
}

}

// wtemplate.h:270
// Whether pred holds for an element of the list.
template <class L, class P>
bool FindIf(const L& list, P pred) {
	typename L::const_iterator it = std::find_if(list.begin(), list.end(), pred);
	return it != list.end() ? true : false;
}

// wtemplate.h:313
// Subtracts amount from value without going below limit; returns the result.
template <class T>
T SAFE_SUBVALUE(T& value, const T& limit, T amount) {
	if (value > limit) {
		value -= amount;
		if (value < limit) {
			value = limit;
		}
	}
	return value;
}

// wtemplate.h:354
// Adds amount to value without going above limit; returns the result.
template <class T>
T SAFE_ADDVALUE(T& value, const T& limit, T amount) {
	if (value < limit) {
		value += amount;
		if (value > limit) {
			value = limit;
		}
	}
	return value;
}

// wtemplate.h:502
// Appends count null elements to the container.
template <class T>
void back_insert_null(T& container, unsigned int count) {
	for (; count != 0; count--) {
		container.push_back(0);
	}
}

#endif // PG_CLIENT_PROJECTG_WTEMPLATE_H
