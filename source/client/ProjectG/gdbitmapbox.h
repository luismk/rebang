// Original: d:\build\custom\temp\code\client\projectg\gdbitmapbox.h
#ifndef PG_CLIENT_PROJECTG_GDBITMAPBOX_H
#define PG_CLIENT_PROJECTG_GDBITMAPBOX_H

#include <map>
#include <string>
#include <string.h>

#include "igamedata.h"
#include "../client/Wangreal/include/wtypes.h"

// One "bitmapbox" row: image file and source rectangle. Size 0x50.
struct sGD_BITMAP {
	sGD_BITMAP(void) {                           // gdbitmapbox.h:8
		memset(szFileName, 0, sizeof(szFileName)); // gdbitmapbox.h:10
	}

	char szFileName[64]; // 0x0
	WRect rect;          // 0x40
};
CHECK_SIZE(sGD_BITMAP, 0x50);
CHECK_FIELD(sGD_BITMAP, rect, 0x40);

// "bitmapbox" game data: bitmaps by key. Size 0x10.
class CGDBitmapBox : public IGameData {
	LAYOUT_CHECK_FRIEND;
public:
	DECLARE_GAMEDATA(CGDBitmapBox, eGAMEDATADB_BITMAPBOX, "bitmapbox") // gdbitmapbox.h:20

	CGDBitmapBox(void);
	virtual ~CGDBitmapBox(void);

	virtual void Initialize(TiXmlNode* pNode);
	const char* GetFileName(char* szKey);
	void GetRect(const char* szKey, _WRECT& rect);

private:
	std::map<std::string, sGD_BITMAP> m_mapBitmap; // 0x4
};
CHECK_SIZE(CGDBitmapBox, 0x10);
BEGIN_LAYOUT_CHECK(CGDBitmapBox)
	CHECK_FIELD(CGDBitmapBox, m_mapBitmap, 0x4);
END_LAYOUT_CHECK

#endif
