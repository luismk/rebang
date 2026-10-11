// Original: d:\build\custom\temp\code\client\projectg\uccbrush.h
#ifndef PG_CLIENT_PROJECTG_UCCBRUSH_H
#define PG_CLIENT_PROJECTG_UCCBRUSH_H

#include "..\..\defines.h"
#include "bitmap.h"
#include <vector>

// Brush kind of CUccBrush::Initialize; enumerators not recovered yet.
enum eBrushType {
};

// The brush images of one UCC tool. Size 0x14 (UccBrushList's array
// stride); the constructor initialises only the vector.
class CUccBrush {
	LAYOUT_CHECK_FRIEND;
public:
	CUccBrush(void);
	~CUccBrush(void);

	void Initialize(eBrushType type);
	void Clear(void);
	Bitmap* GetBrush(int index);
	void SetBrush(int index, Bitmap* pBrush);
	void AddBrush(Bitmap* pBrush);
	void RemoveBrush(int index);
	int GetBrushNum(void);                     // uccbrush.h:41

private:
	Bitmap* LoadBrushFromFile(const char* fileName);

	std::vector<Bitmap*> m_brushes;   // 0x0
	eBrushType m_type;                // 0x10
};

CHECK_SIZE(CUccBrush, 0x14);

CUccBrush* UccBrushList(eBrushType type);
BEGIN_LAYOUT_CHECK(CUccBrush)
	CHECK_FIELD(CUccBrush, m_brushes, 0x0);
	CHECK_FIELD(CUccBrush, m_type, 0x10);
END_LAYOUT_CHECK

// The brush the UCC paint tools draw with: a 24x24 copy of a brush image
// tinted with the current colour. Size 0x20 (UccCurrentBrush's static).
struct sUccCurrentBrush {
	LAYOUT_CHECK_FRIEND;
public:
	sUccCurrentBrush(void);

	void SetBrush(eBrushType type, Bitmap* pBrush);
	void SetBrush(eBrushType type, int index, unsigned char r, unsigned char g, unsigned char b);
	void SetBrush(eBrushType type, int index);
	void SetBrush(unsigned char r, unsigned char g, unsigned char b);
	void GetBrushColor(unsigned char& r, unsigned char& g, unsigned char& b);

	Bitmap m_brush;            // 0x0
	eBrushType m_brushType;    // 0x14
	int m_brushIndex;          // 0x18
	unsigned char m_r;         // 0x1c
	unsigned char m_g;         // 0x1d
	unsigned char m_b;         // 0x1e
};

CHECK_SIZE(sUccCurrentBrush, 0x20);
BEGIN_LAYOUT_CHECK(sUccCurrentBrush)
	CHECK_FIELD(sUccCurrentBrush, m_brush, 0x0);
	CHECK_FIELD(sUccCurrentBrush, m_brushType, 0x14);
	CHECK_FIELD(sUccCurrentBrush, m_brushIndex, 0x18);
	CHECK_FIELD(sUccCurrentBrush, m_r, 0x1c);
	CHECK_FIELD(sUccCurrentBrush, m_g, 0x1d);
	CHECK_FIELD(sUccCurrentBrush, m_b, 0x1e);
END_LAYOUT_CHECK

sUccCurrentBrush* UccCurrentBrush(void);

#endif // PG_CLIENT_PROJECTG_UCCBRUSH_H
