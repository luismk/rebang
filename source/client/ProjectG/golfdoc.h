#pragma once

class CGolfDoc
{
public:
	CGolfDoc();
	virtual ~CGolfDoc();

	// TODO: incomplete
	unsigned char m_unused4[0xc1];
	unsigned char m_playerNum;
	unsigned char m_unusedc6[0xa2];
	unsigned char m_currentHole;
	unsigned char m_unused169[0xc];
	unsigned char m_currentPlayer;
};
