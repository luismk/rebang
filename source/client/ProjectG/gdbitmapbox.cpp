// Original: d:\build\custom\temp\code\client\projectg\gdbitmapbox.cpp
#include <stdio.h>
#include <string.h>

#include "gdbitmapbox.h"
#include "tinyxml.h"

// gdbitmapbox.cpp:8
CGDBitmapBox::CGDBitmapBox(void) {
}

// gdbitmapbox.cpp:12
CGDBitmapBox::~CGDBitmapBox(void) {
}

// gdbitmapbox.cpp:17
void CGDBitmapBox::Initialize(TiXmlNode* pNode) {
	TiXmlNode* pRow = pNode->FirstChild("row");
	while (pRow) {

		std::string strKey = pRow->ToElement()->Attribute("key");
		std::string strFile = pRow->ToElement()->Attribute("file");
		std::string strRect = pRow->ToElement()->Attribute("rect");
		sGD_BITMAP bitmap;
		strcpy(bitmap.szFileName, strFile.c_str());
		sscanf(strRect.c_str(), "%f %f %f %f", &bitmap.rect.x, &bitmap.rect.y, &bitmap.rect.w, &bitmap.rect.h);
		std::map<std::string, sGD_BITMAP>::iterator it = m_mapBitmap.find(strKey);
		if (it == m_mapBitmap.end()) {

			m_mapBitmap.insert(std::map<std::string, sGD_BITMAP>::value_type(strKey, bitmap));
		}
		pRow = pRow->NextSibling("row");
	}
}

// gdbitmapbox.cpp:37
const char* CGDBitmapBox::GetFileName(char* szKey) {
	std::map<std::string, sGD_BITMAP>::iterator it = m_mapBitmap.find(szKey);
	if (it != m_mapBitmap.end()) {

		return it->second.szFileName;
	}

	return NULL;
}

// gdbitmapbox.cpp:48
void CGDBitmapBox::GetRect(const char* szKey, _WRECT& rect) {
	std::map<std::string, sGD_BITMAP>::iterator it = m_mapBitmap.find(szKey);
	if (it != m_mapBitmap.end()) {

		rect.x = it->second.rect.x;
		rect.y = it->second.rect.y;
		rect.w = it->second.rect.w;
		rect.h = it->second.rect.h;
	}
}
