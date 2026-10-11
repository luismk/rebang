// Original: d:\build\custom\temp\code\client\projectg\binstr.h
#ifndef PG_CLIENT_PROJECTG_BINSTR_H
#define PG_CLIENT_PROJECTG_BINSTR_H

#include "..\..\defines.h"

// A tokenizer over a character buffer the derived class provides (GetBuf).
// Layout from the PDB.
class cToken {
	LAYOUT_CHECK_FRIEND;

public:
	cToken(void);
	virtual ~cToken(void);

	virtual const char *GetBuf(void) const = 0;

	int GetPos(void) const;                                         // binstr.inl:6
	int GetLen(void) const;                                         // binstr.inl:26
	void MovePos(int pos);
	bool IsOutOfToken(void) const;
	bool GetToken(char **ppToken, const char *sep, int sepLen);
	bool GetToken(char *const pToken, const char *sep, int sepLen);
	bool GetTokenWithoutFail(char **ppToken, const char *sep, int sepLen);
	bool GetTokenWithoutFail(char *const pToken, const char *sep, int sepLen);
	bool GetTokenFullSep(char **ppToken, const char *sep, int sepLen);
	bool GetTokenFullSep(char *const pToken, const char *sep, int sepLen);
	int GetTokenNum(int start, const char *sep, int sepLen) const;
	int GetTokenNum(int start, char sep) const;
	char GetChar(void);
	int GetChange(char *const pDest);

protected:
	bool GetResultBuf(void);

	int m_Pos;          // 0x04
	int m_Len;          // 0x08
	int m_ResLen;       // 0x0C
	char *m_pcResBuf;   // 0x10
};

// Tokenizes a buffer it does not own.
class cTokenV : public cToken {
	LAYOUT_CHECK_FRIEND;

public:
	cTokenV(void);
	virtual ~cTokenV(void);

	virtual const char *GetBuf(void) const;                         // binstr.inl:118

	bool Init(const char *buf, int len);

protected:
	const char *m_pcBuf;   // 0x14
};

// Tokenizes its own copy of a buffer.
class cTokenS : public cToken {
	LAYOUT_CHECK_FRIEND;

public:
	cTokenS(void);
	virtual ~cTokenS(void);

	virtual const char *GetBuf(void) const;                         // binstr.inl:125

	bool Init(const char *buf, int len);

protected:
	char *m_pcBuf;   // 0x14
};

int StrToIntDefA(const char *str, int def);
int StrToIntDefFilterA(const char *str, int def);
char *IntToStrA(int value, char *buf);
char *MakePriceStrA(char *buf, __int64 price);
int PosB(const char *str, int strLen, const char *sub, int subLen);   // binstr.inl:135
char *TrimA(char *str, const char *chars);
char *ExchangeA(char *str, const char *from, const char *to);
char *RemoveA(char *str, const char *chars);
char *UpperCase(const char *src, char *dest);
char *LowerCase(const char *src, char *dest);
void MakeShortFileName(const char *path, char *dest);
int __cdecl mbscut(char *dest, const char *src, unsigned int len);
void ExtractPath(const char *path, char *dest);

#include "binstr.inl"

CHECK_SIZE(cToken, 0x14);
CHECK_SIZE(cTokenV, 0x18);
CHECK_SIZE(cTokenS, 0x18);
BEGIN_LAYOUT_CHECK(cToken)
	CHECK_FIELD(cToken, m_Pos, 0x04);
	CHECK_FIELD(cToken, m_Len, 0x08);
	CHECK_FIELD(cToken, m_ResLen, 0x0C);
	CHECK_FIELD(cToken, m_pcResBuf, 0x10);
END_LAYOUT_CHECK
BEGIN_LAYOUT_CHECK(cTokenV)
	CHECK_FIELD(cTokenV, m_pcBuf, 0x14);
END_LAYOUT_CHECK
BEGIN_LAYOUT_CHECK(cTokenS)
	CHECK_FIELD(cTokenS, m_pcBuf, 0x14);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_BINSTR_H
