#pragma once

#include "frform.h"

class FrButton;
class FrLogoutDlg : public FrForm
{
	DECLARE_OBJECT(FrLogoutDlg)
	FrLogoutDlg();

	void DisableServerSelect();

protected:
	virtual void OnProc(const float dt);

	void OnServerBtnInit(int param);
	void OnServerBtnUp();
	void OnLogoutBtnInit(int param);
	void OnLogoutBtnUp();
	void OnExitBtnInit(int param);
	void OnExitBtnUp();

	FrButton* m_pServerBtn;
	FrButton* m_pLogoutBtn;
	FrButton* m_pExitBtn;
	FrButton* m_pDescBtn;

	DECLARE_FRESH_MSGMAP()
};
