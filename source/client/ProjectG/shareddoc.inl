inline CSharedDoc* Doc()
{
	return CSharedDoc::Instance();
}

inline CItemManager* ItemManager();

inline unsigned char PrevMap(unsigned char map)
{
	int cur = 17;
	for (int i = 0; i < 18; i++)
	{
		if (s_courseOrder[i] == map)
		{
			cur = i;
			break;
		}
	}

	for (int j = cur - 1; j >= 0; j--)
	{
		if (IsLocalContent(S3_ROOKIE_CHANNEL))
		{
			if ((Doc()->m_curChannel.Type & 0x800) &&
				!Doc()->CanUseRookieChannelMap(s_courseOrder[j]))
				continue;
		}
		return s_courseOrder[j];
	}

	return map;
}

inline unsigned char NextMap(unsigned char map, bool bLoop)
{
	int cur = 0;
	for (int i = 0; i < 18; i++)
	{
		if (s_courseOrder[i] == map)
		{
			cur = i;
			break;
		}
	}

	for (int j = cur + 1; j < 18; j++)
	{
		if (IsLocalContent(S3_ROOKIE_CHANNEL))
		{
			if ((Doc()->m_curChannel.Type & 0x800) &&
				!Doc()->CanUseRookieChannelMap(s_courseOrder[j]))
				continue;
		}
		return s_courseOrder[j];
	}

	return map;
}

inline unsigned char FirstMap()
{
	if (!(Doc()->m_curChannel.Type & 0x800))
		return 19;

	if (IsLocalContent(S3_ROOKIE_CHANNEL) && (Doc()->m_curChannel.Type & 0x800))
		return 11;

	int i;
	for (i = 0; i < 18; i++)
	{
		IFF_STRUCT::sCourse* pCourse =
			ItemManager()->FindCourse(s_courseOrder[i] | 0x28000000);
		if (pCourse->Difficulty < 3)
			break;
	}
	return s_courseOrder[i];
}

inline unsigned char LastMap(bool bAll)
{
	if (!(Doc()->m_curChannel.Type & 0x800))
	{
		if (bAll)
			return 9;
		return 127;
	}

	int i;
	for (i = 17; i >= 0; i--)
	{
		if (bAll && s_courseOrder[i] == 127)
			continue;
		IFF_STRUCT::sCourse* pCourse =
			ItemManager()->FindCourse(s_courseOrder[i] | 0x28000000);
		if (pCourse->Difficulty < 3)
			break;
	}
	return s_courseOrder[i];
}

inline CItemManager* ItemManager()
{
	return &Doc()->m_itemManager;
}

inline CGolfDoc* GOLFDOC()
{
	return Doc()->m_pGolfDoc;
}

inline char* MyId()
{
	return Doc()->m_myInfo.info.sID;
}

inline char* MyNick()
{
	return Doc()->m_myInfo.info.sNick;
}

inline unsigned long MyUID()
{
	return Doc()->m_myInfo.info.dwUID;
}

inline int GalleryMode();

inline unsigned long MyGuid(bool bGallery)
{
	if (bGallery && GalleryMode())
		return Doc()->m_myInfo.info.dwGalleryGuid;
	return Doc()->m_myInfo.info.dwGuid;
}

inline __int64 MyPang()
{
	return Doc()->m_myInfo.stat.i64Pang;
}

inline __int64 MyCookie()
{
	return Doc()->m_cookie;
}

inline unsigned long GetShotTimeLimit()
{
	return Doc()->m_golfGame.shotTimeLimit;
}

inline unsigned char GetHoleIndex(unsigned char hole)
{
	for (unsigned char i = 0; i < 18; i++)
	{
		if (Doc()->m_holeOrder[i] == hole)
			return i + 1;
	}
	return 19;
}

inline const char* GetGameTypeName(unsigned char gameType);

inline void SetGameType(unsigned char gameType)
{
	Doc()->m_golfGame.gameType = gameType;
	if (gameType < GAME_TYPE_MAX)
		strcpy(Doc()->m_golfGame.gameTypeName, GetGameTypeName(gameType));
}

inline const sGameTypeInfo* GetGameTypeInfo(unsigned char gameType)
{
	if (gameType >= GAME_TYPE_MAX)
	{
		return NULL;
	}

	return &Doc()->m_gameTypeInfo[gameType];
}

inline const char* GetGameTypeName(unsigned char gameType)
{
	if (gameType >= GAME_TYPE_MAX)
	{
		return NULL;
	}

	return GetGameTypeInfo(gameType)->name;
}

inline unsigned char GetHoles()
{
	return Doc()->m_golfGame.holes;
}

inline void SetHoles(unsigned char holes)
{
	Doc()->m_golfGame.holes = holes;
}

inline void SetWeather(unsigned char weather)
{
	Doc()->m_golfGame.weather = weather;
}

inline unsigned char CSharedDoc::GetIndex(unsigned long uid)
{
	if (uid == 0xffffffff)
	{
		CTask* pTask = AfxGetTask();
		if (pTask && pTask->IsKindOf(&CGolfTask::m_RTTI))
			return GOLFDOC()->m_currentPlayer;
	}
	else if (OnlinePlay())
	{
		std::map<unsigned long, unsigned char>::iterator it =
			m_indexMap.find(uid);

		if (it != m_indexMap.end())
			return m_indexMap[uid];
	}
	else
	{
		CTask* pTask = AfxGetTask();
		if (pTask && pTask->IsKindOf(&CGolfTask::m_RTTI))
			return GOLFDOC()->m_currentPlayer;
	}

	return 0xff;
}

inline int IsMassGame(unsigned char gameType)
{
	switch (gameType)
	{
	case GAME_TYPE_30S:
	case GAME_TYPE_30S_TEAM:
	case GAME_TYPE_GUILD_MATCH:
	case GAME_TYPE_APPROACH:
	case GAME_TYPE_NEW_APPROACH:
	case GAME_TYPE_USEMAX:
		return TRUE;
	}
	return FALSE;
}

inline int GalleryMode()
{
	bool bGM = (Doc()->m_myInfo.info.dwIdentity >> 1) & 1;
	if (bGM)
		return TRUE;
	return FALSE;
}

inline unsigned char GetCurRoomMapType()
{
	return Doc()->m_roomInfo.mapType;
}
