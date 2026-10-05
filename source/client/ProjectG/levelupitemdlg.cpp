#include "minatl.h"
#include "levelupitemdlg.h"
#include "projectg.h"
#include "wfont.h"
#include "wresrcmng.h"
#include "../../shared/sharedtables.h"

static __declspec(thread) int __rtti_obj;

extern Fresh* g_pFresh;

struct LevelUpGift
{
	int level;
	unsigned long tid;
	int count;
};

static const LevelUpGift s_levelUpGift[] = {
	{ 0,  0x0,        0     },
	{ 1,  0x0,        0     },
	{ 2,  0x18000005, 10    },
	{ 3,  0x1a000011, 20    },
	{ 4,  0x18000004, 10    },
	{ 5,  0x1a00000f, 5     },
	{ 6,  0x1a000010, 3000  },
	{ 7,  0x18000010, 5     },
	{ 8,  0x70000002, 18    },
	{ 9,  0x1a000028, 5     },
	{ 10, 0x1a000002, 5     },
	{ 11, 0x1a000010, 5000  },
	{ 12, 0x18000011, 10    },
	{ 13, 0x70000003, 18    },
	{ 14, 0x18000025, 10    },
	{ 15, 0x1a000002, 5     },
	{ 16, 0x1a000010, 10000 },
	{ 17, 0x7cc00003, 1     },
	{ 18, 0x1a00003d, 3     },
	{ 19, 0x18000025, 20    },
	{ 20, 0x1a000002, 5     },
	{ 21, 0x1a000033, 1     },
};

static const char*
	s_levelUpText[]
				 [3] = {
					 { "",																																																					 "",
                      K2L_Compatibility(
							 "\267\347\305\260E\267\316 \275\302\261\336\307\317\275\303\270\351 \275\305\260\346 \276\310\301\244 \272\270\301\266\301\246 10\260\263, \267\260\305\260 \306\316\276\337 \272\270\301\266\301\246 10\260\263\270\246 \265\345\267\301\277\344!")																																																																																					   },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \306\316\276\337\274\246\300\273 \275\261\260\324 \304\245 \274\366 \300\326\264\302\n\276\306\300\314\305\333\265\351\300\273 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\306\316\276\337\274\246\300\314 \276\356\267\301\277\357 \266\247\n\273\347\277\353\307\317\274\274\277\344!"),
                      K2L_Compatibility(
							 "\267\347\305\260D\267\316 \275\302\261\336\307\317\275\303\270\351 \271\314\266\363\305\254 \273\347\300\316 10\260\263\270\246 \265\345\267\301\277\344!")																																																																																																											   },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \271\314\266\363\305\254 \273\347\300\316 10\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\276\356\267\301\277\356 \306\333\306\303\277\241 \273\347\277\353\307\317\270\351\n\301\301\276\306\277\344!"),
                      K2L_Compatibility(
							 "\267\347\305\260C\267\316 \275\302\261\336\307\317\275\303\270\351 \303\274\267\302\272\270\301\266\301\246 10\260\263\270\246 \265\345\267\301\277\344!")																																																																																																												},
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \305\270\300\323\272\316\275\272\305\315 20\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\272\374\270\245 \301\370\307\340\300\273 \277\370\307\317\275\305\264\331\270\351 \307\312\274\366 \276\306\300\314\305\333\300\314\301\322!"),
                      K2L_Compatibility(
							 "\267\347\305\260B\267\316 \275\302\261\336\307\317\275\303\270\351 \303\274\267\302\272\270\301\266\301\246 10\260\263\270\246 \265\345\267\301\277\344!")																																																																																																												},
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \303\274\267\302 \272\270\301\266\301\246 10\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility("\306\257\274\366\274\246 \277\254\275\300\277\241 \265\265\277\362\300\314\n\265\307\261\346 \272\364\276\356\277\344!"),																																																																		   K2L_Compatibility("\267\347\305\260A\267\316 \275\302\261\336\307\317\275\303\270\351 \303\265\273\347\300\307\273\347\305\301 5\260\263\270\246 \265\345\267\301\277\344!")               },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \303\265\273\347\300\307 \273\347\305\301 5\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\272\361\261\342\263\312\261\356\301\366\n\276\363\270\266 \276\310 \263\262\276\322\276\356\277\344!"),
                      K2L_Compatibility(
							 "\272\361\261\342\263\312E\267\316 \275\302\261\336\307\317\275\303\270\351 3,000\306\316\300\273 \265\345\267\301\277\344!")																																																																																																																			  },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 3,000\306\316\300\273 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\272\361\261\342\263\312 \265\356\261\336\300\307 \273\365\267\316\277\356\n\276\306\300\314\305\333\265\351\300\273 \303\274\307\350\307\330 \272\270\274\274\277\344"),
                      K2L_Compatibility(
							 "\272\361\261\342\263\312D\267\316 \275\302\261\336\307\317\275\303\270\351 \303\274\267\302\276\310\301\244\301\246 5\260\263\270\246 \265\345\267\301\277\344!")																																																																																																										 },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \303\274\267\302\276\310\301\244\301\246 5\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\265\316\260\241\301\366 \264\311\267\302\300\314 \307\325\301\256\301\370 \276\306\300\314\305\333\300\273 \273\347\277\353\307\330 \272\270\274\274\277\344!"),
                      K2L_Compatibility(
							 "\272\361\261\342\263\312C\267\316 \275\302\261\336\307\317\275\303\270\351 \301\244\310\256\265\265\300\307 \271\335\301\366\270\246 \265\345\267\301\277\344!")																																																																																																										  },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \301\244\310\256\265\265 \271\335\301\366\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\306\316\276\337\274\246\300\273 \275\307\306\320\307\330\265\265 \271\256\301\246\276\370\276\356\277\344!"),
                      K2L_Compatibility(
							 "\272\361\261\342\263\312B\267\316 \275\302\261\336\307\317\275\303\270\351 \272\300\264\331\270\256\274\245\300\314\277\353\261\307 5\300\345\300\273 \265\345\267\301\277\344!")																																																																																																						 },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \272\300\264\331\270\256\274\245 \300\314\277\353\261\307\300\273\n5\300\345 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\267\271\276\356 \276\306\300\314\305\333\300\273\n\276\362\300\273 \274\366 \300\326\264\302 \261\342\310\270!"),
                      K2L_Compatibility(
							 "\272\361\261\342\263\312A\267\316 \275\302\261\336\307\317\275\303\270\351 \306\316 \270\266\275\272\305\315\270\256 5\260\263\270\246 \265\345\267\301\277\344!")																																																																																																										},
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \306\316 \270\266\275\272\305\315\270\256 5\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\301\326\264\317\276\356\261\356\301\366 \310\373 \263\273\274\274\277\344!"),
                      K2L_Compatibility(
							 "\301\326\264\317\276\356E\267\316 \275\302\261\336\307\317\275\303\270\351 5,000\306\316\300\273 \265\345\267\301\277\344!")																																																																																																																			  },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 5,000\306\316\300\273 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\301\326\264\317\276\356 \265\356\261\336\300\307 \273\365\267\316\277\356\n\276\306\300\314\305\333\265\351\300\273 \303\274\307\350\307\330 \272\270\274\274\277\344!"),
                      K2L_Compatibility(
							 "\301\326\264\317\276\356D\267\316 \275\302\261\336\307\317\275\303\270\351 \275\272\307\311\276\310\301\244\301\246 10\260\263\270\246 \265\345\267\301\277\344!")																																																																																																										},
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \275\272\307\311\276\310\301\244\301\246 10\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\265\316\260\241\301\366 \264\311\267\302\300\314 \307\325\301\256\301\370 \276\306\300\314\305\333\300\273 \273\347\277\353\307\330 \272\270\274\274\277\344!"),
                      K2L_Compatibility(
							 "\301\326\264\317\276\356C\267\316 \275\302\261\336\307\317\275\303\270\351 \275\272\307\311\300\307 \271\335\301\366\270\246 \265\345\267\301\277\344!")																																																																																																												  },
					 { K2L_Compatibility("\274\261\271\260\267\316 \275\272\307\311\300\307 \271\335\301\366\270\246 \265\345\270\263\264\317\264\331."),                                                                                      K2L_Compatibility("\264\365\277\355 \261\344 \306\304\277\366 \275\272\307\311\274\246\300\273\n\304\245 \274\366 \300\326\276\356\277\344!"),                                                K2L_Compatibility("\301\326\264\317\276\356B\267\316 \275\302\261\336\307\317\275\303\270\351 \303\274\267\302\272\270\303\346\301\246 10\260\263\270\246 \265\345\267\301\277\344!")      },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \303\274\267\302 \272\270\303\346\301\246 10\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\306\304\277\366 \306\257\274\366\274\246\300\273\n\275\261\260\324 \273\347\277\353\307\330 \272\270\274\274\277\344!"),
                      K2L_Compatibility(
							 "\301\326\264\317\276\356A\267\316 \275\302\261\336\307\317\275\303\270\351 \306\316 \270\266\275\272\305\315\270\256 5\260\263\270\246 \265\345\267\301\277\344!")																																																																																																										},
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \306\316 \270\266\275\272\305\315\270\256 5\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\270\326\260\355\265\265 \307\350\307\321 \275\303\264\317\276\356\300\307\n\261\346\267\316 \300\374\301\370!"),
                      K2L_Compatibility(
							 "\275\303\264\317\276\356E\267\316 \275\302\261\336\307\317\275\303\270\351 10,000\306\316\300\273 \265\345\267\301\277\344!")																																																																																																																			 },
					 { K2L_Compatibility("\274\261\271\260\267\316 10,000\306\316\300\273 \265\345\267\301\277\344!"),																														 K2L_Compatibility("\275\303\264\317\276\356 \265\356\261\336\300\307 \273\365\267\316\277\356\n\276\306\300\314\305\333\265\351\300\273 \303\274\307\350\307\330 \272\270\274\274\277\344!"), K2L_Compatibility("\275\303\264\317\276\356D\267\316 \275\302\261\336\307\317\275\303\270\351 \272\352\267\320\301\356\304\253\265\345\306\274\304\317\300\273 \265\345\267\301\277\344!") },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \272\352\267\320\301\356\304\253\265\345\306\274\304\317\300\273 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\304\253\265\345\310\246\270\257 \275\303\275\272\305\333\300\273 \303\274\307\350\307\330 \272\270\274\274\277\344!"),
                      K2L_Compatibility(
							 "\275\303\264\317\276\356C\267\316 \275\302\261\336\307\317\275\303\270\351 \275\272\305\251\267\241\304\241 \272\270\301\266\261\307 3\260\263\270\246 \265\345\267\301\277\344!")																																																																																																						},
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \275\272\305\251\267\241\304\241 \272\270\301\266\261\307\n3\300\345\300\273 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility("10\300\345\300\273 \270\360\300\270\270\351 \275\272\305\251\267\241\304\241 \304\253\265\345\270\246 \270\270\265\351 \274\366 \300\326\276\356\277\344"),																																																										 K2L_Compatibility("\275\303\264\317\276\356B\267\316 \275\302\261\336\307\317\275\303\270\351 \303\274\267\302\272\270\303\346\301\246 20\260\263\270\246 \265\345\267\301\277\344!")      },
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \303\274\267\302 \272\270\303\346\301\246 20\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\306\304\277\366 \306\257\274\366\274\246\300\273\n\273\347\277\353\307\330 \272\270\274\274\277\344!"),
                      K2L_Compatibility(
							 "\275\303\264\317\276\356A\267\316 \275\302\261\336\307\317\275\303\270\351 \306\316 \270\266\275\272\305\315\270\256 5\260\263\270\246 \265\345\267\301\277\344!")																																																																																																										},
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \306\316 \270\266\275\272\305\315\270\256 5\260\263\270\246 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\260\355\274\366\300\307 \261\346\300\314 \264\253\276\325\277\241!"),
                      K2L_Compatibility(
							 "\276\306\270\266\303\337\276\356E\267\316 \275\302\261\336\307\317\275\303\270\351 \275\272\305\251\267\241\304\241 \304\253\265\345\270\246 1\300\345 \265\345\267\301\277\344!")																																																																																																						},
					 { K2L_Compatibility(
						   "\274\261\271\260\267\316 \275\272\305\251\267\241\304\241 \304\253\265\345\270\246\n1\300\345 \265\345\270\263\264\317\264\331."),
                      K2L_Compatibility(
							 "\267\271\276\356 \276\306\300\314\305\333\300\314\n\263\252\277\300\261\346 \272\364\276\356\277\344!"),
                      K2L_Compatibility(
							 "\267\316 \275\302\261\336\307\317\275\303\270\351 \275\272\305\251\267\241\304\241 \304\253\265\345\270\246 1\300\345 \265\345\267\301\277\344!")																																																																																																														 },
};

IMPLEMENT_OBJECT(FrLevelupItemForm, FrForm)

BEGIN_FRESH_MSGMAP(FrLevelupItemForm, FrForm)

ON_FRESH_VI("levelupitem", FRCMD_INIT, FrLevelupItemForm::OnLevelUpItemInit)
ON_FRESH_VV("cancel", FRCMD_LBUTTONUP, FrLevelupItemForm::OnCloseUp)
ON_FRESH_VI("level", FRCMD_INIT, FrLevelupItemForm::OnLevelIcon)
ON_FRESH_VI("frame2", FRCMD_INIT, FrLevelupItemForm::OnFrame2Init)
ON_FRESH_VI("frame3", FRCMD_INIT, FrLevelupItemForm::OnFrame3Init)
ON_FRESH_VI("itemsend", FRCMD_INIT, FrLevelupItemForm::OnItemSendInit)
ON_FRESH_VI("leveluplist", FRCMD_INIT, FrLevelupItemForm::OnItemListInit)
ON_FRESH_VI("leveluplist", FRCMD_OWNERDRAW,
	FrLevelupItemForm::OnItemListOwnerDraw)
ON_FRESH_VI("itemexplane", FRCMD_INIT, FrLevelupItemForm::OnItemExplaneInit)

END_FRESH_MSGMAP()

FrLevelupItemForm::FrLevelupItemForm()
{
	m_pLevelIcon = NULL;
	m_pFrame2 = NULL;
	m_pFrame3 = NULL;
	m_pItemSend = NULL;
	m_pItemList = NULL;
	m_pItemExplane = NULL;
	m_level = 0;
	m_state = 0;
	m_itemTypeId = 0;
	m_itemTypeId2 = 1;
	m_bResult = false;
	static char* s_fontFile[] = { "[font_wind.jpg" };
	WTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = s_fontFile;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:";
	m_pFont = g_resrcmng->GetTitleFont();
	m_pFont->Create(&info);
}

FrLevelupItemForm::~FrLevelupItemForm()
{
	if (g_resrcmng && m_pFont)
	{
		g_resrcmng->Release(m_pFont);
		m_pFont = NULL;
	}
}

void FrLevelupItemForm::OnLevelUpItemInit(int param)
{
}

void FrLevelupItemForm::OnCloseUp()
{
	Close(FrNONE, true);
}

bool FrLevelupItemForm::Close(eFormRet ret, bool bSound)
{
	return FrForm::Close(ret, bSound);
}

void FrLevelupItemForm::OnLevelIcon(int param)
{
	m_pLevelIcon = DYNAMIC_CAST(FrArea, param);
}

void FrLevelupItemForm::OnFrame2Init(int param)
{
	m_pFrame2 = DYNAMIC_CAST(FrArea, param);
}

void FrLevelupItemForm::OnFrame3Init(int param)
{
	m_pFrame3 = DYNAMIC_CAST(FrArea, param);
}

void FrLevelupItemForm::OnItemSendInit(int param)
{
	m_pItemSend = DYNAMIC_CAST(FrEdit, param);
}

void FrLevelupItemForm::OnItemListInit(int param)
{
	m_pItemList = DYNAMIC_CAST(FrListBox, param);
	m_clockIcon.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "clock");
	m_clockIcon.rect = WRect(50.0f, 35.0f, (float)m_clockIcon.pBitmap->Width(),
		(float)m_clockIcon.pBitmap->Height());
	m_deleteIcon.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "delete");
	m_deleteIcon.rect =
		WRect(50.0f, 35.0f, (float)m_deleteIcon.pBitmap->Width(),
			(float)m_deleteIcon.pBitmap->Height());
}

void FrLevelupItemForm::OnItemListOwnerDraw(int param)
{
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!pGDI)
		return;
	FrListItem* pItem = (FrListItem*)param;
	if (!pItem)
		return;
	unsigned long* pTid = (unsigned long*)pItem->pData;
	if (!*pTid)
		return;
	IFF_ITEM_COMMON* pInfo = ItemManager()->FindCommonItem(*pTid);
	if (!pInfo)
		return;
	unsigned char timeFlag = pInfo->TimeFlag;
	const Bitmap* pBitmap =
		g_pFresh->GetManager()->GetBitmap("ITEMS", pInfo->Icon);
	if (!pBitmap)
		return;
	float x = 25.0f, y = 30.0f, countOffset = 0.0f;
	ItemManager()->GetCouponKind(*pTid);
	if (m_level > 1)
	{
		if ((*pTid & 0xfc000000) != 0x70000000)
		{
			x = 75.0f;
			y = 30.0f;
		}
		else
		{
			x = 55.0f;
			y = 0.0f;
		}
		countOffset += 55.0f;
	}
	WRect rect(x + pItem->pos.x + 18.0f, y + pItem->pos.y + 18.0f,
		(float)pBitmap->Width(), (float)pBitmap->Height());
	pGDI->DrawTexture(pBitmap, rect, 0xffffffff, 0);
	pGDI->SetTextColor(0xff000000, 0xffffffff);
	pGDI->SetTextStyle(4);
	if (timeFlag >= 2)
	{
		pGDI->DrawTexture(m_clockIcon.pBitmap,
			m_clockIcon.rect +
				WPoint(pItem->pos.x + 15.0f, pItem->pos.y + 15.0f),
			0xffffffff, 0);
		return;
	}
	int count;
	if (m_level == 1)
		count = 10;
	else
	{
		if (m_level >= 21)
			return;
		count = s_levelUpGift[m_level].count;
		if (count <= 1)
			return;
	}
	const char* text = MakeStr("%d", count);
	float textX = pItem->pos.x + 90.0f;
	for (count = 0; count < strlen(text); ++count)
		textX -= m_pFont->GetCharWidth(g_view, text[count] - '0');
	m_pFont->Print(g_view, textX + countOffset, pItem->pos.y + 75.0f, text, 0,
		0xffffffff, NULL);
}

void FrLevelupItemForm::OnItemExplaneInit(int param)
{
	m_pItemExplane = DYNAMIC_CAST(FrEdit, param);
}

bool FrLevelupItemForm::OnInit()
{
	if (m_pLevelIcon)
	{
		char name[32];
		sprintf(name, "level_%03d", m_level + 1);
		m_pLevelIcon->SetBgImg(name);
	}
	SetItemInfo();
	if (!m_state && m_bResult)
	{
		WSendPacket send((enumClientPacket)147);
		send.Send(TO_GAME);
	}
	return true;
}

void FrLevelupItemForm::SetItemInfo()
{
	if (m_level > 21)
	{
		m_pItemSend->AddText(s_levelUpText[21][0], false, true);
		m_pItemExplane->AddText(s_levelUpText[21][1], false, true);
	}
	else
	{
		m_pItemSend->AddText(s_levelUpText[m_level][0], false, true);
		m_pItemExplane->AddText(s_levelUpText[m_level][1], false, true);
	}
	if (m_level == 1)
	{
		m_itemTypeId = 0x18000008;
		m_itemTypeId2 = 0x18000007;
		m_pItemList->AddItem(&m_itemTypeId);
		m_pItemList->AddItem(&m_itemTypeId2);
	}
	else
	{
		if (m_level > 21)
			m_itemTypeId = 0x1a000033;
		else
			m_itemTypeId = s_levelUpGift[m_level].tid;
		m_pItemList->AddItem(&m_itemTypeId);
	}
	WRect rect = m_pFrame3->GetRect();
	m_pItemExplane->MoveWindow(WPoint(rect.x + 50.0f, rect.y + 15.0f));
	if (ItemManager()->GetCouponKind(s_levelUpGift[m_level].tid) == 1)
		SetDesc(
			"     \310\271\265\346 \276\306\300\314\305\333\300\272 \304\355\306\371\307\324\277\241\274\255 \310\256\300\316\307\317\274\274\277\344.");
	else
		SetDesc(
			"     \310\271\265\346 \276\306\300\314\305\333\300\272 \274\261\271\260\307\324\277\241\274\255 \310\256\300\316\307\317\274\274\277\344.");
}

void FrLevelupItemForm::OnProc(const float delta)
{
}

bool FrLevelupItemForm::SetData(sLevelUpDone data)
{
	m_level = data.bLevel;
	m_state = data.bItemType;
	m_bResult = data.bDone != 0;
	return true;
}
