// Original: d:\build\custom\temp\code\client\projectg\taileffect.h
#ifndef PG_CLIENT_PROJECTG_TAILEFFECT_H
#define PG_CLIENT_PROJECTG_TAILEFFECT_H

#include <map>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"
#include "scenemanager.h"

class CTailEffect : public CRenderFuncPtr {
public:
	struct TAILEDGE {
		bool bUse;      // 0x0
		float fLife;    // 0x4
		WVector v1;     // 0x8
		WVector v2;     // 0x14
	};

	CTailEffect(const std::string &);
	~CTailEffect(void);
	virtual void Display(void);
	void PutSpot(const WVector &, const WVector &);
	void Reset(void);
	void Process(float);
	void ChangeTexture(const std::string &);

protected:
	void Init(const std::string &);
	int GetLength(void) const {   // taileffect.h:23
		if (m_head < m_tail) {
			return m_maxEdges - m_tail + m_head;
		}
		return m_head - m_tail;
	}

public:
	// 0x0: vftable
	std::vector<TAILEDGE> m_edges;      // 0x4, ring buffer of m_maxEdges smoothed edges
	std::vector<TAILEDGE> m_spots;      // 0x14, raw spots given to PutSpot
	WVector m_lastSpot1;                // 0x24
	WVector m_lastSpot2;                // 0x30
	float m_life;                       // 0x3C
	int m_clearedCount;                 // 0x40, only cleared by the constructor in this unit
	int m_maxEdges;                     // 0x44
	int m_head;                         // 0x48
	int m_tail;                         // 0x4C
	bool m_clearedFlag;                 // 0x50, only cleared by the constructor in this unit
	int m_texture;                      // 0x54, texture handle drawn by Display
	std::map<std::string, int> m_textures;   // 0x58, texture handles loaded by ChangeTexture, by name
};
CHECK_SIZE(CTailEffect::TAILEDGE, 0x20);
CHECK_FIELD(CTailEffect::TAILEDGE, bUse, 0x0);
CHECK_FIELD(CTailEffect::TAILEDGE, fLife, 0x4);
CHECK_FIELD(CTailEffect::TAILEDGE, v1, 0x8);
CHECK_FIELD(CTailEffect::TAILEDGE, v2, 0x14);
CHECK_FIELD(CTailEffect, m_edges, 0x4);
CHECK_FIELD(CTailEffect, m_spots, 0x14);
CHECK_FIELD(CTailEffect, m_lastSpot1, 0x24);
CHECK_FIELD(CTailEffect, m_lastSpot2, 0x30);
CHECK_FIELD(CTailEffect, m_life, 0x3C);
CHECK_FIELD(CTailEffect, m_maxEdges, 0x44);
CHECK_FIELD(CTailEffect, m_head, 0x48);
CHECK_FIELD(CTailEffect, m_tail, 0x4C);
CHECK_FIELD(CTailEffect, m_clearedCount, 0x40);
CHECK_FIELD(CTailEffect, m_clearedFlag, 0x50);
CHECK_FIELD(CTailEffect, m_texture, 0x54);
CHECK_FIELD(CTailEffect, m_textures, 0x58);
CHECK_SIZE(CTailEffect, 0x64);

#endif // PG_CLIENT_PROJECTG_TAILEFFECT_H
