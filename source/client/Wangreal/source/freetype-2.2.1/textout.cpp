// Original: d:\build\custom\temp\code\client\wangreal\source\freetype-2.2.1\textout.cpp
#include <windows.h>
#include <mmsystem.h>
#include <string.h>

#include "textout.h"
#include "freetype2.h"
#include "../../include/wresrcmng.h" 
// textout.cpp:12
CTextOut::CTextOut(CRenderer* renderer)
	: m_pFontSet(NULL), m_pRenderer(renderer) {
	m_iFontSize = 0;

	m_WorkTextureNum = 2;
	m_WorkTextureCnt = 0;
	m_WorkX = 0;
	m_WorkY[0] = 0;
	m_WorkY[1] = 0;
	m_BeginWorkTextureCnt = 0;
	m_bResetTexture = false;
	m_RecentCheckTime = timeGetTime();

	m_WorkImage.Create(256, 256, 32);
	memset(m_WorkImage.vram, 0xff, m_WorkImage.Size());
	for (int y = 0; y < 256; y++) {
		for (int x = 0; x < 256; x++) {
			m_WorkImage.vram[m_WorkImage.pitch * y + x * 4 + 3] = 0;
		}
	}
}

// textout.cpp:40
CTextOut::~CTextOut(void) {
	if (m_pFontSet)
		delete m_pFontSet;
}

// textout.cpp:47
bool CTextOut::Load(const char* fname, int size, int fontindex, bool preload) {
	CFreeType2* pFontSet = new CFreeType2;
	if (!pFontSet->Load(fname, size, fontindex, preload)) {
		delete pFontSet;
		return false;
	}
	m_pFontSet = pFontSet;
	m_iFontSize = size;
	return true;
}

// textout.cpp:61
void CTextOut::SetFontSize(int fntsize) {
	m_iFontSize = fntsize;
}

// textout.cpp:67
int CTextOut::GetWidth(const char* str) {
	_TEXTSET* set = GetTextSet(str, m_iFontSize);
	return set->width;
}

// textout.cpp:78
int CTextOut::Print(float x, float y, const char* str, unsigned long rs, unsigned long diffuse) {
	if (str == NULL || *str == '\0')
		return 0;

	_DISPLAYLIST d;
	_TEXTSET* set = GetTextSet(str, m_iFontSize);

	set->lastrendertick = timeGetTime();

	d.set = set;
	d.x = x;
	d.y = y;
	d.rs = rs;
	d.diffuse = diffuse;

	m_DisplayList.push_back(d);
	return set->width;
}

// textout.cpp:105
void CTextOut::Flush(WView* view) {
	if (m_DisplayList.empty())
		return;

	do {
		if (m_bResetTexture == true) {
			Reset();
			m_WorkTextureNum++;
			m_bResetTexture = false;
		}

		for (unsigned int i = 0; i < m_DisplayList.size(); i++) {
			if (!m_DisplayList[i].set->valid)
				UpdateText(m_DisplayList[i].set);
		}
	} while (m_bResetTexture == true);

	if (m_bUpdateTexture)
		UpdateTexture();

	for (unsigned int i = 0; i < m_DisplayList.size(); i++) {
		_DISPLAYLIST& d = m_DisplayList[i];
		WRect src;
		WRect dest;

		dest.x = d.x;
		dest.y = d.y;

		for (_IMAGESET* ptr = d.set->ptr; ptr != NULL; ptr = ptr->next) {
			src.x = (float)ptr->x / 256.0f;
			src.y = (float)ptr->y / 256.0f;
			src.w = (float)ptr->w / 256.0f;
			src.h = (float)ptr->h / 256.0f;
			dest.w = (float)ptr->w;
			dest.h = (float)ptr->h;
			DrawTexture(view, m_TextList[ptr->idx], src, dest, d.rs, d.diffuse, 0.0f, 0, true);

			dest.x += (float)ptr->w;
		}
	}

	m_DisplayList.clear();

	m_BeginWorkTextureCnt = m_WorkTextureCnt;

	unsigned long curtime = timeGetTime();
	if (curtime - m_RecentCheckTime > 5000) {
		list::multimap_str<_TEXTSET*>::iterator it = m_TextSet.begin();
		while (it != m_TextSet.end()) {
			if (curtime - it->second->lastrendertick > 10000) {
				_IMAGESET* img = it->second->ptr;
				while (img != NULL) {
					_IMAGESET* p = img;
					img = img->next;
					delete p;
				}
				delete it->second;

				m_TextSet.erase(it);
				it = m_TextSet.begin();
			} else {
				++it;
			}
		}
		m_RecentCheckTime = curtime;
	}
}

// textout.cpp:203
CTextOut::_TEXTSET* CTextOut::GetTextSet(const char* text, int fontsize) {
	list::multimap_str<_TEXTSET*>::iterator ii;
	ii = m_TextSet.find(text);

	if (ii != m_TextSet.end()) {
		list::multimap_str<_TEXTSET*>::iterator ub = m_TextSet.upper_bound(text);

		for (; ii != ub; ++ii) {
			if (ii->second->fntsize == fontsize)
				return ii->second;
		}
	}

	_TEXTSET* set = (_TEXTSET*)new char[strlen(text) + sizeof(_TEXTSET)];

	set->valid = false; set->width = 0; set->fntsize = fontsize;
	set->lastrendertick = timeGetTime();
	set->ptr = NULL;

	strcpy(set->text, text);

	UpdateText(set);

	m_TextSet.insert(std::pair<const char* const, _TEXTSET*>(set->text, set));
	return set;
}

// textout.cpp:238
void CTextOut::UpdateText(_TEXTSET* textset) {
	_IMAGESET* root = NULL;
	_IMAGESET* cur = NULL;
	int wlen;
	_IMAGESET set;
	unsigned short code;
	wchar_t Uni[16];

	set.idx = m_WorkTextureCnt;

	set.x = m_WorkX; set.h = 0; int w = 0; int h = 0; wlen = 0; set.y = m_WorkY[0];

	m_pFontSet->ChangFontSize(textset->fntsize);

	const char* text = textset->text;
	for (int i = 0; text[i] != '\0'; ) {

		if (text[i] & 0x80) {

			code = (unsigned char)text[i] | ((unsigned char)text[i + 1] << 8);


			MultiByteToWideChar(0, 0, (const char*)&code, 2, Uni, 1);
			code = Uni[0];
			m_pFontSet->Render(code, &w, &h);
			i += 2;
		} else {


			m_pFontSet->Render(text[i], &w, &h);
			i++;
		}

		if (m_WorkX + w > 256) {

			set.w = m_WorkX - set.x;


			if (set.w != 0) {

				_IMAGESET* p = new _IMAGESET;
				wlen += set.w;
				*p = set;
				p->next = NULL;
				if (cur == NULL)
					root = cur = p;
				else {
					cur->next = p; cur = p;
				}
				m_bUpdateTexture = true;
			}


			m_WorkX = 0; set.x = 0; set.h = 0; m_WorkY[0] = m_WorkY[1]; set.y = m_WorkY[0];
		}

		if (m_WorkY[0] + h > 256 || m_WorkY[0] + set.h > 256) {

			set.w = m_WorkX - set.x;


			if (set.w != 0) {

				_IMAGESET* p = new _IMAGESET;
				wlen += set.w;
				*p = set;
				p->next = NULL;
				if (cur == NULL)
					root = cur = p;
				else {
					cur->next = p; cur = p;
				}
				m_bUpdateTexture = true;
			}

			UpdateTexture();
			UseNextTexture();

			set.idx = m_WorkTextureCnt;
			m_WorkX = 0; set.x = 0;
			m_WorkY[1] = 0; m_WorkY[0] = 0; set.y = 0; set.h = 0;
		}

		m_pFontSet->Write32a(m_WorkImage.vram + m_WorkY[0] * m_WorkImage.pitch + m_WorkX * 4, m_WorkImage.pitch);

		m_WorkX += w;
		m_bUpdateTexture = true;
		if (h > set.h) {

			set.h = h;
			if (m_WorkY[0] + h > m_WorkY[1])
				m_WorkY[1] = m_WorkY[0] + h;
		}
	}

	set.w = m_WorkX - set.x;


	if (set.w != 0) {

		wlen += set.w;
		_IMAGESET* p = new _IMAGESET;
		*p = set;

		p->next = NULL;
		if (cur == NULL)
			root = p;
		else
			cur->next = p;
		m_bUpdateTexture = true;
	}

	_IMAGESET* it = textset->ptr;
	while (it != NULL) {
		_IMAGESET* p = it;
		it = it->next;
		delete p;
	}

	textset->ptr = root;
	textset->valid = true;
	textset->width = wlen;
	textset->lastrendertick = timeGetTime();
}

// textout.cpp:369
void CTextOut::UpdateTexture(void) {
	if (m_bUpdateTexture == true) {
		if (m_WorkTextureCnt >= (int)m_TextList.size()) {
			m_TextList.push_back(GetResrcManager()->UploadTexture(NULL, &m_WorkImage, 0x10000000, NULL));
		}
		GetResrcManager()->FixTexture(m_TextList[m_WorkTextureCnt], &m_WorkImage, 0, NULL);
	}
	m_bUpdateTexture = false;
}

// textout.cpp:397
void CTextOut::UseNextTexture(void) {
	m_WorkTextureCnt = (m_WorkTextureCnt + 1) % m_WorkTextureNum;

	memset(m_WorkImage.vram, 0xff, m_WorkImage.Size());
	for (int y = 0; y < 256; y++) {
		for (int x = 0; x < 256; x++) {
			m_WorkImage.vram[m_WorkImage.pitch * y + x * 4 + 3] = 0;
		}
	}

	for (list::multimap_str<_TEXTSET*>::iterator it = m_TextSet.begin(); it != m_TextSet.end(); ++it) {
		_TEXTSET* set = it->second;
		_IMAGESET* img = set->ptr;
		while (img != NULL) {
			if (img->idx == m_WorkTextureCnt) {
				set->valid = false;
				break;
			}
			img = img->next;
		}
	}

	if (m_WorkTextureCnt == m_BeginWorkTextureCnt)
		m_bResetTexture = true;
}

// textout.cpp:431
void CTextOut::Reset(void) {
	for (list::multimap_str<_TEXTSET*>::iterator it = m_TextSet.begin(); it != m_TextSet.end(); ++it)
		it->second->valid = false;

	m_WorkTextureCnt = 0;
	m_WorkX = 0;
	m_WorkY[0] = 0;
	m_WorkY[1] = 0;
	m_bUpdateTexture = false;
	m_BeginWorkTextureCnt = 0;
}

// textout.cpp:447
void CTextOut::Clear(void) {
	Reset();

	for (list::multimap_str<_TEXTSET*>::iterator it = m_TextSet.begin(); it != m_TextSet.end(); ++it) {
		_IMAGESET* img = it->second->ptr;
		while (img != NULL) {
			_IMAGESET* p = img;
			img = img->next;
			delete p;
		}
		delete it->second;
	}

	m_TextSet.clear();

	m_DisplayList.clear();

	for (unsigned int i = 0; i < m_TextList.size(); i++)
		GetResrcManager()->Release(m_TextList[i]);

	m_TextList.clear();
}

// textout.cpp:484
float CTextOut::PrintInside(WView* view, float x, float y, const char* text, int type, unsigned long diffuse, Bitmap* bmp) {
	return (float)Print(x, y, text, type, diffuse);
}

// textout.cpp:489
float CTextOut::GetTextWidthInside(WView* view, const char* pText) {
	return (float)GetTextSet(pText, m_iFontSize)->width;
}
