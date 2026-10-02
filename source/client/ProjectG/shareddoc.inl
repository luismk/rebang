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
