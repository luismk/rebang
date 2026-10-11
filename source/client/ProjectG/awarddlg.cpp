// Original: d:\build\custom\temp\code\client\projectg\awarddlg.cpp
#include "AwardDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frstatic.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "SharedDoc.h"
#include "NetResourceManager.h"
#include "ProjectG.h"
#include "Intrusion.h"

// awarddlg.cpp:17 (run-time type and factory registration of FrAwardDlg)
const WRTTI FrAwardDlg::m_RTTI("FrAwardDlg", &FrForm::m_RTTI);

IObject* FrAwardDlgMakeInstance(void) {
	return new FrAwardDlg;
}

struct __sFrAwardDlg {
	__sFrAwardDlg(void) {
		ObjectFactory().AddObjectFunctor(FrAwardDlgMakeInstance, "FrAwardDlg");
	}
};
static __sFrAwardDlg s_registerFrAwardDlg;

// awarddlg.cpp:19
const sFRESH_MSGMAP* FrAwardDlg::GetMessageMap(void) const {
	return &FrAwardDlg::_MsgMap;
}

// Define the message entries and message map for FrAwardDlg so the linker
// can resolve the static symbols referenced by callers. Follow the pattern
// used by other dialogs: an array of sFRESH_ENTRY terminated with a zero
// entry and a sFRESH_MSGMAP pointing to the parent form's map and entries.
// These are minimal stubs; actual message bindings should be filled in
// later if runtime handlers are needed.
// Note: FrForm::_MsgMap is defined elsewhere.

sFRESH_ENTRY FrAwardDlg::_MsgEntries[] = {
	// Minimal terminator entry: name=null, command=FRCMD_NONE, handler type=FrFuncNone
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

sFRESH_MSGMAP FrAwardDlg::_MsgMap = { &FrForm::_MsgMap, &FrAwardDlg::_MsgEntries[0] };

// awarddlg.cpp:55
void FrAwardDlg::OnGoldStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);

	if (pStatic) {
		int count = 0;

		for (int i = 0; i < 6; i++) {
			unsigned long uid = WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[i].m_uid;

			if (uid != 0xFFFFFFFF) {
				if (GetMedalIndex(uid) == 0)
					count++;
			}
		}

		if (count == 0)
			pStatic->SetVisible(false);

		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
			pStatic->SetCaption("\xB1\xE6\xB5\xE5\xB1\xE2\xBF\xA9\xB5\xB5 1\xC0\xA7 :");
		} else if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
			pStatic->SetCaption("1\xC0\xA7 :");
		}
	}
}

// awarddlg.cpp:84
void FrAwardDlg::OnSilverStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);

	if (pStatic) {
		int count = 0;

		for (int i = 0; i < 6; i++) {
			unsigned long uid = WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[i].m_uid;

			if (uid != 0xFFFFFFFF) {
				int medal = GetMedalIndex(uid);

				if (medal == 0)
					count++;
				else if (medal == 1)
					count += 10;
			}
		}

		if (count > 10) {
			WRect rect;
			pStatic->GetClientRect(rect);

			int line = count % 10;
			rect.y += line * 25.0f;
			pStatic->SetClientRect(rect);
		} else {
			pStatic->SetVisible(false);
		}

		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
			pStatic->SetCaption("\xB1\xE6\xB5\xE5\xB1\xE2\xBF\xA9\xB5\xB5 2\xC0\xA7 :");
		} else if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
			pStatic->SetCaption("2\xC0\xA7 :");
		}
	}
}

// awarddlg.cpp:128
void FrAwardDlg::OnBronzeStaticInit(int param) {
	FrStatic* pStatic = DYNAMIC_CAST(FrStatic, param);

	if (pStatic) {
		int count = 0;

		for (int i = 0; i < 6; i++) {
			unsigned long uid = WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[i].m_uid;

			if (uid != 0xFFFFFFFF) {
				int medal = GetMedalIndex(uid);

				if (medal == 2)
					count++;
				else if (medal <= 1)
					count += 10;
			}
		}

		if (count > 10) {
			WRect rect;
			pStatic->GetClientRect(rect);

			int line = count % 10;
			rect.y += line * 25.0f;
			pStatic->SetClientRect(rect);
		} else {
			pStatic->SetVisible(false);
		}

		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
			pStatic->SetCaption("\xB1\xE6\xB5\xE5\xB1\xE2\xBF\xA9\xB5\xB5 3\xC0\xA7 :");
		} else if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
			pStatic->SetCaption("3\xC0\xA7 :");
		}
	}
}

// awarddlg.cpp:186
int FrAwardDlg::GetMedalIndex(unsigned long uid) {
	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(uid);

		if (index == 0xFF)
			return 3;

		sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

		for (int i = 0; i < 6; i++) {
			if (WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[i].m_uid == info.m_uid)
				return info.m_rank;
		}
	} else if (WSingleton<CSharedDoc>::Instance()->m_roomType == 6) {
		if (WSingleton<CSharedDoc>::Instance()->m_gameResultList.size() < 6)
			return 3;

		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(uid);

		if (index == 0xFF || WSingleton<CSharedDoc>::Instance()->m_gameResultList[index].m_characterType > 3)
			return 3;

		unsigned long playerUID = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index].m_uid;

		for (int i = 0; i < 6; i++) {
			if (WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[i].m_uid == playerUID) {
				if (i <= 0)
					return 0;
				else if (i < 3)
					return 1;

				return i < 6 ? 2 : 3;
			}
		}
	} else {
		unsigned char memberSize = 0;

		if (IsLocalContent((localContentType_t)0x2E)) {
			if (WSingleton<CSharedDoc>::Instance()->m_roomType == 4)
				memberSize = WSingleton<CIntrusion>::Instance()->GetCalcExpMemberSize();
			else
				memberSize = 0;
		}

		if (memberSize == 0) {
			if (WSingleton<CSharedDoc>::Instance()->m_gameResultList.size() < 10)
				return 3;
		} else {
			if (memberSize < 10)
				return 3;
		}

		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(uid);

		if (index == 0xFF || WSingleton<CSharedDoc>::Instance()->m_gameResultList[index].m_characterType > 3)
			return 3;

		unsigned char rank = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index].m_rank;
		unsigned char playerCount = 0;
		unsigned char skipped = 0;
		std::vector<sRivalData>::iterator it;

		if (memberSize == 0) {
			for (it = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {
				if (it->m_state == 3 && GetHoleIndex(it->m_hole) < 4)
					continue;

				playerCount++;

				if (it->m_state != 3 && it->m_rank < rank && it->m_characterType > 3)
					skipped++;
			}
		} else {
			playerCount = memberSize;

			for (it = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {
				if (it->m_state != 3 && it->m_rank < rank && it->m_characterType > 3)
					skipped++;
			}
		}

		rank -= skipped;

		if (WSingleton<CSharedDoc>::Instance()->m_holes == 9) {
			if (playerCount <= 14) {
				return 3;
			} else if (playerCount <= 18) {
				if (rank == 1)
					return 2;
			} else if (playerCount <= 22) {
				if (rank == 1)
					return 1;
			} else if (playerCount <= 26) {
				if (rank == 1)
					return 1;
				else if (rank == 2)
					return 2;
			} else {
				if (rank == 1)
					return 0;
				else if (rank == 2)
					return 1;
			}
		} else if (WSingleton<CSharedDoc>::Instance()->m_holes == 18) {
			if (playerCount <= 14) {
				if (rank == 1)
					return 2;
			} else if (playerCount <= 18) {
				if (rank == 1)
					return 1;
				else if (rank == 2)
					return 2;
			} else if (playerCount <= 22) {
				if (rank == 1)
					return 0;
				else if (rank == 2)
					return 1;
				else if (rank == 3)
					return 2;
			} else if (playerCount <= 26) {
				if (rank == 1)
					return 0;
				else if (rank == 2)
					return 1;
				else if (rank <= 4)
					return 2;
			} else {
				if (rank == 1)
					return 0;
				else if (rank <= 3)
					return 1;
				else if (rank <= 6)
					return 2;
			}
		}
	}

	return 3;
}

// awarddlg.cpp:395
void FrAwardDlg::OnWinner1Init(int param) {
	m_pWinnerArea[0] = DYNAMIC_CAST(FrArea, param);
	m_pWinnerArea[0]->m_nFlags.Enable(0x1000);
}

// awarddlg.cpp:401
void FrAwardDlg::OnWinner1OwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[0].m_uid);

		if (index == 0xFF)
			return;

		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[0].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

		if (info.m_hasEmblem) {
			const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

			if (emblem) {
				gdi->DrawTexture(emblem, WRect(m_pWinnerArea[0]->GetRect().x, m_pWinnerArea[0]->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
				offset = 27.0f;
			}
		}

		gdi->SetTextColor(color, 0xFFFFFFFF);
		gdi->SetTextStyle(0);
		g_pFresh->GetManager()->PrintText(WPoint(m_pWinnerArea[0]->GetRect().x + offset, m_pWinnerArea[0]->GetRect().y), 0, info.m_nickname, -1.0f, 0xFFFFFFFF);
	}
}

// awarddlg.cpp:441
void FrAwardDlg::OnWinner2Init(int param) {
	m_pWinnerArea[1] = DYNAMIC_CAST(FrArea, param);
	m_pWinnerArea[1]->m_nFlags.Enable(0x1000);
}

// awarddlg.cpp:447
void FrAwardDlg::OnWinner2OwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[1].m_uid);

		if (index == 0xFF)
			return;

		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[1].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

		if (info.m_hasEmblem) {
			const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

			if (emblem) {
				gdi->DrawTexture(emblem, WRect(m_pWinnerArea[1]->GetRect().x, m_pWinnerArea[1]->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
				offset = 27.0f;
			}
		}

		gdi->SetTextColor(color, 0xFFFFFFFF);
		gdi->SetTextStyle(0);
		g_pFresh->GetManager()->PrintText(WPoint(m_pWinnerArea[1]->GetRect().x + offset, m_pWinnerArea[1]->GetRect().y), 0, info.m_nickname, -1.0f, 0xFFFFFFFF);
	}
}

// awarddlg.cpp:485
void FrAwardDlg::OnWinner3Init(int param) {
	m_pWinnerArea[2] = DYNAMIC_CAST(FrArea, param);
	m_pWinnerArea[2]->m_nFlags.Enable(0x1000);
}

// awarddlg.cpp:491
void FrAwardDlg::OnWinner3OwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[2].m_uid);

		if (index == 0xFF)
			return;

		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[2].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

		if (info.m_hasEmblem) {
			const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

			if (emblem) {
				gdi->DrawTexture(emblem, WRect(m_pWinnerArea[2]->GetRect().x, m_pWinnerArea[2]->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
				offset = 27.0f;
			}
		}

		gdi->SetTextColor(color, 0xFFFFFFFF);
		gdi->SetTextStyle(0);
		g_pFresh->GetManager()->PrintText(WPoint(m_pWinnerArea[2]->GetRect().x + offset, m_pWinnerArea[2]->GetRect().y), 0, info.m_nickname, -1.0f, 0xFFFFFFFF);
	}
}

// awarddlg.cpp:529
void FrAwardDlg::OnWinner4Init(int param) {
	m_pWinnerArea[3] = DYNAMIC_CAST(FrArea, param);
	m_pWinnerArea[3]->m_nFlags.Enable(0x1000);
}

// awarddlg.cpp:535
void FrAwardDlg::OnWinner4OwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[3].m_uid);

		if (index == 0xFF)
			return;

		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[3].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

		if (info.m_hasEmblem) {
			const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

			if (emblem) {
				gdi->DrawTexture(emblem, WRect(m_pWinnerArea[3]->GetRect().x, m_pWinnerArea[3]->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
				offset = 27.0f;
			}
		}

		gdi->SetTextColor(color, 0xFFFFFFFF);
		gdi->SetTextStyle(0);
		g_pFresh->GetManager()->PrintText(WPoint(m_pWinnerArea[3]->GetRect().x + offset, m_pWinnerArea[3]->GetRect().y), 0, info.m_nickname, -1.0f, 0xFFFFFFFF);
	}
}

// awarddlg.cpp:573
void FrAwardDlg::OnWinner5Init(int param) {
	m_pWinnerArea[4] = DYNAMIC_CAST(FrArea, param);
	m_pWinnerArea[4]->m_nFlags.Enable(0x1000);
}

// awarddlg.cpp:579
void FrAwardDlg::OnWinner5OwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[4].m_uid);

		if (index == 0xFF)
			return;

		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[4].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

		if (info.m_hasEmblem) {
			const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

			if (emblem) {
				gdi->DrawTexture(emblem, WRect(m_pWinnerArea[4]->GetRect().x, m_pWinnerArea[4]->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
				offset = 27.0f;
			}
		}

		gdi->SetTextColor(color, 0xFFFFFFFF);
		gdi->SetTextStyle(0);
		g_pFresh->GetManager()->PrintText(WPoint(m_pWinnerArea[4]->GetRect().x + offset, m_pWinnerArea[4]->GetRect().y), 0, info.m_nickname, -1.0f, 0xFFFFFFFF);
	}
}

// awarddlg.cpp:617
void FrAwardDlg::OnWinner6Init(int param) {
	m_pWinnerArea[5] = DYNAMIC_CAST(FrArea, param);
	m_pWinnerArea[5]->m_nFlags.Enable(0x1000);
}

// awarddlg.cpp:623
void FrAwardDlg::OnWinner6OwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[5].m_uid);

		if (index == 0xFF)
			return;

		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_winnerPlayers[5].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

		if (info.m_hasEmblem) {
			const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

			if (emblem) {
				gdi->DrawTexture(emblem, WRect(m_pWinnerArea[5]->GetRect().x, m_pWinnerArea[5]->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
				offset = 27.0f;
			}
		}

		gdi->SetTextColor(color, 0xFFFFFFFF);
		gdi->SetTextStyle(0);
		g_pFresh->GetManager()->PrintText(WPoint(m_pWinnerArea[5]->GetRect().x + offset, m_pWinnerArea[5]->GetRect().y), 0, info.m_nickname, -1.0f, 0xFFFFFFFF);
	}
}

// awarddlg.cpp:661
void FrAwardDlg::OnLuckInit(int param) {
	m_pLuckArea = DYNAMIC_CAST(FrArea, param);
}

// awarddlg.cpp:666
void FrAwardDlg::OnLuckOwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_awardPlayers[0].m_uid);
		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_awardPlayers[0].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		const char* name;

		if (index == 0xFF) {
			name = "    -    ";
		} else {
			sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

			if (info.m_hasEmblem) {
				const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

				if (emblem) {
					gdi->DrawTexture(emblem, WRect(m_pLuckArea->GetRect().x, m_pLuckArea->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
					offset = 27.0f;
				}
			}

			name = info.m_nickname;
		}

		if (!WSingleton<CProjectG>::Instance()->HidePrivacy()) {
			gdi->SetTextColor(color, 0xFFFFFFFF);
			gdi->SetTextStyle(0);
			g_pFresh->GetManager()->PrintText(WPoint(m_pLuckArea->GetRect().x + offset, m_pLuckArea->GetRect().y), 0, name, -1.0f, 0xFFFFFFFF);
		}
	}
}

// awarddlg.cpp:725
void FrAwardDlg::OnSpeederInit(int param) {
	m_pSpeederArea = DYNAMIC_CAST(FrArea, param);
}

// awarddlg.cpp:730
void FrAwardDlg::OnSpeederOwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_awardPlayers[1].m_uid);
		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_awardPlayers[1].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		const char* name;

		if (index == 0xFF) {
			name = "    -    ";
		} else {
			sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

			if (info.m_hasEmblem) {
				const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

				if (emblem) {
					gdi->DrawTexture(emblem, WRect(m_pSpeederArea->GetRect().x, m_pSpeederArea->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
					offset = 27.0f;
				}
			}

			name = info.m_nickname;
		}

		if (!WSingleton<CProjectG>::Instance()->HidePrivacy()) {
			gdi->SetTextColor(color, 0xFFFFFFFF);
			gdi->SetTextStyle(0);
			g_pFresh->GetManager()->PrintText(WPoint(m_pSpeederArea->GetRect().x + offset, m_pSpeederArea->GetRect().y), 0, name, -1.0f, 0xFFFFFFFF);
		}
	}
}

// awarddlg.cpp:789
void FrAwardDlg::OnRunnerInit(int param) {
	m_pRunnerArea = DYNAMIC_CAST(FrArea, param);
}

// awarddlg.cpp:794
void FrAwardDlg::OnRunnerOwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_awardPlayers[2].m_uid);
		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_awardPlayers[2].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		const char* name;

		if (index == 0xFF) {
			name = "    -    ";
		} else {
			sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

			if (info.m_hasEmblem) {
				const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

				if (emblem) {
					gdi->DrawTexture(emblem, WRect(m_pRunnerArea->GetRect().x, m_pRunnerArea->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
					offset = 27.0f;
				}
			}

			name = info.m_nickname;
		}

		if (!WSingleton<CProjectG>::Instance()->HidePrivacy()) {
			gdi->SetTextColor(color, 0xFFFFFFFF);
			gdi->SetTextStyle(0);
			g_pFresh->GetManager()->PrintText(WPoint(m_pRunnerArea->GetRect().x + offset, m_pRunnerArea->GetRect().y), 0, name, -1.0f, 0xFFFFFFFF);
		}
	}
}

// awarddlg.cpp:854
void FrAwardDlg::OnChipInInit(int param) {
	m_pChipInArea = DYNAMIC_CAST(FrArea, param);
}

// awarddlg.cpp:859
void FrAwardDlg::OnChipInOwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_awardPlayers[3].m_uid);
		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_awardPlayers[3].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		const char* name;

		if (index == 0xFF) {
			name = "    -    ";
		} else {
			sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

			if (info.m_hasEmblem) {
				const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

				if (emblem) {
					gdi->DrawTexture(emblem, WRect(m_pChipInArea->GetRect().x, m_pChipInArea->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
					offset = 27.0f;
				}
			}

			name = info.m_nickname;
		}

		if (!WSingleton<CProjectG>::Instance()->HidePrivacy()) {
			gdi->SetTextColor(color, 0xFFFFFFFF);
			gdi->SetTextStyle(0);
			g_pFresh->GetManager()->PrintText(WPoint(m_pChipInArea->GetRect().x + offset, m_pChipInArea->GetRect().y), 0, name, -1.0f, 0xFFFFFFFF);
		}
	}
}

// awarddlg.cpp:918
void FrAwardDlg::OnLongPuttInit(int param) {
	m_pLongPuttArea = DYNAMIC_CAST(FrArea, param);
}

// awarddlg.cpp:923
void FrAwardDlg::OnLongPuttOwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_awardPlayers[4].m_uid);
		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_awardPlayers[4].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		const char* name;

		if (index == 0xFF) {
			name = "    -    ";
		} else {
			sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

			if (info.m_hasEmblem) {
				const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

				if (emblem) {
					gdi->DrawTexture(emblem, WRect(m_pLongPuttArea->GetRect().x, m_pLongPuttArea->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
					offset = 27.0f;
				}
			}

			name = info.m_nickname;
		}

		if (!WSingleton<CProjectG>::Instance()->HidePrivacy()) {
			gdi->SetTextColor(color, 0xFFFFFFFF);
			gdi->SetTextStyle(0);
			g_pFresh->GetManager()->PrintText(WPoint(m_pLongPuttArea->GetRect().x + offset, m_pLongPuttArea->GetRect().y), 0, name, -1.0f, 0xFFFFFFFF);
		}
	}
}

// awarddlg.cpp:983
void FrAwardDlg::OnRecoveryInit(int param) {
	m_pRecoveryArea = DYNAMIC_CAST(FrArea, param);
}

// awarddlg.cpp:988
void FrAwardDlg::OnRecoveryOwnerDraw(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi) {
		unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(WSingleton<CSharedDoc>::Instance()->m_awardPlayers[5].m_uid);
		unsigned long color = WSingleton<CSharedDoc>::Instance()->m_awardPlayers[5].m_uid == WSingleton<CSharedDoc>::Instance()->m_userInfo.guid ? 0xFFFF0000 : 0xFF000000;
		float offset = 0.0f;
		const char* name;

		if (index == 0xFF) {
			name = "    -    ";
		} else {
			sRivalData& info = WSingleton<CSharedDoc>::Instance()->m_gameResultList[index];

			if (info.m_hasEmblem) {
				const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(info.m_emblemName);

				if (emblem) {
					gdi->DrawTexture(emblem, WRect(m_pRecoveryArea->GetRect().x, m_pRecoveryArea->GetRect().y - 6.0f, (float)emblem->Width(), (float)emblem->Height()), 0xFFFFFFFF, 0);
					offset = 27.0f;
				}
			}

			name = info.m_nickname;
		}

		if (!WSingleton<CProjectG>::Instance()->HidePrivacy()) {
			gdi->SetTextColor(color, 0xFFFFFFFF);
			gdi->SetTextStyle(0);
			g_pFresh->GetManager()->PrintText(WPoint(m_pRecoveryArea->GetRect().x + offset, m_pRecoveryArea->GetRect().y), 0, name, -1.0f, 0xFFFFFFFF);
		}
	}
}

