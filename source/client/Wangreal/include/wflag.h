// Original: d:\build\custom\temp\code\client\wangreal\include\wflag.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WFLAG_H
#define PG_CLIENT_WANGREAL_INCLUDE_WFLAG_H

#include "..\..\defines.h"

// A set of bit flags. Layout and methods from the PDB; the bodies are the
// inline ones the line marks put on wflag.h:8-18.
class WFlags {
	LAYOUT_CHECK_FRIEND;
public:
	WFlags(void) : m_flag(0) {}                                    // wflag.h:8
	WFlags(const unsigned long& flag) : m_flag(flag) {}            // wflag.h:9

	void Set(unsigned long flag) { m_flag = flag; }                // wflag.h:11
	void Enable(unsigned long flag) { m_flag |= flag; }            // wflag.h:12
	void Disable(unsigned long flag) { m_flag &= ~flag; }          // wflag.h:13
	void Turn(unsigned long flag, bool on) {                       // wflag.h:14
		if (on == true) {
			m_flag |= flag;
		} else {
			m_flag &= ~flag;
		}
	}
	void Reset(void) { m_flag = 0; }                               // wflag.h:15
	bool GetFlag(unsigned long flag) const { return (m_flag & flag) ? true : false; }   // wflag.h:16
	unsigned long Get(void) const { return m_flag; }
	operator unsigned long(void) const { return m_flag; }          // wflag.h:18

protected:
	unsigned long m_flag;   // 0x0
};

CHECK_SIZE(WFlags, 0x4);
BEGIN_LAYOUT_CHECK(WFlags)
	CHECK_FIELD(WFlags, m_flag, 0x0);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WFLAG_H
