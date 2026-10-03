#pragma once

class CGolfRule;

class CGolfRuleBase
{
public:
	CGolfRuleBase();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();
	virtual void AddPlayer(unsigned char index, const char* name,
		unsigned long uid, unsigned long oid, unsigned long caddie);
	virtual void AddPlayerOffline(unsigned char index, const char* name,
		unsigned long oid, unsigned long caddie);
	virtual int GetNumTimeout();
	virtual bool CheckGiveUp();
	virtual ~CGolfRuleBase();

	void CheckCalipersCount();

protected:
	CGolfRule* m_pGolfRule;
	unsigned long m_playerColor[4];
};

class CGolfRuleStroke : public CGolfRuleBase
{
public:
	CGolfRuleStroke(CGolfRule* pGolfRule);
	virtual ~CGolfRuleStroke();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();
};

class CGolfRuleGuildMatch : public CGolfRuleBase
{
public:
	CGolfRuleGuildMatch(CGolfRule* pGolfRule);
	virtual ~CGolfRuleGuildMatch();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();
};

class CGolfRuleSkins : public CGolfRuleBase
{
public:
	CGolfRuleSkins(CGolfRule* pGolfRule);
	virtual ~CGolfRuleSkins();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();
};
