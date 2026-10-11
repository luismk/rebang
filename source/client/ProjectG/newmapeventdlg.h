// Original: d:\build\custom\temp\code\client\projectg\newmapeventdlg.h
#ifndef PG_CLIENT_PROJECTG_NEWMAPEVENTDLG_H
#define PG_CLIENT_PROJECTG_NEWMAPEVENTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrArea;

// The new-map event window: a caption, an info line and six score areas, each
// drawn with a "5under"/"10under"... picture of the NEWMAP_EVENT bitmap
// group. Its size (0x130) is the `new` of FrNewMapEventDlgMakeInstance.
class FrNewMapEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // newmapeventdlg.h:8

	FrNewMapEventDlg(void);
	virtual ~FrNewMapEventDlg(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnCaptionInit(intptr_t param);
	void OnInfo1Init(intptr_t param);
	void OnScore0Init(intptr_t param);
	void OnScore1Init(intptr_t param);
	void OnScore2Init(intptr_t param);
	void OnScore3Init(intptr_t param);
	void OnScore4Init(intptr_t param);
	void OnScore5Init(intptr_t param);
	void OnScore0OwnerDraw(void);
	void OnScore1OwnerDraw(void);
	void OnScore2OwnerDraw(void);
	void OnScore3OwnerDraw(void);
	void OnScore4OwnerDraw(void);
	void OnScore5OwnerDraw(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

	FrArea* m_pScore[6];    // 0x110 (OnScore0Init..OnScore5Init)
	UNKNOWN(0x128, 0x130);
};

CHECK_SIZE(FrNewMapEventDlg, 0x130);
BEGIN_LAYOUT_CHECK(FrNewMapEventDlg)
	CHECK_FIELD(FrNewMapEventDlg, m_pScore, 0x110);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NEWMAPEVENTDLG_H
