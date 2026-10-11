// Original: d:\build\custom\temp\code\client\wangreal\include\wxpuppet.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WXPUPPET_H
#define PG_CLIENT_WANGREAL_INCLUDE_WXPUPPET_H

#include "..\..\defines.h"
#include "wpuppet.h"
#include "wresource.h"
#include "wxtnlbuffer.h"

class WScene;
class WView;
struct LightSet;

// A WPuppet rendered through a hardware T&L buffer (PDB layout, size 0x1d0).
class WxPuppet : public WPuppet {
	LAYOUT_CHECK_FRIEND;
public:
	WxPuppet(bool bNeedTnLBuf);
	virtual ~WxPuppet();
	virtual int LoadPET(char*, bool, int);
	virtual void xUpdateTnLBuffers(bool);
	virtual WPuppet* MakeClone(bool);
	void SetAlpha(unsigned char ucAlpha);
	virtual void UpdateLightSource(LightSet*, bool, WScene*);
	virtual void UpdateMeshBonePtr(WPuppet* pSrc);
	virtual void Render(WView*, bool, float, bool, bool, bool);

private:
	void xReleaseTnLBuffer();
	void xShareTnLResources(WxPuppet* pxPet);
	void xUpdateTnLResources();

	bool m_xbNeedTnLBuf;          // 0x1c8
	WxTnLBuffer* m_xpTnLBuf;      // 0x1cc
};
CHECK_SIZE(WxPuppet, 0x1d0);
BEGIN_LAYOUT_CHECK(WxPuppet)
	CHECK_FIELD(WxPuppet, m_xbNeedTnLBuf, 0x1c8);
	CHECK_FIELD(WxPuppet, m_xpTnLBuf, 0x1cc);
END_LAYOUT_CHECK

// Static puppets sharing one T&L buffer (PDB layout, size 0x1c8).
class WxStaticPuppetGrp : public WResource {
	LAYOUT_CHECK_FRIEND;
public:
	WxStaticPuppetGrp();
	virtual ~WxStaticPuppetGrp();
	int xLoad(int nSPets, const char* FileName, WPuppet** ppaSPets);
	void xUpdateCullFlag(WView* pView);
	void xRender(WView* pView);

private:
	void xReleaseTnLBuffer();

	int m_xnPets;                 // 0x2c
	WPuppet** m_xpapPets;         // 0x30
	WxTnLBuffer m_xTnLBuf;        // 0x34
};
CHECK_SIZE(WxStaticPuppetGrp, 0x1c8);
BEGIN_LAYOUT_CHECK(WxStaticPuppetGrp)
	CHECK_FIELD(WxStaticPuppetGrp, m_xnPets, 0x2c);
	CHECK_FIELD(WxStaticPuppetGrp, m_xpapPets, 0x30);
	CHECK_FIELD(WxStaticPuppetGrp, m_xTnLBuf, 0x34);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WXPUPPET_H
