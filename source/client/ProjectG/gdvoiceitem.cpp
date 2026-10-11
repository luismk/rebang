// Original: d:\build\custom\temp\code\client\projectg\gdvoiceitem.cpp
#include <stdio.h>

#include "gdvoiceitem.h"
#include "tinyxml.h"

// gdvoiceitem.cpp:7
CGDVoiceItem::CGDVoiceItem(void) {
}

// gdvoiceitem.cpp:12
CGDVoiceItem::~CGDVoiceItem(void) {
}

// gdvoiceitem.cpp:17
void CGDVoiceItem::Initialize(TiXmlNode* pNode) {
	TiXmlNode* pRow = pNode->FirstChild("row");
	while (pRow) {

		sGD_VOICEITEM item;
		std::string strTypeID = pRow->ToElement()->Attribute("typeid");
		item.strName = pRow->ToElement()->Attribute("name");
		std::string strState = pRow->ToElement()->Attribute("state");
		std::string strNumOfSounds = pRow->ToElement()->Attribute("numofsounds");

		sscanf(strTypeID.c_str(), "%d", &item.nTypeID);
		sscanf(strState.c_str(), "%d", &item.nState);
		sscanf(strNumOfSounds.c_str(), "%d", &item.byNumOfSounds);

		m_listVoiceItem.push_back(item);

		pRow = pRow->NextSibling("row");
	}
}
