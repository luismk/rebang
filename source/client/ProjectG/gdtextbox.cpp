// Original: d:\build\custom\temp\code\client\projectg\gdtextbox.cpp
#include "gdtextbox.h"
#include "tinyxml.h"
#include "commonutil.h"

// gdtextbox.cpp:6
CGDTextBox::CGDTextBox(void) {
}

// gdtextbox.cpp:11
CGDTextBox::~CGDTextBox(void) {
}

// gdtextbox.cpp:16
void CGDTextBox::Initialize(TiXmlNode* pNode) {
	TiXmlNode* pRow = pNode->FirstChild("row");
	while (pRow) {

		std::string strKey = pRow->ToElement()->Attribute("key");
		std::string strText = pRow->ToElement()->Attribute("text");

		str_Replace(strText, "\\n", "\n");
		str_Replace(strText, "\\\\", "\\");

		strText = strText.c_str();

		std::map<std::string, std::string>::iterator it = m_mapText.find(strKey);
		if (it == m_mapText.end()) {

			m_mapText.insert(std::map<std::string, std::string>::value_type(strKey, strText));
		}
		pRow = pRow->NextSibling("row");
	}
}

// gdtextbox.cpp:39
const char* CGDTextBox::GetText(const char* szKey) {
	std::map<std::string, std::string>::iterator it = m_mapText.find(szKey);
	if (it != m_mapText.end()) {

		return it->second.c_str();
	}

	return NULL;
}

// gdtextbox.cpp:50
const char* CGDTextBox::GetText(std::string& strKey) {
	std::map<std::string, std::string>::iterator it = m_mapText.find(strKey);
	if (it != m_mapText.end()) {

		return it->second.c_str();
	}

	return NULL;
}
