#pragma once

#include "frform.h"
#include "../../shared/globalgamedefine.h"

enum eHoleType;
class FrViewer;
class FrTreasureCourse;

class FrChangeRoomInfoDlg : public FrForm
{
	DECLARE_OBJECT(FrChangeRoomInfoDlg)

	FrChangeRoomInfoDlg();
	virtual ~FrChangeRoomInfoDlg();

	virtual bool OnInit();
	virtual void OnProc(const float dt);

protected:
	void OnTextInit(int param);
	void OnTitleInit(int param);
	void OnPasswordInit(int param);
	void OnMapPrevInit(int param);
	void OnMapPrevUp();
	void OnMapNextInit(int param);
	void OnMapNextUp();
	void OnGameTypeInit(int param);
	void DrawGameTypeButton(FrGraphicInterface* gi, int type, WRect rect,
		bool bEnable);
	void OnGameTypeOwnerDraw(int param);
	void OnUserLimitInit(int param);
	void DrawUserLimitButton(FrGraphicInterface* gi, int limit, WRect rect);
	void OnUserLimitOwnerDraw(int param);
	void OnHoleInit(int param);
	void DrawHoleButton(FrGraphicInterface* gi, int hole, WRect rect);
	void OnHoleOwnerDraw(int param);
	void OnTimeLimitInit(int param);
	void DrawTimeLimitButton(FrGraphicInterface* gi, int time, WRect rect);
	void OnTimeLimitOwnerDraw(int param);
	void OnCourseInit(int param);
	bool OnMapSelectDlgResult(int result, FrForm* form);
	void OnCourseUp();
	void OnCourseOwnerDraw(int param);
	void OnHoleTypeInit(int param);
	void DrawHoleTypeButton(FrGraphicInterface* gi, eHoleType type, WRect rect,
		bool bEnable);
	void OnHoleTypeOwnerDraw(int param);
	void OnOkBtnUp();
	void ChangeGameType(int type, bool bUnused, bool bKeepValues);
	void ChangeUserLimit(int limit);
	void ChangeHole(int hole);
	void ChangeTimeLimit(int time);
	void ChangeCourse(int course);
	void ChangeHoleType(eHoleType type);
	bool IsBtnEnable();

	FrEdit* m_pTitle;
	FrEdit* m_pPassword;
	FrArea* m_pText;
	FrWnd* m_pMapCtrl;
	FrButton* m_pMapPrev;
	FrButton* m_pMapNext;
	FrArea* m_pGameType;
	FrArea* m_pUserLimit;
	FrArea* m_pHole;
	FrArea* m_pTimeLimit;
	FrViewer* m_pCourse;
	FrArea* m_pHoleType;
	const Bitmap* m_pSelectBtnN;
	const Bitmap* m_pSelectBtnO;
	const Bitmap* m_pSelectBtnD;
	const Bitmap* m_pHoleTypeBtn[4][2];
	const Bitmap* m_pMapSelectO;
	FrTreasureCourse* m_pMapSelectDlg;
	int m_gameTypeGroup;
	int m_gameType;
	int m_userLimit;
	int m_timeLimit;
	int m_hole;
	int m_course;
	unsigned char m_holeType;
	unsigned long m_gameTime;
	unsigned long m_shotTime;
	float m_btnDelay;
	sRoomInfo m_roomInfo;

	DECLARE_FRESH_MSGMAP()
};

namespace
{
	struct _invalidPwd
	{
		bool bInvalid;

		_invalidPwd()
			: bInvalid(false)
		{
		}
		void operator()(char c)
		{
			if (c == ' ')
				bInvalid = true;
		}
	};
}
