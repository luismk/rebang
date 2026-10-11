// Original: d:\build\custom\temp\code\client\fresh\source\fremoticon.cpp
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>

#include "fremoticon.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "../client/ProjectG/binstr.h"
#include "../client/ProjectG/tinyxml.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wview.h"

// Initialisers of the guarded function-local separator pointers of the
// text routines (the original folds the literal into the guarded store).
static const char* const EMOTICON_LEFT = "(";
static const char* const EMOTICON_RITE = ")";
static const char* const EMOTICON_BOTH = "()";

// fremoticon.cpp:17
FrEmoticon::FrEmoticon(FrWndManager* pManager) {
	m_pManager = pManager;
	m_overlay = 0;
	m_EnableAnim = false;
	m_AnimStart = 0;
	m_selWidth = 26;
	m_selHeight = 24;
	m_selXNum = 9;
	m_selYNum = 10;
	m_selNum = 90;
}

// fremoticon.cpp:32
FrEmoticon::~FrEmoticon(void) {
	for (EMOTICONLIST_itr it = m_Emoticons.begin(); it != m_Emoticons.end(); ++it) {
		(*it)->Frames.clear();
		delete *it;
	}
	m_Emoticons.clear();

	for (RESMAP_itr itr = m_ResMap.begin(); itr != m_ResMap.end(); ++itr) {
		if (g_resrcmng && itr->second) {
			g_resrcmng->Release(itr->second);
			itr->second = 0;
		}
	}
	m_ResMap.clear();

	if (g_resrcmng && m_overlay) {
		g_resrcmng->Release(m_overlay);
		m_overlay = 0;
	}
}

// fremoticon.cpp:56
bool FrEmoticon::Init(void) {
	LoadXml("emoticon.xml");

	if (m_overlay == 0) {
		m_overlay = g_resrcmng->GetOverlay("emoticon.tga", 0);
	}
	return true;
}

// fremoticon.cpp:67
bool FrEmoticon::Draw(int icon, float x, float y, unsigned long diffuse, bool flip) {
	return Draw(icon, WRect(x, y, m_selWidth, m_selHeight), diffuse, flip);
}

// fremoticon.cpp:72
void FrEmoticon::SetClippingArea(WRect* clip) {
	m_pManager->GetGDI()->SetClippingArea(clip);
	m_overlay->SetClippingArea(clip);

	for (RESMAP_itr it = m_ResMap.begin(); it != m_ResMap.end(); ++it) {
		it->second->SetClippingArea(clip);
	}
}

// fremoticon.cpp:86
bool FrEmoticon::Draw(int icon, const WRect& dst, unsigned long diffuse, bool flip) {
	if (icon < 0)
		return false;

	FrGraphicInterface* pGdi = m_pManager->GetGDI();
	if (pGdi == 0)
		return false;

	unsigned long frame = (timeGetTime() - m_AnimStart) / (1000 / m_Fps);

	sEmoticon* emoticon = m_Emoticons[icon];
	int selected;
	if (m_EnableAnim && emoticon->Frames.size() != 0) {
		selected = emoticon->Frames[frame % emoticon->Frames.size()];
	} else {
		selected = icon;
	}

	int selX = selected % m_selXNum;
	int selY = (selected % m_selNum) / m_selXNum;

	WOverlay* overlay;
	if (m_EnableAnim && emoticon->Frames.size() != 0) {
		overlay = emoticon->Overlay;
	} else {
		overlay = m_overlay;
	}

	int width = overlay->GetWidth();
	int height = overlay->GetHeight();

	unsigned long color = ((int)(pGdi->GetAlpha() * 255.0f) << 24) | 0xffffff;

	if (flip) {
		overlay->Render(g_view, WRect((float)((selX + 1) * m_selWidth) / width, (float)(selY * m_selHeight) / height,
			(float)-m_selWidth / width, (float)m_selHeight / height), dst, 0, color, 0, 0);
	} else {
		overlay->Render(g_view, WRect((float)(selX * m_selWidth) / width, (float)(selY * m_selHeight) / height,
			(float)m_selWidth / width, (float)m_selHeight / height), dst, 0, color, 0, 0);
	}
	return true;
}

// fremoticon.cpp:137
float FrEmoticon::PrintText(const WPoint& pos, unsigned long align, const char* text, unsigned long emoDiffuse) {
	FrGraphicInterface* pGdi = m_pManager->GetGDI();
	if (pGdi == 0)
		return 0.0f;

	pGdi->SetTextColor((pGdi->GetTextColor() & 0xffffff) | (emoDiffuse & 0xff000000),
		(pGdi->GetTextOutlineColor() & 0xffffff) | (emoDiffuse & 0xff000000));

	WPoint p;
	int icon;
	switch (align) {
	case 0:
		p.x = pos.x;
		p.y = pos.y;
		break;
	case 1:
		p.x = pos.x - GetTextWidth(text) * 0.5f;
		p.y = pos.y;
		break;
	case 2:
		p.x = pos.x - GetTextWidth(text);
		p.y = pos.y;
		break;
	}

	cTokenV token;
	token.Init(text, strlen(text));

	static const char* pLeft = EMOTICON_LEFT;
	static const char* pRite = EMOTICON_RITE;
	static const char* pBoth = EMOTICON_BOTH;

	while (token.GetPos() < token.GetLen()) {
		char* pToken;
		bool ret = token.GetToken(&pToken, pLeft, 1);
		p.x += pGdi->PrintText(p, 0, pToken, 0);
		if (token.GetPos() >= token.GetLen()) {
			if (ret)
				p.x += pGdi->PrintText(p, 0, pLeft, 0);
			break;
		}

		for (;;) {
			ret = token.GetToken(&pToken, pBoth, 2);
			if (ret && token.GetPos() != 0 && token.GetBuf()[token.GetPos() - 1] != *pRite) {
				p.x += pGdi->PrintText(p, 0, pLeft, 0);
				p.x += pGdi->PrintText(p, 0, pToken, 0);
				continue;
			}
			break;
		}

		icon = GetIconIndex(pToken);
		if (ret && icon != -1) {
			bool flip = strstr(pToken, "-") != 0;
			int height = m_selHeight;
			float y = p.y - (height - pGdi->GetFontHeight());
			float h = height;
			float w = m_selWidth;
			Draw(icon, WRect(p.x, y, w, h), emoDiffuse, flip);
			p.x += m_selWidth;
		} else {
			p.x += pGdi->PrintText(p, 0, pLeft, 0);
			p.x += pGdi->PrintText(p, 0, pToken, 0);
			if (ret)
				p.x += pGdi->PrintText(p, 0, pRite, 0);
		}
	}
	return p.x - pos.x;
}

// fremoticon.cpp:247
float FrEmoticon::PrintText11(const WPoint& pos, unsigned long align, const char* text, unsigned long emoDiffuse) {
	FrGraphicInterface* pGdi = m_pManager->GetGDI();
	if (pGdi == 0)
		return 0.0f;

	pGdi->SetTextColor((pGdi->GetTextColor() & 0xffffff) | (emoDiffuse & 0xff000000),
		(pGdi->GetTextOutlineColor() & 0xffffff) | (emoDiffuse & 0xff000000));

	WPoint p;
	int icon;
	switch (align) {
	case 0:
		p.x = pos.x;
		p.y = pos.y;
		break;
	case 1:
		p.x = pos.x - GetTextWidth11(text) * 0.5f;
		p.y = pos.y;
		break;
	case 2:
		p.x = pos.x - GetTextWidth11(text);
		p.y = pos.y;
		break;
	}

	cTokenV token;
	token.Init(text, strlen(text));

	static const char* pLeft = EMOTICON_LEFT;
	static const char* pRite = EMOTICON_RITE;
	static const char* pBoth = EMOTICON_BOTH;

	while (token.GetPos() < token.GetLen()) {
		char* pToken;
		bool ret = token.GetToken(&pToken, pLeft, 1);
		p.x += pGdi->PrintText11(p, 0, pToken, 0);
		if (token.GetPos() >= token.GetLen()) {
			if (ret)
				p.x += pGdi->PrintText11(p, 0, pLeft, 0);
			break;
		}

		for (;;) {
			ret = token.GetToken(&pToken, pBoth, 2);
			if (ret && token.GetPos() != 0 && token.GetBuf()[token.GetPos() - 1] != *pRite) {
				p.x += pGdi->PrintText11(p, 0, pLeft, 0);
				p.x += pGdi->PrintText11(p, 0, pToken, 0);
				continue;
			}
			break;
		}

		icon = GetIconIndex(pToken);
		if (ret && icon != -1) {
			bool flip = strstr(pToken, "-") != 0;
			int height = m_selHeight;
			float y = p.y - (height - pGdi->GetFontHeight());
			float h = height;
			float w = m_selWidth;
			Draw(icon, WRect(p.x, y, w, h), emoDiffuse, flip);
			p.x += m_selWidth;
		} else {
			p.x += pGdi->PrintText11(p, 0, pLeft, 0);
			p.x += pGdi->PrintText11(p, 0, pToken, 0);
			if (ret)
				p.x += pGdi->PrintText11(p, 0, pRite, 0);
		}
	}
	return p.x - pos.x;
}

// fremoticon.cpp:343
float FrEmoticon::GetTextWidth(const char* text) {
	FrGraphicInterface* pGdi = m_pManager->GetGDI();
	if (pGdi == 0)
		return 0.0f;

	float width = 0.0f;
	cTokenV token;
	token.Init(text, strlen(text));

	static const char* pLeft = EMOTICON_LEFT;
	static const char* pRite = EMOTICON_RITE;
	static const char* pBoth = EMOTICON_BOTH;

	while (token.GetPos() < token.GetLen()) {
		char* pToken;
		bool ret = token.GetToken(&pToken, pLeft, 1);
		width += pGdi->GetTextExtend(pToken);
		if (token.GetPos() >= token.GetLen()) {
			if (ret)
				width += pGdi->GetTextExtend(pLeft);
			break;
		}

		for (;;) {
			ret = token.GetToken(&pToken, pBoth, 2);
			if (ret && token.GetPos() != 0 && token.GetBuf()[token.GetPos() - 1] != *pRite) {
				width += pGdi->GetTextExtend(pLeft);
				width += pGdi->GetTextExtend(pToken);
				continue;
			}
			if (ret && GetIconIndex(pToken) != -1) {
				width += m_selWidth;
				break;
			}
			width += pGdi->GetTextExtend(pLeft);
			width += pGdi->GetTextExtend(pToken);
			if (ret)
				width += pGdi->GetTextExtend(pRite);
			break;
		}
	}
	return width;
}

// fremoticon.cpp:425
float FrEmoticon::GetTextWidth11(const char* text) {
	FrGraphicInterface* pGdi = m_pManager->GetGDI();
	if (pGdi == 0)
		return 0.0f;

	float width = 0.0f;
	cTokenV token;
	token.Init(text, strlen(text));

	static const char* pLeft = EMOTICON_LEFT;
	static const char* pRite = EMOTICON_RITE;
	static const char* pBoth = EMOTICON_BOTH;

	while (token.GetPos() < token.GetLen()) {
		char* pToken;
		bool ret = token.GetToken(&pToken, pLeft, 1);
		width += pGdi->GetTextExtend11(pToken);
		if (token.GetPos() >= token.GetLen()) {
			if (ret)
				width += pGdi->GetTextExtend11(pLeft);
			break;
		}

		for (;;) {
			ret = token.GetToken(&pToken, pBoth, 2);
			if (ret && token.GetPos() != 0 && token.GetBuf()[token.GetPos() - 1] != *pRite) {
				width += pGdi->GetTextExtend11(pLeft);
				width += pGdi->GetTextExtend11(pToken);
				continue;
			}
			if (ret && GetIconIndex(pToken) != -1) {
				width += m_selWidth;
				break;
			}
			width += pGdi->GetTextExtend11(pLeft);
			width += pGdi->GetTextExtend11(pToken);
			if (ret)
				width += pGdi->GetTextExtend11(pRite);
			break;
		}
	}
	return width;
}

// fremoticon.cpp:504
int FrEmoticon::GetIconIndex(const char* key) {
	if (key == 0 || key[0] == 0) {
		return -1;
	}

	for (int i = 0; i < m_Emoticons.size(); i++) {
		sEmoticon* emoticon = m_Emoticons[i];
		if (_strcmpi(emoticon->Name.c_str(), key) == 0 || (key[0] == '-' && _strcmpi(emoticon->Name.c_str(), key + 1) == 0)) {
			return i;
		}
		for (int j = 0; j < 2; j++) {
			if (_strcmpi(emoticon->Alias[j].c_str(), key) == 0 || (key[0] == '-' && _strcmpi(emoticon->Alias[j].c_str(), key + 1) == 0)) {
				return i;
			}
		}
	}
	return -1;
}

// fremoticon.cpp:590
const char* FrEmoticon::GetIconName(int index) {
	return m_Emoticons[index]->Name.c_str();
}

// fremoticon.cpp:600
int FrEmoticon::GetIconNum(void) {
	return m_Emoticons.size();
}

// fremoticon.cpp:605
void FrEmoticon::GetAlias(std::string (*buffer)[2], int index) {
	if (strlen(m_Emoticons[index]->Alias[0].c_str()) != 0) {
		(*buffer)[0] = m_Emoticons[index]->Alias[0];
	}
	if (strlen(m_Emoticons[index]->Alias[1].c_str()) != 0) {
		(*buffer)[1] = m_Emoticons[index]->Alias[1];
	}
}

// fremoticon.cpp:619
void FrEmoticon::ParseAnimFrame(const std::string& Frames, std::vector<int>& Vector) {
	char Seps[2] = " ";
	char* token = strtok((char*)Frames.c_str(), Seps);

	while (token) {
		Vector.push_back(atoi(token));
		token = strtok(0, Seps);
	}
}

// fremoticon.cpp:632
bool FrEmoticon::LoadXml(const char* FileName) {
	TiXmlDocument Xml;
	if (m_Emoticons.size() != 0)
		return true;

	if (!Xml.LoadFileEx(FileName))
		return false;

	TiXmlNode* Node = Xml.FirstChild("emoticon");

	if (Node == 0)
		return false;

	Node->ToElement()->Attribute("fps", &m_Fps);

	TiXmlNode* DefNode = Node->FirstChild("def");

	while (DefNode) {
		TiXmlElement* Elem = DefNode->ToElement();

		sEmoticon* Emoticon = new sEmoticon;
		Emoticon->Name = Elem->Attribute("name");
		Emoticon->Alias[0] = Elem->Attribute("alias");
		Emoticon->Alias[1] = Elem->Attribute("alias2");

		TiXmlNode* AnimNode = DefNode->FirstChild("anim");

		if (AnimNode) {
			TiXmlElement* Anim = AnimNode->ToElement();

			std::string Res = Anim->Attribute("res");

			ParseAnimFrame(Anim->Attribute("frame"), Emoticon->Frames);

			RESMAP_itr i = m_ResMap.find(Res);

			if (i == m_ResMap.end()) {
				WOverlay* Overlay = g_resrcmng->GetOverlay(Res.c_str(), 0);

				m_ResMap[Res] = Overlay;
				Emoticon->Overlay = Overlay;
			} else {
				Emoticon->Overlay = m_ResMap[Res];
			}
		}

		m_Emoticons.push_back(Emoticon);

		DefNode = DefNode->NextSibling();
	}

	return true;
}
