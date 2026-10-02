#pragma once

class ILoginInfo;

class CLoginInfo
{
public:
	static CLoginInfo* Instance()
	{
		if (m_pInst == NULL)
			m_pInst = new CLoginInfo;
		return m_pInst;
	}

	int IsPcBang() const;
	unsigned long AuthUid() const;

private:
	CLoginInfo();
	virtual ~CLoginInfo();

	static CLoginInfo* m_pInst;
	ILoginInfo* m_pLoginInfo;
	int m_bWebLogin;
	// TODO: incomplete
};
