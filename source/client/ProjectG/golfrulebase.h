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

class CGolfRuleApproach : public CGolfRuleBase
{
public:
	CGolfRuleApproach(CGolfRule* pGolfRule);
	virtual ~CGolfRuleApproach();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();

private:
	WVector GetRandomStartPos(unsigned long oid);
};

class CGolfRuleNewApproach : public CGolfRuleBase
{
public:
	CGolfRuleNewApproach(CGolfRule* pGolfRule);
	virtual ~CGolfRuleNewApproach();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();

private:
	WVector GetRandomStartPos(unsigned long oid);
};

class CGolfRuleMatch : public CGolfRuleBase
{
public:
	CGolfRuleMatch(CGolfRule* pGolfRule);
	virtual ~CGolfRuleMatch();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();
	virtual void AddPlayer(unsigned char index, const char* name,
		unsigned long uid, unsigned long oid, unsigned long caddie);
};

class CGolfRuleTimeattack : public CGolfRuleBase
{
public:
	CGolfRuleTimeattack(CGolfRule* pGolfRule);
	virtual ~CGolfRuleTimeattack();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();
};

class CGolfRuleQuickrace : public CGolfRuleBase
{
public:
	CGolfRuleQuickrace(CGolfRule* pGolfRule);
	virtual ~CGolfRuleQuickrace();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();
};

class CGolfRuleTutorial : public CGolfRuleBase
{
public:
	CGolfRuleTutorial(CGolfRule* pGolfRule);
	virtual ~CGolfRuleTutorial();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();

protected:
	void SetEquipCharCaddieInfoForTutorial();
};

class CGolfRule30Battle : public CGolfRuleBase
{
public:
	CGolfRule30Battle(CGolfRule* pGolfRule);
	virtual ~CGolfRule30Battle();

	virtual void SetPlayer();
	virtual void SetGrade();
	virtual void SetReady();
};
