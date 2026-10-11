// Original: d:\build\custom\temp\code\client\projectg\wind.h
#ifndef PG_CLIENT_PROJECTG_WIND_H
#define PG_CLIENT_PROJECTG_WIND_H

#include <vector>
#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"
#include "encrypttypes.h"

class WView;
class WPuppet;
class CFxSequence;

class CQuadTree;

class CWind {
	LAYOUT_CHECK_FRIEND;
	friend class CQuadTree;
public:
	struct sWind {
		sWind() {}

		unsigned long m_type;           // 0x0
		Waabb m_area;                   // 0x4
		WVector m_wind;                 // 0x1C
		char m_flag;                    // 0x28
		float m_intensity;              // 0x2C, read from the area's description
		float m_yaw;                    // 0x30, degrees, the RotMat about y
		float m_pitch;                  // 0x34, degrees, the RotMat about x
		long m_linked;                  // 0x38, m_type of an overlapping area, -1 if none
	};

	CWind(void);
	// wind.h:10
	virtual ~CWind(void) {}

	void Init(unsigned char, unsigned char, unsigned char, unsigned char);
	void SetSpecialWind(bool);
	void GenerateFx(bool);
	bool IsGlobalWind_Compare(void);
	// wind.h:48
	bool HasSpecialWind(void) { return m_specialIntensity > 0.0f; }
	bool IsActiveSpecialWind(void);   // wind.h:57
	void Display(WPuppet*, float, float);
	void CheckSpecialWind(void);
	WVector GetWind(const WVector& pos);
	WVector GetGlobalWind(void);
	float GetGlobalIntensity(void);
	float GetGlobalDirection(void);
	void SetIntensity(float);
	void SetDirection(float);
	bool IsInTurbulence(const WVector&);
	bool IsInTurbulence(const WVector&, const WVector&);
	void DrawWindArea(WView* view);
	void Process(float);
	void SetWind(const WVector& wind);
	void PlusWind(const WVector& wind);
	void MakeWindList(void);

protected:
	bool IsInclude(const Waabb&, const WVector&);
	void AddWindArea(const Waabb&, char*);

private:
	unsigned long m_specialStartFrame;      // 0x4, the quadtree frame the special wind began (CQuadTree::PreProcess)
	PADDING(0x8, 0x14);
	WCrypticValue<float> m_intensity;       // 0x14
	WCrypticValue<float> m_direction;       // 0x24
	PADDING(0x34, 0x38);
	WMatrix m_matrix;                       // 0x38
	int m_unk68;                            // 0x68
	WVector m_unk6C;                        // 0x6C
	unsigned long m_unk78;                  // 0x78
	unsigned long m_unk7C;                  // 0x7C
	unsigned long m_unk80;                  // 0x80
	PADDING(0x84, 0x88);
	float m_unk88;                          // 0x88
	float m_unk8C;                          // 0x8C
	WCrypticValue<bool> m_unk90;            // 0x90
	std::vector<sWind> m_windList;          // 0xA0
	WVector m_globalWind;                   // 0xB0
	WVector m_wind;                         // 0xBC
	WVector m_specialWind;                  // 0xC8
	WCrypticValue<float> m_specialIntensity;  // 0xD4
	WCrypticValue<float> m_specialDirection;  // 0xE4
	bool m_activeSpecialWind;               // 0xF4
	WMatrix m_fxMatrix;                     // 0xF8, the matrix the cannon effect is attached to (GenerateFx)
	WVector m_fxOffset;                     // 0x128, the offset the cannon effect is attached at (GenerateFx)
	CFxSequence* m_pFxSequence;             // 0x134, the cannon effect, null when closed
};
CHECK_SIZE(CWind::sWind, 0x3C);
CHECK_FIELD(CWind::sWind, m_area, 0x4);
CHECK_FIELD(CWind::sWind, m_wind, 0x1C);
CHECK_FIELD(CWind::sWind, m_flag, 0x28);
CHECK_FIELD(CWind::sWind, m_intensity, 0x2C);
CHECK_FIELD(CWind::sWind, m_linked, 0x38);
BEGIN_LAYOUT_CHECK(CWind)
	CHECK_FIELD(CWind, m_intensity, 0x14);
	CHECK_FIELD(CWind, m_direction, 0x24);
	CHECK_FIELD(CWind, m_matrix, 0x38);
	CHECK_FIELD(CWind, m_unk68, 0x68);
	CHECK_FIELD(CWind, m_unk6C, 0x6C);
	CHECK_FIELD(CWind, m_unk80, 0x80);
	CHECK_FIELD(CWind, m_unk88, 0x88);
	CHECK_FIELD(CWind, m_unk90, 0x90);
	CHECK_FIELD(CWind, m_windList, 0xA0);
	CHECK_FIELD(CWind, m_globalWind, 0xB0);
	CHECK_FIELD(CWind, m_wind, 0xBC);
	CHECK_FIELD(CWind, m_specialWind, 0xC8);
	CHECK_FIELD(CWind, m_specialIntensity, 0xD4);
	CHECK_FIELD(CWind, m_specialDirection, 0xE4);
	CHECK_FIELD(CWind, m_activeSpecialWind, 0xF4);
	CHECK_FIELD(CWind, m_fxMatrix, 0xF8);
	CHECK_FIELD(CWind, m_fxOffset, 0x128);
	CHECK_FIELD(CWind, m_pFxSequence, 0x134);
END_LAYOUT_CHECK

CWind& Wind(void);

#endif // PG_CLIENT_PROJECTG_WIND_H
