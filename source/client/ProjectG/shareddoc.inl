inline CSharedDoc* Doc()
{
	return CSharedDoc::Instance();
}

inline char* MyId()
{
	return Doc()->m_myInfo.userInfo.id;
}

inline char* MyNick()
{
	return Doc()->m_myInfo.userInfo.nickname;
}

inline unsigned long MyUID()
{
	return Doc()->m_myInfo.userInfo.uid;
}
