// Original: d:\build\custom\temp\code\client\fresh\source\frelement.cpp
#include <locale.h>
#include <stdio.h>
#include <string.h>

#include "frelement.h"
#include "bitmap.h"
#include "westpak.h"
#include "wresrcmng.h"
#include "../client/ProjectG/commonutil.h"
#include "../client/ProjectG/tinyxml.h"

// <windows.h> maps GetForm to GetFormA; FrElementDoc::GetForm is declared before it.
#ifdef GetForm
#undef GetForm
#endif


void __cdecl CloseCFile(cFile* file);

// frelement.cpp:21
FrElementTID ElementType;
// frelement.cpp:22
FrGuiTID GuiType;

// frelement.cpp:25
FrElementTID::FrElementTID(void) {
	m_map[ED_NONE] = "NONE";
	m_map[ED_FRAME] = "FRAME";
	m_map[ED_LAYOUT] = "LAYOUT";
	m_map[ED_FORM] = "FORM";
	m_map[ED_BITMAP] = "BITMAP";
	m_map[ED_ICON] = "ICON";
	m_map[ED_INCLUDE] = "INCLUDE";
	m_map[ED_MACROITEM] = "MACROITEM";
}

// frelement.cpp:41
FrGuiTID::FrGuiTID(void) {
	m_map[GI_NONE] = "NONE";
	m_map[GI_FORM] = "FORM";
	m_map[GI_STATIC] = "STATIC";
	m_map[GI_TEXTBUTTON] = "TEXTBUTTON";
	m_map[GI_EDIT] = "EDIT";
	m_map[GI_COMBOBOX] = "COMBOBOX";
	m_map[GI_COMBOCTLEX] = "COMBOCTLEX";
	m_map[GI_BUTTON] = "BUTTON";
	m_map[GI_FRAME] = "FRAME";
	m_map[GI_RESOURCE] = "RESOURCE";
	m_map[GI_BITMAP] = "BITMAP";
	m_map[GI_AREA] = "AREA";
	m_map[GI_LISTBOX] = "LISTBOX";
	m_map[GI_GAUGEBAR] = "GAUGEBAR";
	m_map[GI_GAUGEBAREX] = "GAUGEBAREX";
	m_map[GI_GAUGEBARIMAGE] = "GAUGEBARIMAGE";
	m_map[GI_VIEWER] = "VIEWER";
	m_map[GI_CONTEXTMENU] = "CONTEXTMENU";
	m_map[GI_TABBUTTON] = "TABBUTTON";
	m_map[GI_GROUPBOX] = "GROUPBOX";
	m_map[GI_MACROITEM] = "MACROITEM";
}

// frelement.cpp:70
static void CreateGuiItem(const TiXmlNode* pSrc, std::list<FrGuiItem*>& guiList) {
	TiXmlNode* pNode = pSrc->FirstChild("item");
	while (pNode != 0) {
		FrGuiItem* pItem;
		if (pNode->FirstChild("item") != 0) {
			pItem = new FrGuiItemNested;
		} else {
			pItem = new FrGuiItem;
		}

		if (pItem != 0) {
			pItem->Init(pNode);
			guiList.push_back(pItem);
		}

		pNode = pNode->NextSibling("item");
	}
}

// frelement.cpp:121
void FrGuiItem::Init(const TiXmlNode* pSrc) {
	TiXmlElement* pElement = pSrc->ToElement();

	std::string aType = pElement->Attribute("type");
	std::string aRect = pElement->Attribute("rect");
	std::string aPos = pElement->Attribute("pos");

	memset(&m_rect, 0, sizeof(m_rect));
	if (aPos.length() == 0) {
		int v[4];
		sscanf(aRect.c_str(), "%d %d %d %d", &v[0], &v[1], &v[2], &v[3]);

		m_rect.left = v[0];
		m_rect.top = v[1];
		m_rect.right = v[2];
		m_rect.bottom = v[3];
	} else {
		int v[2];
		sscanf(aPos.c_str(), "%d %d", &v[0], &v[1]);

		m_rect.left = v[0];
		m_rect.top = v[1];
		m_rect.right = v[0] + 10;
		m_rect.bottom = v[1] + 10;
	}

	m_type = GuiType[aType];
	m_resource = pElement->Attribute("resource");
	m_name = pElement->Attribute("name");
	m_caption = pElement->Attribute("caption");

	pElement->Attribute("flag", (int*)&m_flag);

	TiXmlNode* pParam = pSrc->FirstChild("param");
	while (pParam != 0) {
		TiXmlElement* pParamElement = pParam->ToElement();

		std::string aName = pParamElement->Attribute("name");
		std::string aVar = pParamElement->Attribute("var");
		if (aName.length() != 0 && aVar.length() != 0) {
			m_param[aName] = aVar;
		}
		pParam = pParam->NextSibling();
	}
}

// frelement.cpp:192
const std::list<FrGuiItem*>* FrGuiItem::GetChildList(void) {
	return 0;
}

// frelement.cpp:197
FrGuiItemNested::~FrGuiItemNested(void) {
	sequence_delete(m_childList.begin(), m_childList.end());
	m_childList.clear();
}

// frelement.cpp:203
void FrGuiItemNested::Init(const TiXmlNode* pSrc) {
	FrGuiItem::Init(pSrc);

	CreateGuiItem(pSrc, m_childList);
}

// frelement.cpp:210
const std::list<FrGuiItem*>* FrGuiItemNested::GetChildList(void) {
	return &m_childList;
}

// frelement.cpp:261
void FrElementFrame::Init(const TiXmlNode* pSrc) {
	std::string color = pSrc->ToElement()->Attribute("color");
	sscanf(color.c_str(), "%x", &m_color);

	TiXmlNode* pLayer = pSrc->FirstChild("layer");
	m_layers = 0;
	while (pLayer != 0 && m_layers < sizeof(m_aType) / sizeof(m_aType[0])) {
		m_aInvisible[m_layers] = 0;
		TiXmlElement* pElement = pLayer->ToElement();
		if (pElement != 0) {
			pElement->Attribute("type", &m_aType[m_layers]);
			pElement->Attribute("height", &m_aHeight[m_layers]);
			pElement->Attribute("pos", &m_aPos[m_layers]);
			pElement->Attribute("invisible", &m_aInvisible[m_layers]);

			int bottomPos;
			pElement->Attribute("bottompos", &bottomPos);
			if (bottomPos != 0) {
				m_aPos[m_layers] = -bottomPos;
			}
		}

		pLayer = pLayer->NextSibling("layer");
		m_layers++;
	}

	TiXmlNode* pNode = pSrc->FirstChild("bfrm");
	if (pNode != 0) {
		TiXmlElement* pElement = pNode->ToElement();
		if (pElement != 0) {
			m_bfrmName = pElement->Attribute("filename");
		}
	}

	pNode = pSrc->FirstChild("sfrm");
	if (pNode != 0) {
		TiXmlElement* pElement = pNode->ToElement();
		if (pElement != 0) {
			m_sfrmName = pElement->Attribute("filename");
		}
	}

	pNode = pSrc->FirstChild("cfrm");
	if (pNode != 0) {
		TiXmlElement* pElement = pNode->ToElement();
		if (pElement != 0) {
			m_cfrmName = pElement->Attribute("filename");
		}
	}
}

// frelement.cpp:316
void FrElementLayout::Init(const TiXmlNode* pSrc) {
	TiXmlNode* pBase = pSrc->FirstChild("base");
	if (pBase != 0) {
		TiXmlElement* pElement = pBase->ToElement();
		pElement->Attribute("width", &m_bgSize.cx);
		pElement->Attribute("height", &m_bgSize.cy);

		char buff[256];
		sprintf(buff, "0x%s", pElement->Attribute("color").c_str());
		sscanf(buff, "%X", &m_bgColor);

		m_bgFilename = pElement->Attribute("background");

		CreateGuiItem(pSrc, m_guiList);

		TiXmlNode* pAniBg = pSrc->FirstChild("anibg");
		if (pAniBg != 0) {
			pElement = pAniBg->ToElement();

			sprintf(buff, "%s", pElement->Attribute("rect").c_str());
			int v[4];
			sscanf(buff, "%d %d %d %d", &v[0], &v[1], &v[2], &v[3]);

			m_aniBg.m_rect.left = v[0];
			m_aniBg.m_rect.top = v[1];
			m_aniBg.m_rect.right = v[2];
			m_aniBg.m_rect.bottom = v[3];

			m_aniBg.m_param["vel"] = pElement->Attribute("vel");
			m_aniBg.m_param["img"] = pElement->Attribute("img");
		}
	}
}

// frelement.cpp:367
void FrElementForm::Init(const TiXmlNode* pSrc) {
	m_caption = pSrc->ToElement()->Attribute("caption");
	m_desc = pSrc->ToElement()->Attribute("description");

	std::string bgSize = pSrc->ToElement()->Attribute("size");
	sscanf(bgSize.c_str(), "%d %d", &m_bgSize.w, &m_bgSize.h);
	m_bgFrame.m_resource = pSrc->ToElement()->Attribute("resource");

	short w = m_bgSize.w;
	short h = m_bgSize.h;
	m_bgFrame.m_rect.left = 0;
	m_bgFrame.m_rect.top = 0;
	m_bgFrame.m_rect.right = w;
	m_bgFrame.m_rect.bottom = h;
	m_bgFrame.m_type = GI_FRAME;
	m_bgFrame.m_flag = 1;

	CreateGuiItem(pSrc, m_guiList);
}

// frelement.cpp:397
void FrElementBitmap::Init(const TiXmlNode* pSrc) {
	m_bitmapFilename = pSrc->ToElement()->Attribute("filename");
}

// frelement.cpp:403
void FrElementIcon::Init(const TiXmlNode* pSrc) {
}

// frelement.cpp:407
void FrElementMacroItem::Init(const TiXmlNode* pSrc) {
	CreateGuiItem(pSrc, m_guiList);
}

// frelement.cpp:465
const Bitmap* FrElementDoc::GetBitmap(const std::string& resID) {
	if (resID.length() == 0) {
		return 0;
	}

	if (m_mapBitmap.find(resID) == m_mapBitmap.end()) {
		LoadPic(resID.c_str());

		if (m_mapBitmap.find(resID) == m_mapBitmap.end()) {
			return 0;
		}
	}

	return m_mapBitmap[resID].bitmap;
}

// frelement.cpp:483
void FrElementDoc::ClearBitmaps(void) {
	for (std::map<std::string, resBitmap_t>::iterator it = m_mapBitmap.begin(); it != m_mapBitmap.end(); ++it) {
		delete it->second.bitmap;
	}
	m_mapBitmap.clear();
}

// frelement.cpp:529
int FrElementDoc::LoadPic(const char* fileName) {
	enum { PIC_BMP, PIC_TGA };

	int picType = PIC_TGA;
	cFile* pFile = g_resrcmng->GetCFile(MakeStr("%s.tga", fileName), 0xFFFF);
	if (pFile == 0) {
		pFile = g_resrcmng->GetCFile(MakeStr("%s.bmp", fileName), 0xFFFF);
		if (pFile == 0) {
			return 0;
		}
		picType = PIC_BMP;
	}

	int fileSize = pFile->Length();
	char* pBuffer = new char[fileSize + 1];
	pFile->Read(pBuffer, fileSize);
	pBuffer[fileSize] = 0;
	CloseCFile(pFile);

	Bitmap* pBitmap;
	char* pBits;
	unsigned int height;
	int srcPitch;
	if (picType == PIC_BMP) {
		// BITMAPFILEHEADER + BITMAPINFOHEADER, bottom-up rows.
		pBits = pBuffer + 0x36;
		pBitmap = new Bitmap(*(int*)(pBuffer + 0x12), *(int*)(pBuffer + 0x16), *(short*)(pBuffer + 0x1c));

		height = *(int*)(pBuffer + 0x16);

		srcPitch = pBitmap->pitch;
	} else if (picType == PIC_TGA) {
		// TGA header, bottom-up rows.
		pBits = pBuffer + 0x12;
		pBitmap = new Bitmap(*(short*)(pBuffer + 0xc), *(short*)(pBuffer + 0xe), *(unsigned char*)(pBuffer + 0x10));

		height = *(short*)(pBuffer + 0xe);
		srcPitch = *(unsigned char*)(pBuffer + 0x10) * *(short*)(pBuffer + 0xc) / 8;
	} else {
		delete[] pBuffer;
		return 0;
	}

	for (unsigned int y = 0; y < height; y++) {
		memcpy(pBitmap->GetVram(y), pBits + (height - 1 - y) * srcPitch, srcPitch);
	}

	resBitmap_t res;
	res.bitmap = pBitmap;
	res.name = fileName;
	res.size = fileSize;

	m_mapBitmap[fileName] = res;

	delete[] pBuffer;

	return 1;
}

// frelement.cpp:618
template <class T>
struct sameElement {
	sameElement(enumElement type, const std::string& resID) : m_type(type), m_resID(resID) {}
	bool operator()(T pElement) {
		return pElement->m_type == m_type && pElement->m_name == m_resID;
	}

	const std::string& m_resID;                          // 0x0
	const enumElement m_type;                            // 0x4
};

// frelement.cpp:644
bool FrElementDoc::Load(const char* fileName) {
	std::list<FrElement*> elementList;

	ClearBitmaps();

	setlocale(LC_CTYPE, "Kor");

	m_xmlFileName = fileName;
	LoadXml(fileName, elementList);

	// Replace the previous document's elements.
	sequence_delete(m_elementList.begin(), m_elementList.end());
	m_elementList.clear();

	for (std::list<FrElement*>::iterator it = elementList.begin(); it != elementList.end(); ++it) {
		m_elementList.push_back(*it);
	}

	return true;
}

// frelement.cpp:672
bool FrElementDoc::LoadXml(const char* pszXmlFilename, std::list<FrElement*>& tempList) {
	TiXmlDocument xml;
	if (!xml.LoadFileEx(pszXmlFilename)) {
		return false;
	}

	TiXmlNode* pNode = xml.FirstChild("resource");
	if (pNode == 0) {
		return false;
	}

	int count;
	pNode->ToElement()->Attribute("count", &count);
	pNode = pNode->FirstChild("element");
	while (pNode != 0) {
		TiXmlElement* pXmlElement = pNode->ToElement();
		if (pXmlElement == 0) {
			pNode = pNode->NextSibling();
			continue;
		}

		std::string name = pXmlElement->Attribute("name");
		std::string type = pXmlElement->Attribute("type");
		enumElement elementType = ElementType[type];

		std::list<FrElement*>::iterator it = m_elementList.end();

		switch (elementType) {
		case ED_INCLUDE:
			if (!LoadXml(name.c_str(), tempList)) {
				return false;
			}
			break;
		case ED_FRAME:
		case ED_LAYOUT:
		case ED_FORM:
		case ED_MACROITEM:
			break;
		default:
			it = std::find_if(m_elementList.begin(), m_elementList.end(), sameElement<FrElement*>(elementType, name));
			break;
		}

		if (it != m_elementList.end()) {
			// Already loaded: keep the existing element.
			tempList.push_back(*it);
			m_elementList.erase(it);
		} else {
			FrElement* pElement = 0;
			switch (elementType) {
			case ED_FRAME:		pElement = new FrElementFrame;		break;
			case ED_LAYOUT:		pElement = new FrElementLayout;		break;
			case ED_BITMAP:		pElement = new FrElementBitmap;		break;
			case ED_FORM:		pElement = new FrElementForm;		break;
			case ED_MACROITEM:	pElement = new FrElementMacroItem;	break;
			}

			if (pElement != 0) {
				pElement->m_name = name;
				pElement->Init(pNode);
				tempList.push_back(pElement);
			}
		}

		pNode = pNode->NextSibling();
	}

	return true;
}

// frelement.cpp:768
FrElementDoc::~FrElementDoc(void) {
	sequence_delete(m_elementList.begin(), m_elementList.end());
	m_elementList.clear();

	ClearBitmaps();
}

// frelement.cpp:776
FrElementLayout* FrElementDoc::GetLayout(const std::string& resID) {
	std::list<FrElement*>::iterator it;
	it = std::find_if(m_elementList.begin(), m_elementList.end(), sameElement<FrElement*>(ED_LAYOUT, resID));
	if (it != m_elementList.end()) {
		return static_cast<FrElementLayout*>(*it);
	}

	return 0;
}

// frelement.cpp:788
FrElementFrame* FrElementDoc::GetFrame(const std::string& resID) {
	return FindElement<std::list<FrElement*>, FrElementFrame>(m_elementList, ED_FRAME, resID);
}

// frelement.cpp:795
FrElementForm* FrElementDoc::GetForm(const std::string& resID) {
	return FindElement<std::list<FrElement*>, FrElementForm>(m_elementList, ED_FORM, resID);
}

// frelement.cpp:802
FrElementMacroItem* FrElementDoc::GetMacroItem(const std::string& resID) {
	return FindElement<std::list<FrElement*>, FrElementMacroItem>(m_elementList, ED_MACROITEM, resID);
}
