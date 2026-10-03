#pragma once

#include <string>
#include "frform.h"
#include "standinginline.h"

class FrFriendDlg;
class FrButton;
class FrListBox;
class FrEdit;
class FrEmoticonDlg;
class CPost_MyItemList;
class WTitleFont;
class Bitmap;

class CPost_SendDlg : public FrForm
{
	friend class FrFriendDlg;

	DECLARE_OBJECT(CPost_SendDlg)

protected:
	FrFriendDlg* m_pFriendDlg;
	FrButton* m_pFriendList;
	FrButton* m_pEmoticonBtn;
	FrButton* m_pItemAdd;
	FrEmoticonDlg* m_pEmoticonDlg;
	CPost_MyItemList* m_pMyItemList;
	FrListBox* m_pPostList;
	FrEdit* m_pServiceCharge;
	std::string m_receiverNick;
	std::string m_receiverId;
	unsigned long m_receiverUid;
	const Bitmap* m_pUccOriginal;
	const Bitmap* m_pUccCopied;
	const Bitmap* m_pUccTempSaved;
	WTitleFont* m_pTitleFont;
	FrEdit* m_pUserNick;
	FrEdit* m_pReciveUser;
	// TODO: this class definition is incomplete
};
