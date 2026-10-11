// Original: d:\build\custom\temp\code\client\projectg\fortunedlg.h
#ifndef PG_CLIENT_PROJECTG_FORTUNEDLG_H
#define PG_CLIENT_PROJECTG_FORTUNEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrArea;
class FrStatic;
struct IFF_ITEM_COMMON;

// Fortune dialog: the portrait, name, level and description of a fortune
// item. Its size (0x128) is the `new` of FrFortuneDlgMakeInstance.
class FrFortuneDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // fortunedlg.h:9

	FrFortuneDlg(void);
	virtual ~FrFortuneDlg(void);

	unsigned long GetTypeId(void);   // fortunedlg.h:16
	void SetTypeId(unsigned long typeId, IFF_ITEM_COMMON* pItem);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnPortraitInit(intptr_t param);
	void OnDescNameInit(intptr_t param);
	void OnDescLevelInit(intptr_t param);
	void OnDescDescInit(intptr_t param);

private:
	UNKNOWN(0x110, 0x114);
	unsigned long m_typeId;          // 0x114
	FrArea* m_pPortrait;             // 0x118
	FrStatic* m_pDescName;           // 0x11c
	FrStatic* m_pDescLevel;          // 0x120
	FrStatic* m_pDescDesc;           // 0x124
};
BEGIN_LAYOUT_CHECK(FrFortuneDlg)
	CHECK_FIELD(FrFortuneDlg, m_typeId, 0x114);
	CHECK_FIELD(FrFortuneDlg, m_pPortrait, 0x118);
	CHECK_FIELD(FrFortuneDlg, m_pDescName, 0x11c);
	CHECK_FIELD(FrFortuneDlg, m_pDescLevel, 0x120);
	CHECK_FIELD(FrFortuneDlg, m_pDescDesc, 0x124);
END_LAYOUT_CHECK
CHECK_SIZE(FrFortuneDlg, 0x128);

#endif // PG_CLIENT_PROJECTG_FORTUNEDLG_H
