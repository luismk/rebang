// Original: d:\build\custom\temp\code\client\projectg\commonutil.h
#ifndef PG_CLIENT_PROJECTG_COMMONUTIL_H
#define PG_CLIENT_PROJECTG_COMMONUTIL_H

#include <list>
#include <stack>
#include <string>

#include "..\..\defines.h"
#include "singleton.h"

class cFile;
class WResourceManager;

// Only the functions other units call so far are declared.
char* __cdecl MakeStr(char const* pszFormat, ...);
char* GetStr(char* str, char* out);
bool str_Replace(std::string& str, const std::string& from, const std::string& to);
unsigned long ModifyColor(unsigned long color, float factor);

// commonutil.h:70
inline int float2int(float value) {
	return (int)value;
}

// commonutil.h:76
inline int Ceil(float value, float tolerance) {
	int result;
	if (value > 0.0f) {
		result = (int)value;
		if (!(value - (float)result < tolerance)) {
			result += 1;
		}
	} else {
		result = (int)value;
		if (!((float)result - value < tolerance)) {
			result = result + -1;
		}
	}
	return result;
}

// commonutil.h:99
template <class T>
void sequence_delete(T first, T last) {
	while (first != last) {
		delete *first++;
	}
}

// A file opened through the resource manager (pak or disk).
class WFile {
	LAYOUT_CHECK_FRIEND;
public:
	WFile();
	WFile(const char* filename);
	virtual ~WFile();

	cFile* Open(const char* filename, unsigned int flag);
	virtual void Close();
	virtual int Read(void* buffer, int size);
	virtual int GetByte();
	virtual int Tell();
	virtual void Seek(int offset, int origin);
	virtual int Length();

	static void SetResourceManager(WResourceManager* pResMng);

protected:
	const char* m_filename; // 0x4
	cFile* m_pFile; // 0x8

	static WResourceManager* m_pResMng;
};
CHECK_SIZE(WFile, 0xc);
BEGIN_LAYOUT_CHECK(WFile)
	CHECK_FIELD(WFile, m_filename, 0x4);
	CHECK_FIELD(WFile, m_pFile, 0x8);
END_LAYOUT_CHECK

// A WSingleton whose current instance can be saved on a stack and
// restored later (a modal screen pushes the scene's instance, creates its
// own, then pops the saved one back).
template <class T>
class CStackableSingleton : public WSingleton<T> {
public:
	// commonutil.h:169
	virtual ~CStackableSingleton() {}

protected:
	// commonutil.h:172
	CStackableSingleton() {}

public:
	// commonutil.h:183
	static void PushInstance() {
		m_InstanceStack.push(WSingleton<T>::m_pInstance);
		WSingleton<T>::m_pInstance = 0;
	}

	// commonutil.h:190
	static void PopInstance() {
		if (m_InstanceStack.empty()) {
			WSingleton<T>::m_pInstance = 0;
		} else {
			WSingleton<T>::m_pInstance = m_InstanceStack.top();
			m_InstanceStack.pop();
		}
	}

	static std::stack<T*, std::list<T*> > m_InstanceStack;
};

template <class T>
std::stack<T*, std::list<T*> > CStackableSingleton<T>::m_InstanceStack;

#endif // PG_CLIENT_PROJECTG_COMMONUTIL_H
