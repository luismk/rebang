// Original: d:\build\custom\temp\code\client\projectg\guildnoticedlg.h
#ifndef PG_CLIENT_PROJECTG_GUILDNOTICEDLG_H
#define PG_CLIENT_PROJECTG_GUILDNOTICEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrEdit;
class NewsReader;

// Guild notice dialog: pages through the articles of a NewsReader, showing
// "index / count" and the current article, with prev/next/close buttons.
// Its size (0x130) is the `new` of FrGuildNoticeDlgMakeInstance.
class FrGuildNoticeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // guildnoticedlg.h:13

	FrGuildNoticeDlg(void);
	virtual ~FrGuildNoticeDlg(void);

	void DataBind(const NewsReader* pNewsReader, unsigned int index);
	void Prev(void);
	void Next(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void SetArticle(unsigned int index);

	void OnClose_Init(intptr_t param);
	void OnIndex_Init(intptr_t param);
	void OnArticle_Init(intptr_t param);
	void OnPrev_Init(intptr_t param);
	void OnNext_Init(intptr_t param);
	void OnClose_LBtnUp(void);
	void OnPrev_LBtnUp(void);
	void OnNext_LBtnUp(void);

private:
	FrButton* m_pClose;                   // 0x110 (OnClose_Init)
	FrEdit* m_pIndex;                     // 0x114 (OnIndex_Init, "index / count")
	FrEdit* m_pArticle;                   // 0x118 (OnArticle_Init, the article text)
	FrButton* m_pPrev;                    // 0x11c (OnPrev_Init)
	FrButton* m_pNext;                    // 0x120 (OnNext_Init)
	const NewsReader* m_pNewsReader;      // 0x124 (DataBind)
	unsigned int m_curArticle;            // 0x128 (SetArticle; -1 when none)
	UNKNOWN(0x12c, 0x130);
};
BEGIN_LAYOUT_CHECK(FrGuildNoticeDlg)
	CHECK_FIELD(FrGuildNoticeDlg, m_pClose, 0x110);
	CHECK_FIELD(FrGuildNoticeDlg, m_pIndex, 0x114);
	CHECK_FIELD(FrGuildNoticeDlg, m_pArticle, 0x118);
	CHECK_FIELD(FrGuildNoticeDlg, m_pPrev, 0x11c);
	CHECK_FIELD(FrGuildNoticeDlg, m_pNext, 0x120);
	CHECK_FIELD(FrGuildNoticeDlg, m_pNewsReader, 0x124);
	CHECK_FIELD(FrGuildNoticeDlg, m_curArticle, 0x128);
END_LAYOUT_CHECK
CHECK_SIZE(FrGuildNoticeDlg, 0x130);

#endif // PG_CLIENT_PROJECTG_GUILDNOTICEDLG_H
