#pragma once

#include "frform.h"

class FrButton;
class FrListBox;
class FrStatic;
class FrEdit;

class FrGiftDlg : public FrForm
{
	friend class FrFriendDlg;

	DECLARE_OBJECT(FrGiftDlg)

protected:
	FrButton* m_pCheckout;
	FrButton* m_pFriendBtn;
	FrListBox* m_pCart;
	FrStatic* m_pSxId;
	FrEdit* m_pEdId;
	FrButton* m_pBtIdCheck;
	FrStatic* m_pSxNick;
	FrEdit* m_pEdNick;
	FrButton* m_pBtNickCheck;
	FrEdit* m_pEdConfirm;
	// TODO: this class definition is incomplete
};
