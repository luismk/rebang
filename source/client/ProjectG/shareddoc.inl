// Original: d:\build\custom\temp\code\client\projectg\shareddoc.inl
// Global accessors of the shared document. Inline in the original (other
// units emit some of them, e.g. GOLFDOC); defined once here, in the unit
// that emitted these, until their callers are transcribed.

// shareddoc.inl:229
void SetHoles(unsigned char holes) {
	WSingleton<CSharedDoc>::Instance()->m_holes = holes;
}

// shareddoc.inl:293
void SetWeather(unsigned char weather) {
	WSingleton<CSharedDoc>::Instance()->m_weather = weather;
}

// shareddoc.inl:334
unsigned char GetPlayerNum(void) {
	return WSingleton<CSharedDoc>::Instance()->m_pGolfDoc->GetPlayerNum();
}

// shareddoc.inl:368
WVector GetHoleCupPos(void) {
	return GOLFDOC()->GetHoleData().m_cupPos;
}

// shareddoc.inl:389, a player's index by UID (the local player's in the golf
// task for no UID), 0xFF when unknown.
unsigned char CSharedDoc::GetIndex(unsigned long uid) {
	if (uid == 0xFFFFFFFF) {
		if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI) : false) {
			return WSingleton<CSharedDoc>::Instance()->m_pGolfDoc->m_myPlayerIndex;
		}

		return 0xFF;
	}

	if (OnlinePlay()) {
		if (m_indexMap.find(uid) == m_indexMap.end()) {
			return 0xFF;
		}

		return m_indexMap[uid];
	}

	if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI) : false) {
		return WSingleton<CSharedDoc>::Instance()->m_pGolfDoc->m_myPlayerIndex;
	}

	return 0xFF;
}

// shareddoc.inl:340 PLAYER: defined in SharedDoc.h (SHAREDDOC_INLINE).

// shareddoc.inl:446 MyUID: defined in SharedDoc.h (SHAREDDOC_INLINE).

// shareddoc.inl:451
int IsMassGame(void) {
	switch (WSingleton<CSharedDoc>::Instance()->m_roomType) {
	case 4:
	case 5:
	case 6:
	case 9:
	case 10:
		return 1;
	}

	return 0;
}

// shareddoc.inl:482
int IsGuildGame(void) {
	return WSingleton<CSharedDoc>::Instance()->m_roomType == 6;
}

// shareddoc.inl:492
char *MyNick(void) {
	return WSingleton<CSharedDoc>::Instance()->m_userInfo.nickname;
}

// shareddoc.inl:562, the angel wing parts (worn only by manner players of
// level 21 and up, CShopMain).
bool IsAngelWing(unsigned long typeId) {
	switch (typeId) {
	case 0x8016800:
	case 0x8058800:
	case 0x8098800:
	case 0x80DC800:
	case 0x8118800:
	case 0x8160800:
	case 0x8190800:
	case 0x81E2800:
	case 0x8214800:
	case 0x8254800:
		return true;
	}

	return false;
}

// shareddoc.inl:602, under 3% of the local player's games ended without
// manners. Inline in the original (RealMyRoomMainUI emitted the copy the
// linker kept); defined here until that unit's callers are transcribed.
bool IsMannerPlayer(void) {
	unsigned long games = WSingleton<CSharedDoc>::Instance()->m_myStatGamesPlayed;
	if (games < 1) {
		games = 1;
	}

	if ((float)WSingleton<CSharedDoc>::Instance()->m_myStatNoMannerGames / (float)games * 100.0f < 3.0f) {
		return true;
	}

	return false;
}
