#pragma once

#include <list>
#include "frform.h"

class FrListBox;
class FrStatic;

class FrFortuneOpenDlg : public FrForm
{
	DECLARE_OBJECT(FrFortuneOpenDlg)

	FrFortuneOpenDlg();
	virtual ~FrFortuneOpenDlg();

	void SetItemList(unsigned long* items, int num);
	void SetItemNewList(unsigned long* items, int num);
	void SetTypeId(unsigned long typeId);
	unsigned long GetTypeId() { return m_typeId; }

protected:
	void OnItemListInit(int param);
	void OnItemListOwnerDraw(int param);
	void OnTotalPriceInit(int param);
	void OnDescItemsInit(int param);

	struct sFortune
	{
		unsigned long typeId;
		short count;

		bool operator==(const unsigned long& _typeId) const
		{
			return typeId == _typeId;
		}
	};

	unsigned long m_reserved;
	FrListBox* m_pItemList;
	FrStatic* m_pTotalPrice;
	FrStatic* m_pDescItems;
	unsigned long m_typeId;
	std::list<sFortune> m_fortuneList;

	DECLARE_FRESH_MSGMAP()
};
