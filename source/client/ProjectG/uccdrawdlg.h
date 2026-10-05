#pragma once

#include "frform.h"
#include "uccbrush.h"

struct sItemInfo;
class CUccClothes;

class FrUccDrawDlg : public FrForm
{
public:
	void SetName(const char* name);
	void SetCurrentColor(unsigned char r, unsigned char g, unsigned char b);

	sItemInfo* m_pItemInfo;
	CUccClothes* m_pClothes[2];
	bool m_bWaiting;
	FrForm* m_pWaitingForm;
	float m_waitingTime;
	Bitmap m_color[18];
};
