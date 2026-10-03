#pragma once

#include "frform.h"

class FrEdit;
class FrJukeBoxDlg : public FrForm
{
	DECLARE_OBJECT(FrJukeBoxDlg)

	FrJukeBoxDlg();
	virtual ~FrJukeBoxDlg();

	static void Init();
	static void Preserve();
	static void Restore();
	static void PlayWithMap(unsigned char map);

	static bool m_preserve_isPlaying;

protected:
	virtual bool OnInit();
	virtual void OnProc(const float delta);

	void OnTitleInit(int param);
	void OnPlayInit(int param);
	void OnCloseBtnUp();
	void OnPlayBtnUp();
	void OnStopBtnUp();
	void OnPrevBtnUp();
	void OnNextBtnUp();
	void OnChangeTitle();
	void OnChangePlayPauseButton();

	static int m_curBGM;

	FrEdit* m_pTitle;
	FrButton* m_pPlay;

	DECLARE_FRESH_MSGMAP()
};
