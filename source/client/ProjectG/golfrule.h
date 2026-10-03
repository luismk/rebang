#pragma once

class CGolfRule
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

	sShotData* GetShotData() { return &m_shotData; }

	// TODO: incomplete
	unsigned char m_unused0[0x2f4];
	sShotData m_shotData;
};
