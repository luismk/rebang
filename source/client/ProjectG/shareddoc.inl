inline CSharedDoc* Doc()
{
	return CSharedDoc::Instance();
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

inline unsigned long MyGuid(bool bGallery)
{
	bool bGM = (Doc()->m_myInfo.info.dwIdentity >> 1) & 1;
	if (bGM && bGallery)
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
