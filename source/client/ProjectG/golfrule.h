#pragma once

#include "actor.h"

class CGolfRule : public IActor
{
public:
	struct sShotData
	{
		unsigned char hole;
		// TODO: incomplete
	};

	void AddPlayer(unsigned char index, const char* name, unsigned long uid,
		unsigned long oid, unsigned long caddie);
	void SendHoleData();
	void LoadShot(const char* filename);
	unsigned char GetFirstTurn();
	void ResetData(const WVector& pos);
	void ChangeGameMode(unsigned long mode);

	sShotData* GetShotData() { return &m_shotData; }

	// TODO: incomplete
	unsigned char m_unused1c[0x2f4 - sizeof(IActor)];
	sShotData m_shotData;
};
