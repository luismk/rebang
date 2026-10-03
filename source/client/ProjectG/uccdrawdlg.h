#pragma once

#include "frform.h"
#include "uccbrush.h"

struct sItemInfo;
class CUccClothes;

class FrUccDrawDlg : public FrForm
{
public:
	void SetName(const char* name);

	sItemInfo* m_pItemInfo;
	CUccClothes* m_pClothes[2];
};
