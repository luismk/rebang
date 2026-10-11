// Original: d:\build\custom\temp\code\client\projectg\jukeboxdlg.h
#ifndef PG_CLIENT_PROJECTG_JUKEBOXDLG_H
#define PG_CLIENT_PROJECTG_JUKEBOXDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrEdit;
class FrButton;

// The lounge's jukebox dialog: the title edit, the play/pause button and
// the prev/next/stop/close buttons. The current track is class-wide; its
// size (0x118) is the `new` of FrJukeBoxDlgMakeInstance.
class FrJukeBoxDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // jukeboxdlg.h:8

	FrJukeBoxDlg(void);
	virtual ~FrJukeBoxDlg(void);

	static void Init(void);
	static void Preserve(void);
	static void Restore(void);
	static void PlayWithMap(unsigned char map);

	// Whether the jukebox track was playing when Preserve stopped it.
	static bool m_preserve_isPlaying;

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void OnTitleInit(intptr_t param);
	void OnPlayInit(intptr_t param);
	void OnCloseBtnUp(void);
	void OnPlayBtnUp(void);
	void OnStopBtnUp(void);
	void OnPrevBtnUp(void);
	void OnNextBtnUp(void);
	void OnChangeTitle(void);
	void OnChangePlayPauseButton(void);

	// Index of the current track in the jukebox's track list (0..23).
	static int m_curBGM;

private:
	FrEdit* m_pTitle;     // 0x110 ("jukeboxtitle")
	FrButton* m_pPlay;    // 0x114 ("jukeboxplay")
};

CHECK_SIZE(FrJukeBoxDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrJukeBoxDlg)
	CHECK_FIELD(FrJukeBoxDlg, m_pTitle, 0x110);
	CHECK_FIELD(FrJukeBoxDlg, m_pPlay, 0x114);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_JUKEBOXDLG_H
