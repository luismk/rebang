// Original: d:\build\custom\temp\code\client\fresh\include\frtext.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRTEXT_H
#define PG_CLIENT_FRESH_INCLUDE_FRTEXT_H

#include <string.h>

#include <list>
#include <string>

#include "..\..\defines.h"

class FrGraphicInterface;

enum eFrTOKENTYPE {
	FrTEXT_TAG_STRING = 0,
	FrTEXT_TAG_STYLE_THICK = 1,
	FrTEXT_TAG_STYLE_NONE = 2,
	FrTEXT_TAG_STYLE_OUTLINE = 3,
	FrTEXT_TAG_STYLE_SHADOW = 4,
	FrTEXT_TAG_COLOR1 = 5,
	FrTEXT_TAG_COLOR2 = 6
};

enum eFrPRINTOPT {
	FrTEXT_PRINT_EXCLUDE_TAG = 0,
	FrTEXT_PRINT_ALL = 1
};

// One token of a tagged text: a plain string, a style tag or a color tag
// (PDB layout, size 0x20).
class FrTextToken {
	LAYOUT_CHECK_FRIEND;
public:
	FrTextToken(void);
	FrTextToken(const char* text) { ComposeData(std::string(text)); }   // frtext.h:58
	FrTextToken(const std::string& text) { ComposeData(text); }         // frtext.h:59
	~FrTextToken(void) {}                                                 // frtext.h:60

	void RunTag(FrGraphicInterface* pGI);
	std::string Text(void) const { return m_text; }                    // frtext.h:65
	void Text(std::string text);
	bool IsTag(void) const;
	eFrTOKENTYPE Type(void) const;
	bool GetColorValue(unsigned long* pColor) const;
	bool GetColorValue(unsigned long* pColor1, unsigned long* pColor2) const;

private:
	void ComposeData(const std::string& text);
	bool IsStyleTag(const std::string& text);
	bool SetStyleTagType(const char tag);
	bool IsColorTag(const std::string& text);
	bool SetColorTagType(const std::string& text);

	std::string m_text;                  // 0x0
	eFrTOKENTYPE m_type;                 // 0x1c
};

CHECK_SIZE(FrTextToken, 0x20);
BEGIN_LAYOUT_CHECK(FrTextToken)
	CHECK_FIELD(FrTextToken, m_text, 0x0);
	CHECK_FIELD(FrTextToken, m_type, 0x1c);
END_LAYOUT_CHECK

// One line of a tagged text, split into tokens (PDB layout, size 0xc).
class FrTextLine {
	LAYOUT_CHECK_FRIEND;
public:
	typedef std::list<FrTextToken> LTFRTOKEN;
	typedef LTFRTOKEN::iterator LTFRTOKEN_I;
	typedef LTFRTOKEN::const_iterator LTFRTOKEN_CI;

	FrTextLine(const char* text);
	FrTextLine(const std::string& text);
	~FrTextLine(void);

	std::string GetLineText(eFrPRINTOPT option) const;
	void GetTokenList(LTFRTOKEN& tokenList) const;
	float GetTextWidth(FrGraphicInterface* pGI) const;

private:
	void ComposeData(const std::string& text);

	LTFRTOKEN m_ltFrToken;               // 0x0
};

CHECK_SIZE(FrTextLine, 0xc);
BEGIN_LAYOUT_CHECK(FrTextLine)
	CHECK_FIELD(FrTextLine, m_ltFrToken, 0x0);
END_LAYOUT_CHECK

// A tagged multi-line text (PDB layout, size 0x2c).
class FrTEXT {
	LAYOUT_CHECK_FRIEND;
public:
	typedef std::list<FrTextLine*> LTFRTXLINE;
	typedef LTFRTXLINE::iterator LTFRTXLINE_I;
	typedef LTFRTXLINE::const_iterator LTFRTXLINE_CI;

	FrTEXT(const char* text);
	FrTEXT(const std::string& text);
	FrTEXT(void) : m_option(FrTEXT_PRINT_ALL) {}   // frtext.h:93
	~FrTEXT(void);

	void operator=(const char* text);
	void operator=(const std::string& text) { ComposeData(text); }   // frtext.h:99
	void operator<<(std::string& text);
	void SetPrintOption(eFrPRINTOPT option);
	eFrPRINTOPT GetPrintOption(void) const;
	unsigned int GetLineSize(void) const { return m_ltLine.size(); }   // frtext.h:107
	void GetTokenList(FrTextLine::LTFRTOKEN& tokenList) const;
	float GetWidthLong(FrGraphicInterface* pGI) const;
	void Clear(void);

private:
	void ComposeData(const std::string& text);
	std::string GetPrintData(void) const;

	eFrPRINTOPT m_option;                // 0x0
	LTFRTXLINE m_ltLine;                 // 0x4
	std::string m_srcText;               // 0x10
};

CHECK_SIZE(FrTEXT, 0x2c);
BEGIN_LAYOUT_CHECK(FrTEXT)
	CHECK_FIELD(FrTEXT, m_option, 0x0);
	CHECK_FIELD(FrTEXT, m_ltLine, 0x4);
	CHECK_FIELD(FrTEXT, m_srcText, 0x10);
END_LAYOUT_CHECK

// frtext.h:134 (extracts the text between the first begToken and the next
// endToken into output; returns the position of begToken)
inline unsigned int StringExtractFindFirst(const std::string& src, const char* begToken, const char* endToken, std::string& output) {
	if (begToken == NULL || endToken == NULL)
		return std::string::npos;

	unsigned int beg = src.find(begToken);
	if (beg == std::string::npos)
		return std::string::npos;

	unsigned int end = src.find(endToken, beg + 1);
	if (end == std::string::npos)
		return std::string::npos;

	output = std::string(src.begin() + beg + strlen(begToken), src.begin() + end);
	return beg;
}

// frtext.h:175 (removes every character of trimChars from str)
inline void StringTrim(std::string& str, const char* trimChars) {
	if (trimChars == NULL) {
		return;
	}

	std::string result;
	unsigned int size = str.length();
	unsigned int trimSize = strlen(trimChars);
	for (unsigned int i = 0; i < size; ++i) {
		bool found = false;
		unsigned int j;
		for (j = 0; j < trimSize; ++j) {
			if (trimChars[j] == str[i]) {
				found = true;
				break;
			}
		}
		if (!found) {
			result += str[i];
		}
	}
	str = result;
}

#endif // PG_CLIENT_FRESH_INCLUDE_FRTEXT_H
