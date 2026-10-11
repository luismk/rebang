// Original: d:\build\custom\temp\code\client\projectg\guilddlg.h
#ifndef PG_CLIENT_PROJECTG_GUILDDLG_H
#define PG_CLIENT_PROJECTG_GUILDDLG_H

#include "..\..\defines.h"
#include "frform.h"

class FrEdit;
class FrListBox;
class TiXmlDocument;
struct FrListItem;

// One RSS entry of the guild board: the data of a board list item. Both
// strings are new[]'d (UnloadRSS frees them and the entry).
struct sGuildRSSItem {
	char* title;         // 0x0
	char* description;   // 0x4 (shown in the "view" edit when selected)
};

CHECK_SIZE(sGuildRSSItem, 0x8);
CHECK_FIELD(sGuildRSSItem, title, 0x0);
CHECK_FIELD(sGuildRSSItem, description, 0x4);

// Guild dialog: an RSS board list and a text view of the selected entry;
// its size (0x118) is the `new` of FrGuildDlgMakeInstance.
class FrGuildDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // guilddlg.h:16

	FrGuildDlg(void);
	virtual ~FrGuildDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);

	void OnBoardInit(intptr_t param);
	void OnBoardOwnerDraw(intptr_t param);
	void OnBoardBtnDown(void);
	void OnBoardBtnUp(void);
	void OnViewInit(intptr_t param);
	void OnViewBtnUp(void);

	bool DownLoad(const char* fileName, const char* url, const char* localDir, const char* urlBase);
	void ParseRSS(TiXmlDocument& doc);
	void LoadRSS(const char* fileName);
	void UnloadRSS(void);
	void SelectTitle(FrListItem* pItem);

private:
	FrListBox* m_pBoard;   // 0x110 ("board")
	FrEdit* m_pView;       // 0x114 ("view")
};

CHECK_SIZE(FrGuildDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrGuildDlg)
	CHECK_FIELD(FrGuildDlg, m_pBoard, 0x110);
	CHECK_FIELD(FrGuildDlg, m_pView, 0x114);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GUILDDLG_H
