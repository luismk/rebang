// Original: d:\build\custom\temp\code\client\projectg\ground_info.h
#ifndef PG_CLIENT_PROJECTG_GROUND_INFO_H
#define PG_CLIENT_PROJECTG_GROUND_INFO_H

#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "fx.h"
#include "../client/Wangreal/include/wmath.h"
#include "../shared/classdefine.h"

class WPuppet;

// The ground information base of the round's UI: the balls of the players
// (a list of sBall, one per ball type id), the condition ring and its
// effect box. Constructed by gui_control (which emits SetAngle and the
// scalar deleting destructor).
class CBase {
	LAYOUT_CHECK_FRIEND;

public:
	// One loaded ball (8 bytes, `new` of AddBall).
	struct sBall {
		// ground_info.h:14
		sBall(void) {
			m_pData = 0;
			m_pPuppet = 0;
		}

		IFF_STRUCT::sBall* m_pData;   // 0x0, the item record (CItemManager::FindBall)
		WPuppet* m_pPuppet;           // 0x4, "<name>.pet" of g_resrcmng
	};

	// find_if predicate: the ball of a type id.
	struct sFindBall {
		// ground_info.h:22
		sFindBall(unsigned long typeId) {
			m_typeId = typeId;
		}
		// ground_info.h:25
		bool operator()(sBall* pBall) {
			return pBall->m_pData->m_typeId == m_typeId;
		}

		unsigned long m_typeId;       // 0x0
	};

	CBase(void);
	~CBase(void);

	// ground_info.h:44
	void SetAngle(float angle) {
		m_angle = angle;
	}

	void Load(void);
	void ResetUiPosition(void);
	void Process(float elapsed);
	WMatrix GetBaseMatrix(void);
	void SetNormal(const WVector& normal);
	void Display(void);
	sBall* ChangeBall(unsigned long typeId);
	void ChangeTurn(void);

protected:
	void DrawRing(void);
	sBall* FindBall(unsigned long typeId);
	void AttachFx(void);
	sBall* AddBall(unsigned long typeId);

	std::list<sBall*> m_balls;    // 0x0
	sBall* m_pCurBall;            // 0xC, the ball shown (ChangeBall, ChangeTurn)
	float m_screenX;              // 0x10, effect box screen center x (58)
	float m_screenY;              // 0x14, effect box screen center y (COption::gGetBar_Y + 4)
	UNKNOWN(0x18, 0x1C);
	WVector m_normal;             // 0x1C, the ground normal under the ball (SetNormal), zero at construction
	WMatrix m_uiMatrix;           // 0x28, identity, pivot (0, 0, -1.8) * SCREEN_XS / 640
	int m_unknown58;              // 0x58, 2 at construction
	WVector m_lightDir;           // 0x5C, normalized (-1, -1, 1) at construction
	unsigned long m_color68;      // 0x68, 0xFFFFFF at construction
	unsigned long m_color6C;      // 0x6C, 0x808080 at construction
	unsigned long m_color70;      // 0x70, 0xFFFFFF
	float m_angle;                // 0x74, SetAngle
	int m_hConditionLine;         // 0x78, "condition_line.png" texture handle of g_resrcmng
	int m_hConditionFont;         // 0x7C, "font_conditoin.jpg" texture handle of g_resrcmng
	int m_lieIconIndex;           // 0x80, 12 at construction; SetNormal: 12 without power loss, 16 with
	int m_powerLoss;              // 0x84, percent of power lost on the ball's ground (SetNormal: 100 - GetMaxPowerOnGround * 100, 0 on ground type 2)
	WVector2D m_spin;             // 0x88, the "Spin" actor fills it (message 0x161)
	WMatrix m_matrix90;           // 0x90, identity at construction
	float m_unknownC0;            // 0xC0, GetBaseMatrix turns the matrix by -2 times it about axis 1
	CPvsFxBox m_fxBox;            // 0xC4
	char* m_pSlots[4];            // 0x428, the four 0x28-byte buffers below (third and fourth swapped)
	int m_slotCount;              // 0x438
	char m_slot0[0x28];           // 0x43C
	char m_slot1[0x28];           // 0x464
	char m_slot3[0x28];           // 0x48C
	char m_slot2[0x28];           // 0x4B4
};

BEGIN_LAYOUT_CHECK(CBase)
	CHECK_FIELD(CBase, m_balls, 0x0);
	CHECK_FIELD(CBase, m_pCurBall, 0xC);
	CHECK_FIELD(CBase, m_screenX, 0x10);
	CHECK_FIELD(CBase, m_screenY, 0x14);
	CHECK_FIELD(CBase, m_normal, 0x1C);
	CHECK_FIELD(CBase, m_uiMatrix, 0x28);
	CHECK_FIELD(CBase, m_unknown58, 0x58);
	CHECK_FIELD(CBase, m_lightDir, 0x5C);
	CHECK_FIELD(CBase, m_color68, 0x68);
	CHECK_FIELD(CBase, m_color6C, 0x6C);
	CHECK_FIELD(CBase, m_color70, 0x70);
	CHECK_FIELD(CBase, m_angle, 0x74);
	CHECK_FIELD(CBase, m_hConditionLine, 0x78);
	CHECK_FIELD(CBase, m_hConditionFont, 0x7C);
	CHECK_FIELD(CBase, m_lieIconIndex, 0x80);
	CHECK_FIELD(CBase, m_powerLoss, 0x84);
	CHECK_FIELD(CBase, m_spin, 0x88);
	CHECK_FIELD(CBase, m_matrix90, 0x90);
	CHECK_FIELD(CBase, m_unknownC0, 0xC0);
	CHECK_FIELD(CBase, m_fxBox, 0xC4);
	CHECK_FIELD(CBase, m_pSlots, 0x428);
	CHECK_FIELD(CBase, m_slotCount, 0x438);
	CHECK_FIELD(CBase, m_slot0, 0x43C);
	CHECK_FIELD(CBase, m_slot1, 0x464);
	CHECK_FIELD(CBase, m_slot3, 0x48C);
	CHECK_FIELD(CBase, m_slot2, 0x4B4);
END_LAYOUT_CHECK
CHECK_SIZE(CBase::sBall, 0x8);
CHECK_SIZE(CBase::sFindBall, 0x4);

#endif // PG_CLIENT_PROJECTG_GROUND_INFO_H
