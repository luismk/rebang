#pragma once

// TODO: incomplete
class CFxSpray
{
public:
	WVector& Pos() { return m_pos; }

	unsigned char m_unused0[0x110c];
	bool m_bActive;
	unsigned char m_unused110d[0x2b];
	WVector m_pos;
};
