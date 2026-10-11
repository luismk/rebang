// Original: d:\build\custom\temp\code\client\projectg\noticeimage.h
#ifndef PG_CLIENT_PROJECTG_NOTICEIMAGE_H
#define PG_CLIENT_PROJECTG_NOTICEIMAGE_H

#include <list>
#include <vector>

#include "..\..\defines.h"

// A named image of a notice file with its buttons, read by
// LoadNoticeImageFile (size 0x2c: its element count is divided by 0x2c).
struct sNoticeImage {
	// One "(%d %f %f %f %f)" entry of the file.
	struct sBn {
		int index;   // 0x0
		float x;     // 0x4
		float y;     // 0x8
		float w;     // 0xc
		float h;     // 0x10
	};

	// noticeimage.h:20
	sNoticeImage(void) {
		name[0] = 0;
	}

	char name[0x20];           // 0x0
	std::list<sBn> buttons;    // 0x20
};

CHECK_SIZE(sNoticeImage::sBn, 0x14);
CHECK_SIZE(sNoticeImage, 0x2c);
CHECK_FIELD(sNoticeImage, name, 0x0);
CHECK_FIELD(sNoticeImage, buttons, 0x20);

void LoadNoticeImageFile(const char* fileName, std::vector<sNoticeImage>& images, int count, const char* section);

#endif // PG_CLIENT_PROJECTG_NOTICEIMAGE_H
