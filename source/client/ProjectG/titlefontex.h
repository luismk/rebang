#pragma once

enum eTITLEFONTALINE
{
	TFA_LEFT = 0,
	TFA_CENTER = 1,
	TFA_RIGHT = 2
};

class WTitleFontEx
{
public:
	virtual ~WTitleFontEx();

	float Print(float x, float y, const char* text) const;
	void Print(float x, float y, const char* text, eTITLEFONTALINE align,
		float space, float scale) const;
	float GetFontH() const;
	float GetFontW() const;
	float GetStringWidth(const char* text);

	void SetColor(unsigned long color) { m_color = color; }

protected:
	WTitleFontEx();

	int GetCharIndex(char c);

	WTitleFont* m_pFont;
	unsigned long m_color;
};

class CWindFont : public WTitleFontEx
{
public:
	CWindFont();
};

class COpenTournamentFont : public WTitleFontEx
{
public:
	COpenTournamentFont();
};

class CBlueWindFontSmall : public WTitleFontEx
{
public:
	CBlueWindFontSmall();
};

class CBlueWindFontBig : public WTitleFontEx
{
public:
	CBlueWindFontBig();
};

class CBlueHoleSkinFont : public WTitleFontEx
{
public:
	CBlueHoleSkinFont();
};

class CBarFont : public WTitleFontEx
{
public:
	CBarFont();
};
