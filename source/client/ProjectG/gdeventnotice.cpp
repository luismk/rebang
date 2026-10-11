// Original: d:\build\custom\temp\code\client\projectg\gdeventnotice.cpp
#include <stdlib.h>

#include "gdeventnotice.h"
#include "tinyxml.h"
#include "commonutil.h"

// gdeventnotice.cpp:8
CGDEventNotice::CGDEventNotice(void) {
}

// gdeventnotice.cpp:12
CGDEventNotice::~CGDEventNotice(void) {
}

// gdeventnotice.cpp:16
void CGDEventNotice::Initialize(TiXmlNode* pNode) {
	TiXmlNode* pRow = pNode->FirstChild("row");
	while (pRow) {

		std::string strKey = pRow->ToElement()->Attribute("key");
		int nKey = atoi(strKey.c_str());
		std::string strText = pRow->ToElement()->Attribute("text");

		str_Replace(strText, "\\n", "\n");
		str_Replace(strText, "\\\\", "\\");

		strText = strText.c_str();

		std::map<int, std::string>::iterator it = m_mapText.find(nKey);
		if (it == m_mapText.end()) {

			m_mapText.insert(std::map<int, std::string>::value_type(nKey, strText));
		}
		pRow = pRow->NextSibling("row");
	}
}

// gdeventnotice.cpp:40
const char* CGDEventNotice::GetText(int nKey) {
	std::map<int, std::string>::iterator it = m_mapText.find(nKey);
	if (it != m_mapText.end()) {

		return it->second.c_str();
	}

	return NULL;
}
