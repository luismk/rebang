// Original: d:\build\custom\temp\code\client\projectg\ucccolorpicker.h
#ifndef PG_CLIENT_PROJECTG_UCCCOLORPICKER_H
#define PG_CLIENT_PROJECTG_UCCCOLORPICKER_H

#include <stddef.h>

#include "..\..\defines.h"
#include "UccBase.h"

// Colour map of the UCC editor: a CUccBase showing ucc_color_map.tga, whose
// GetColor reads the colour under a point. Size 0x8c (three colour bytes past
// CUccBase). Slots 18-19 follow CUccBase's 18 (vtable ??_7CUccColorPicker@@6B@).
class CUccColorPicker : public CUccBase {
	LAYOUT_CHECK_FRIEND;
public:
	CUccColorPicker(void);
	virtual ~CUccColorPicker(void);

	virtual bool IsInitialized(void) { return m_pArea != NULL && m_pTexture32 != NULL; }   // ucccolorpicker.h:17
	virtual void Process(void);     // ucccolorpicker.h:18
	virtual void MouseEventHandler(eButton button, int state, const WPoint &point);   // ucccolorpicker.h:20
	virtual void DestroyVariables(void);
	virtual void Initialize(FrArea* pArea);   // slot 18

	void GetColor(const WPoint& point, unsigned char& red, unsigned char& green, unsigned char& blue);

protected:
	virtual void Refresh2D(void);   // ucccolorpicker.h:27
	virtual void Refresh3D(void);   // ucccolorpicker.h:28
	virtual void GenerateColorMap(void);   // slot 19

	void GetColor(int x, int y, unsigned char& red, unsigned char& green, unsigned char& blue);

	unsigned char m_red;     // 0x88, the colour last picked
	unsigned char m_green;   // 0x89
	unsigned char m_blue;    // 0x8a
};

CHECK_SIZE(CUccColorPicker, 0x8c);
BEGIN_LAYOUT_CHECK(CUccColorPicker)
	CHECK_FIELD(CUccColorPicker, m_red, 0x88);
	CHECK_FIELD(CUccColorPicker, m_green, 0x89);
	CHECK_FIELD(CUccColorPicker, m_blue, 0x8a);
END_LAYOUT_CHECK

// Brightness bar beside the colour map: GenerateBrightnessMap fills a bitmap
// the size of the area with the shades of one colour.
class CUccColorBrightness : public CUccColorPicker {
public:
	CUccColorBrightness(void);
	virtual ~CUccColorBrightness(void);

	virtual bool IsInitialized(void) { return m_pArea != NULL && m_pTexture32 != NULL; }   // ucccolorpicker.h:43
	virtual void Initialize(FrArea* pArea);

	void GenerateBrightnessMap(unsigned char red, unsigned char green, unsigned char blue);

private:
	virtual void GenerateColorMap(void);   // ucccolorpicker.h:48
};

CHECK_SIZE(CUccColorBrightness, 0x8c);

#endif // PG_CLIENT_PROJECTG_UCCCOLORPICKER_H
