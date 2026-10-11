// Original: d:\build\custom\temp\code\client\projectg\realmyroomdata.h
#ifndef PG_CLIENT_PROJECTG_REALMYROOMDATA_H
#define PG_CLIENT_PROJECTG_REALMYROOMDATA_H

#include <list>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frcmdtarget.h"
#include "polysoup.h"
#include "singleton.h"
#include "wlist.h"
#include "wmath.h"

// A piece of furniture of the room (0x1B bytes, packed: copies move 6
// dwords, a word and a byte; the std::list node holds 0x24). Its original
// header is not identified yet; only the members the transcribed functions
// read are named.
#pragma pack(push, 1)
struct sFurniture_List {
	unsigned long m_id;           // 0x00, item serial
	unsigned long m_typeId;       // 0x04
	unsigned short m_number;      // 0x08, copied with the record (AddModel); a scalar, an array here would bring a /GS cookie to every local copy
	WVector m_pos;                // 0x0A, its position in the room (IsFunritureValueChange)
	float m_angle;                // 0x16, its rotation in degrees (cModel::SetRotate)
	unsigned char m_flags;        // 0x1A, bit 0: placed in the room
};
#pragma pack(pop)
CHECK_SIZE(sFurniture_List, 0x1B);
CHECK_FIELD(sFurniture_List, m_id, 0x00);
CHECK_FIELD(sFurniture_List, m_typeId, 0x04);
CHECK_FIELD(sFurniture_List, m_number, 0x08);
CHECK_FIELD(sFurniture_List, m_pos, 0x0A);
CHECK_FIELD(sFurniture_List, m_angle, 0x16);
CHECK_FIELD(sFurniture_List, m_flags, 0x1A);

class cModel {
public:
	cModel();
	~cModel();

	void SetPosition(WVector const &pos);
	void SetRotate(float angle);
	void Apply_ChangeModelData();
	void OBBToAABB();

	int m_state;                        // 0x0 (1 once constructed)
	sFurniture_List *m_pFurniture;      // 0x4
	WPolySoup::sModel *m_pModelData;    // 0x8
	float m_angle;                      // 0xC (degrees, wrapped at 360)
	int m_handle;                       // 0x10 (-1)
};
CHECK_SIZE(cModel, 0x14);
CHECK_FIELD(cModel, m_state, 0x0);
CHECK_FIELD(cModel, m_pFurniture, 0x4);
CHECK_FIELD(cModel, m_pModelData, 0x8);
CHECK_FIELD(cModel, m_angle, 0xC);
CHECK_FIELD(cModel, m_handle, 0x10);

// The furniture of the player's room.
class CRealMyRoomData : public WSingleton<CRealMyRoomData>, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
	friend class CRealMyRoomMain;
public:
	CRealMyRoomData(void);
	virtual ~CRealMyRoomData(void);

	void OnDestory(void);
	void Process(float elapsed);
	void Render(void);
	cModel *CreateModel(const char *name);
	cModel *CreateModel(const char *name, WVector pos, float angle, bool bRename, short type);
	cModel *ModelByRayIntersectOBB(WVector const &pos);
	cModel *GetModelData(unsigned long id, unsigned long typeId);
	cModel *ModelByRayIntersect(WVector const &origin, WVector const &dir);
	void MoveModelPosition(unsigned char mode);
	void ListFurnitureSelect(sFurniture_List *pList);
	unsigned long GetState(void);
	void SetState(unsigned long state);
	void DeleteList(void);
	void SetTargetPosition(WVector const &pos, unsigned char mode);
	bool IsArragementFurniture(unsigned long id);
	bool IsModelData_SaveList(unsigned long id, unsigned long typeId);
	void ResetList(bool all);
	int IsFunritureValueChange(cModel &model);
	void ChangeWallTexture(sFurniture_List *pList, bool bReset);
	bool ChangePosterTexture(unsigned long typeId, WPolySoup::sModel &model);
	void ChangeFloorTexture(sFurniture_List *pList, bool bReset);
	void SetRotate(float angle);
	WVector ChangeModelPosition(int mode, cModel *pModel, WPolySoup::sModel *pData);
	WVector SlideWallPosition(cModel *pModel, WPolySoup::sModel *pData);
	void SetModelPosition(void);
	bool AddModel(sFurniture_List furniture);
	void ResotreList(void);

protected:
	WList<cModel *> m_modelList;                  // 0x2C
	std::list<sFurniture_List> m_furnitureList;   // 0x4C
	cModel *m_pSelectModel;                       // 0x58, the model being edited
	cModel *m_pPickedModel;                       // 0x5C, cleared with the selection (DeleteList)
	cModel m_editModel;                           // 0x60
	WVector m_targetPos;                          // 0x74, SetTargetPosition's position
	WVector m_pickPos;                            // 0x80, the selection's position when picked (mode 1)
	WVector m_dragPos;                            // 0x8C, the position of a drag (mode 2)
	bool m_bDropped;                              // 0x98, cleared on a drop (mode 3)
	unsigned long m_state;                        // 0x9C
};

BEGIN_LAYOUT_CHECK(CRealMyRoomData)
	CHECK_FIELD(CRealMyRoomData, m_modelList, 0x2C);
	CHECK_FIELD(CRealMyRoomData, m_furnitureList, 0x4C);
	CHECK_FIELD(CRealMyRoomData, m_pSelectModel, 0x58);
	CHECK_FIELD(CRealMyRoomData, m_pPickedModel, 0x5C);
	CHECK_FIELD(CRealMyRoomData, m_editModel, 0x60);
	CHECK_FIELD(CRealMyRoomData, m_targetPos, 0x74);
	CHECK_FIELD(CRealMyRoomData, m_pickPos, 0x80);
	CHECK_FIELD(CRealMyRoomData, m_dragPos, 0x8C);
	CHECK_FIELD(CRealMyRoomData, m_bDropped, 0x98);
	CHECK_FIELD(CRealMyRoomData, m_state, 0x9C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_REALMYROOMDATA_H
