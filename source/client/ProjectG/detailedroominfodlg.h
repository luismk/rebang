#pragma once

#include "frform.h"

struct sRoomUserInfo;
#include <list>

#include "../../shared/globalgamedefine.h"

class FrDtRoomDlg : public FrForm
{
	DECLARE_OBJECT(FrDtRoomDlg)

	enum eHeader
	{
		eHEADER_LEVEL,
		eHEADER_ID,
		eHEADER_STATUS,
	};

	FrDtRoomDlg();

	virtual bool OnInit();
	virtual void OnProc(const float dt);

	void SetRoomInfo(const sRoomInfo& info);
	void EnableJoinBtn(bool bEnable);

protected:
	void OnGameDescInit(int param);
	void OnJoinBtnInit(int param);
	void OnJoinBtnUp();
	void OnUserInfoBtnInit(int param);
	void OnUserInfoBtnUp();
	void OnGalleryBtnInit(int param);
	void OnGalleryBtnUp();
	void OnLevelHdrInit(int param);
	void OnLevelHdrDown();
	void OnIdHdrInit(int param);
	void OnIdHdrDown();
	void OnStatusHdrInit(int param);
	void OnStatusHdrDown();
	void OnHeaderLBtnDown(eHeader header);
	void OnUserListInit(int param);
	void OnUserListOwnerDraw(int param);
	void OnUserListLBtnDown();
	void OnUserListRBtnUp();
	bool OnPasswordDlgResult(int result, FrForm* form);

	std::list<void*> m_delList;
	FrEdit* m_pGameDesc;
	FrButton* m_pJoinBtn;
	FrButton* m_pUserInfoBtn;
	FrButton* m_pGalleryBtn;
	FrButton* m_pLevelHdr;
	FrButton* m_pIdHdr;
	FrButton* m_pStatusHdr;
	FrListBox* m_pUserList;
	sRoomInfo m_roomInfo;
	sRoomUserInfo* m_pSelUser;
	eHeader m_sortHeader;
	const Bitmap* m_pIcon[10];

	DECLARE_FRESH_MSGMAP()
};
