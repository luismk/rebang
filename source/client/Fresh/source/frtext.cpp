// Original: d:\build\custom\temp\code\client\fresh\source\frtext.cpp
#include <stdlib.h>
#include <string.h>

#include "frtext.h"
#include "frgraphicinterface.h"

// frtext.cpp:24 (extracts the first colour tag: output receives the tag
// with its value, the result is its position)
unsigned int StringExtractTagTextFindFirst(const std::string& src, std::string& output) {
	std::string szToken;
	unsigned int pos = src.length();

	{
		std::string szTagch("\\");
		szTagch += 'c';
		unsigned int found = StringExtractFindFirst(src, szTagch.c_str(), szTagch.c_str(), szToken);
		if (found != std::string::npos && found < pos) {
			pos = found;
			output = szTagch + szToken + szTagch;
		}
	}

	if (pos == src.length())
		return std::string::npos;

	return pos;
}

// frtext.cpp:48
void FrTextToken::RunTag(FrGraphicInterface* pGI) {
	switch (m_type) {
	case FrTEXT_TAG_STYLE_THICK:
		pGI->SetTextStyle(1);
		break;
	case FrTEXT_TAG_STYLE_NONE:
		pGI->SetTextStyle(0);
		break;
	case FrTEXT_TAG_STYLE_OUTLINE:
		pGI->SetTextStyle(2);
		break;
	case FrTEXT_TAG_STYLE_SHADOW:
		pGI->SetTextStyle(4);
		break;
	case FrTEXT_TAG_COLOR1:
		{
			unsigned long color;
			if (GetColorValue(&color)) {
				pGI->SetTextColor(color, 0xFFFFFFFF);
			}
		}
		break;
	case FrTEXT_TAG_COLOR2:
		{
			unsigned long color;
			unsigned long outlineColor;
			if (GetColorValue(&color, &outlineColor)) {
				pGI->SetTextColor(color, outlineColor);
			}
		}
		break;
	}
}

// frtext.cpp:90
bool FrTextToken::IsTag(void) const {
	return m_type != FrTEXT_TAG_STRING;
}

// frtext.cpp:95
bool FrTextToken::GetColorValue(unsigned long* textcolor, unsigned long* outcolor) const {
	if (m_type != FrTEXT_TAG_COLOR2) {
		return false;
	}

	std::string trimSet(" \\");

	trimSet += 'c';

	std::string copy(m_text);
	StringTrim(copy, trimSet.c_str());

	unsigned int pos = copy.find(",");
	if (copy.empty() || pos == std::string::npos) {
		return false;
	}

	std::string szColor1 = copy.substr(0, pos);
	std::string szColor2 = copy.substr(pos + 1, copy.length() - pos);

	if (szColor1.empty() || szColor2.empty()) {
		return false;
	}

	*textcolor = strtoul(szColor1.c_str(), NULL, 16);
	*outcolor = strtoul(szColor2.c_str(), NULL, 16);

	return true;
}

// frtext.cpp:126
bool FrTextToken::GetColorValue(unsigned long* pColor) const {
	if (m_type != FrTEXT_TAG_COLOR1) {
		return false;
	}

	std::string trimChars(" \\");

	trimChars += 'c';

	std::string value(m_text);
	StringTrim(value, trimChars.c_str());

	if (value.empty()) {
		return false;
	}
	*pColor = strtoul(value.c_str(), NULL, 16);
	return true;
}

// frtext.cpp:147
void FrTextToken::ComposeData(const std::string& text) {
	m_text = text;

	m_type = FrTEXT_TAG_STRING;
	if (m_text.length() < 2) {
		return;
	}

	std::string trimSet(" \\");

	trimSet += 'c';

	std::string copy(m_text);
	StringTrim(copy, trimSet.c_str());

	if (IsStyleTag(copy)) {
		SetStyleTagType(copy[0]);
	} else if (IsColorTag(copy)) {
		SetColorTagType(copy);
	}
}

// frtext.cpp:175
bool FrTextToken::IsStyleTag(const std::string& text) {
	if (text.length() == 1) {
		if (strchr("nNtToOsS", text[0])) {
			return true;
		}
	}
	return false;
}

// frtext.cpp:186
// __forceinline as in the original: ComposeData inlines this and the TU still
// emits the unreferenced out-of-line copy (AGENTS.md rule 14 exception).
__forceinline bool FrTextToken::IsColorTag(const std::string& text) {
	const char* format = "0xAARRGGBB";
	if (text.length() >= strlen(format) && text[0] == '0' && text[1] == 'x') {
		return true;
	}
	return false;
}

// frtext.cpp:197
bool FrTextToken::SetStyleTagType(const char tag) {
	if (tag == 't' || tag == 'T') {
		m_type = FrTEXT_TAG_STYLE_THICK;
	} else if (tag == 'n' || tag == 'N') {
		m_type = FrTEXT_TAG_STYLE_NONE;
	} else if (tag == 'o' || tag == 'O') {
		m_type = FrTEXT_TAG_STYLE_OUTLINE;
	} else if (tag == 's' || tag == 'S') {
		m_type = FrTEXT_TAG_STYLE_SHADOW;
	} else {
		return false;
	}
	return true;
}

// frtext.cpp:217
bool FrTextToken::SetColorTagType(const std::string& text) {
	unsigned int size = text.length();
	const char* format1 = "0xAARRGGBB";
	const char* format2 = "0xAARRGGBB,0xAARRGGBB";
	if (size >= strlen(format1) && text[0] == '0' && text[1] == 'x') {
		unsigned int pos;

		if ((pos = text.find(',')) != std::string::npos && size == strlen(format2)) {

			if (text[pos + 1] == '0' && text[pos + 2] == 'x') {

				m_type = FrTEXT_TAG_COLOR2;
				return true;
			}
		} else {

			m_type = FrTEXT_TAG_COLOR1;
			return true;
		}
	}
	return false;
}

// frtext.cpp:255
FrTextLine::FrTextLine(const char* text) { ComposeData(std::string(text)); }
// frtext.cpp:256
FrTextLine::FrTextLine(const std::string& text) { ComposeData(text); }
// frtext.cpp:258
FrTextLine::~FrTextLine(void) {}

// frtext.cpp:261
std::string FrTextLine::GetLineText(eFrPRINTOPT option) const {
	std::string text;

	if (option == FrTEXT_PRINT_EXCLUDE_TAG) {
		for (LTFRTOKEN_CI it = m_ltFrToken.begin(); it != m_ltFrToken.end(); ++it) {
			if (!it->IsTag()) {
				text += it->Text();
			}
		}
	} else {
		for (LTFRTOKEN_CI it = m_ltFrToken.begin(); it != m_ltFrToken.end(); ++it) {
			text += it->Text();
		}
	}
	return text;
}

// frtext.cpp:279
void FrTextLine::GetTokenList(LTFRTOKEN& tokenList) const {
	for (LTFRTOKEN_CI it = m_ltFrToken.begin(); it != m_ltFrToken.end(); ++it) {
		tokenList.push_back(*it);
	}
}

// frtext.cpp:286
float FrTextLine::GetTextWidth(FrGraphicInterface* pGI) const {
	std::string text = GetLineText(FrTEXT_PRINT_EXCLUDE_TAG);



	return pGI->GetTextExtend11(text.c_str());
}

// frtext.cpp:296
void FrTextLine::ComposeData(const std::string& src) {
	std::string szToken, szText;

	std::string copy(src);

	unsigned int pos = StringExtractTagTextFindFirst(copy, szToken);
	while (pos != std::string::npos) {
		if (pos != 0) {
			szText.assign(copy.begin(), copy.begin() + pos);
			m_ltFrToken.push_back(FrTextToken(szText));
		}

		m_ltFrToken.push_back(FrTextToken(szToken));

		if (szText.length() + szToken.length() >= copy.length())
			return;
		copy = std::string(copy.begin() + szText.length() + szToken.length(), copy.end());
		pos = StringExtractTagTextFindFirst(copy, szToken);
	}

	szText.assign(copy.begin(), copy.end());
	m_ltFrToken.push_back(FrTextToken(szText));
}

// frtext.cpp:339
FrTEXT::~FrTEXT(void) {
	Clear();
}

// frtext.cpp:344
void FrTEXT::Clear(void) {
	m_option = FrTEXT_PRINT_EXCLUDE_TAG;
	m_srcText = "";
	for (LTFRTXLINE_I it = m_ltLine.begin(); it != m_ltLine.end(); ++it) {
		delete *it;
	}
	m_ltLine.clear();
}

// frtext.cpp:354
void FrTEXT::GetTokenList(FrTextLine::LTFRTOKEN& tokenList) const {
	LTFRTXLINE_CI it = m_ltLine.begin();
	while (it != m_ltLine.end()) {
		FrTextLine* pLine = *it;
		FrTextLine::LTFRTOKEN lineTokens;
		pLine->GetTokenList(lineTokens);

		tokenList.insert(tokenList.end(), lineTokens.begin(), lineTokens.end());


		if (++it != m_ltLine.end()) {
			tokenList.push_back(FrTextToken("\n"));
		}
	}
}

// frtext.cpp:371
std::string FrTEXT::GetPrintData(void) const {
	if (m_option == FrTEXT_PRINT_ALL) {
		return m_srcText;
	}
	std::string text("");
	LTFRTXLINE_CI it = m_ltLine.begin();
	while (it != m_ltLine.end()) {
		FrTextLine* pLine = *it;
		text += pLine->GetLineText(m_option);

		if (++it != m_ltLine.end()) {
			text += "\n";
		}
	}
	return text;
}

// frtext.cpp:389
void FrTEXT::ComposeData(const std::string& src) {
	bool same = m_srcText == src;
	if (same) {
		return;
	}

	Clear();
	m_srcText = src;

	std::string buffer("");
	for (std::string::const_iterator srcci = src.begin(); srcci != src.end(); ++srcci) {
		if (*srcci == '\n' || *srcci == '\r') {
			if (buffer.length()) {
				FrTextLine* pTxLine = new FrTextLine(buffer);
				m_ltLine.push_back(pTxLine);
				buffer.clear();
			} else {
				FrTextLine* pTxLine = new FrTextLine(" ");
				m_ltLine.push_back(pTxLine);
				buffer.clear();
			}
		} else if (srcci + 1 != src.end()
			&& *srcci == '\\' && *(srcci + 1) == 'n') {
			if (buffer.length()) {
				FrTextLine* pTxLine = new FrTextLine(buffer);
				m_ltLine.push_back(pTxLine);
			} else {
				FrTextLine* pTxLine = new FrTextLine(" ");
				m_ltLine.push_back(pTxLine);
			}
			buffer.clear();
			++srcci;
		} else {
			buffer += *srcci;
		}
	}

	if (buffer.length()) {
		FrTextLine* pLast = new FrTextLine(buffer);
		m_ltLine.push_back(pLast);
	}
}

// frtext.cpp:456
float FrTEXT::GetWidthLong(FrGraphicInterface* pGI) const {
	float width = 0.0f;
	for (LTFRTXLINE_CI it = m_ltLine.begin(); it != m_ltLine.end(); ++it) {
		FrTextLine* pLine = *it;
		float lineWidth = pLine->GetTextWidth(pGI);
		if (lineWidth > width) {
			width = lineWidth;
		}
	}
	return width;
}
