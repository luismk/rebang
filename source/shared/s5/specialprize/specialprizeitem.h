// Original: d:\build\custom\temp\code\shared\s5\specialprize\specialprizeitem.h
#ifndef PG_SHARED_S5_SPECIALPRIZE_SPECIALPRIZEITEM_H
#define PG_SHARED_S5_SPECIALPRIZE_SPECIALPRIZEITEM_H

#include "..\..\defines.h"

class SpecialPrizeItem {
	LAYOUT_CHECK_FRIEND;

public:
	SpecialPrizeItem();
	virtual ~SpecialPrizeItem();
	bool getflag();
	void changeflag();
	float getvalue();
	void setvalue(float value);
	void InitalizeItem();

private:
	bool m_flag;   // 0x4
	float m_value; // 0x8
};

CHECK_SIZE(SpecialPrizeItem, 0xc);
BEGIN_LAYOUT_CHECK(SpecialPrizeItem)
	CHECK_FIELD(SpecialPrizeItem, m_flag, 0x4);
	CHECK_FIELD(SpecialPrizeItem, m_value, 0x8);
END_LAYOUT_CHECK

// Avatar_Main.cpp and Avatar_Avatar.cpp define these macros empty before their
// includes: the original holds the out-of-line copies of these inline
// accessors in their objects (getflag, changeflag, setvalue in Avatar_Main;
// getvalue in Avatar_Avatar); every other unit inlines them.
#ifndef AVATAR_MAIN_SPECIALPRIZEITEM_INLINE
#define AVATAR_MAIN_SPECIALPRIZEITEM_INLINE inline
#endif
#ifndef AVATAR_AVATAR_SPECIALPRIZEITEM_INLINE
#define AVATAR_AVATAR_SPECIALPRIZEITEM_INLINE inline
#endif

// specialprizeitem.h:11
AVATAR_MAIN_SPECIALPRIZEITEM_INLINE bool SpecialPrizeItem::getflag() {
	return m_flag;
}

// specialprizeitem.h:12
AVATAR_MAIN_SPECIALPRIZEITEM_INLINE void SpecialPrizeItem::changeflag() {
	m_flag = !m_flag;
}

// specialprizeitem.h:14
AVATAR_AVATAR_SPECIALPRIZEITEM_INLINE float SpecialPrizeItem::getvalue() {
	return m_value;
}

// specialprizeitem.h:15
AVATAR_MAIN_SPECIALPRIZEITEM_INLINE void SpecialPrizeItem::setvalue(float value) {
	m_value = value;
}

#endif
