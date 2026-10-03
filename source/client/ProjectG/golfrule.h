#pragma once

#include "actor.h"

class CGolfRule : public IActor
{
public:
	friend class CGolfRuleBase;
	friend class CGolfRuleQuickrace;
	friend class CGolfRuleTimeattack;
	friend class CGolfRuleTutorial;

	struct sShotData
	{
		unsigned char hole;
		// TODO: incomplete
	};

	void AddPlayer(unsigned char index, const char* name, unsigned long uid,
		unsigned long oid, unsigned long caddie);
	void SendHoleData();
	void LoadShot(const char* filename);

	sShotData* GetShotData() { return &m_shotData; }

protected:
	unsigned char GetFirstTurn();
	void ResetData(const WVector& pos);
	void ChangeGameMode(unsigned long mode);

public:
	// TODO: incomplete
	unsigned char m_unused1c[0x2f4 - sizeof(IActor)];
	sShotData m_shotData;
};
