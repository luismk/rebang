// Original: d:\build\custom\temp\code\client\wangreal\source\wxpuppet.cpp
#include "../include/wxpuppet.h"
#include "../include/wbone.h"
#include "../include/wview.h"
#include "../include/wresrcmng.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

// wxpuppet.cpp:19
WxPuppet::WxPuppet(bool bNeedTnLBuf) : m_xbNeedTnLBuf(bNeedTnLBuf), m_xpTnLBuf(NULL) {
}

// wxpuppet.cpp:23
WxPuppet::~WxPuppet() {
	xReleaseTnLBuffer();
}

// wxpuppet.cpp:28
void WxPuppet::xReleaseTnLBuffer() {
	if (m_xpTnLBuf) { delete m_xpTnLBuf; m_xpTnLBuf = NULL; }
}

// wxpuppet.cpp:33
int WxPuppet::LoadPET(char* filename, bool alphatest, int iPetType) {
	int result = WPuppet::LoadPET(filename, alphatest, iPetType);

	char buf[260];
	sprintf(buf, "WxPuppet:%s", filename);
	SetLeakHint(buf);

	if (!m_xbNeedTnLBuf || result != 0) {
		return result;
	}

	xReleaseTnLBuffer();
	m_xpTnLBuf = new WxTnLBuffer;

	result = m_xpTnLBuf->xCreateBuffers(GetResrcManager()->VideoReference(), this);
	result |= m_xpTnLBuf->xFillBuffers();

	return result;
}

// wxpuppet.cpp:56
void WxPuppet::xUpdateTnLBuffers(bool bCreate) {
	if (bCreate) {
		xReleaseTnLBuffer();
		m_xpTnLBuf = new WxTnLBuffer;
		m_xpTnLBuf->xCreateBuffers(GetResrcManager()->VideoReference(), this);
	}

	m_xpTnLBuf->xFillBuffers();
}

// wxpuppet.cpp:70
WPuppet* WxPuppet::MakeClone(bool bNeedTnLBuf) {
	w_share_pet_data share;

	share.petName = m_petName;
	share.rootbone = m_rootbone;
	share.length = m_aniLen;
	share.faceNum = m_faceNum; share.numFramedata = m_framenum;
	share.frame = m_frame;
	share.bound = m_bound;

	share.bbList = &m_bbList; share.mdList = m_mdList;
	share.fanimList = &m_fanimList;

	WxPuppet* pClone = new WxPuppet(bNeedTnLBuf);
	pClone->m_iPetType = m_iPetType;
	pClone->m_rendMode = m_rendMode;
	pClone->Share(&share);

	if (bNeedTnLBuf) {
		if (m_xpTnLBuf) { m_xpTnLBuf->xCopy(&pClone->m_xpTnLBuf, GetResrcManager()->VideoReference()); pClone->m_xpTnLBuf->xSetRootBone(pClone); }
	}

	return pClone;
}

// wxpuppet.cpp:101
void WxPuppet::xShareTnLResources(WxPuppet* pxPet) {
	if (m_xpTnLBuf == NULL) {
		return;
	}
	m_xpTnLBuf->xCopy(&pxPet->m_xpTnLBuf, GetResrcManager()->VideoReference());
	pxPet->m_xpTnLBuf->xSetRootBone(pxPet);
}

// wxpuppet.cpp:110
void WxPuppet::xUpdateTnLResources() {
	m_xpTnLBuf->xSetRootBone(this);
}

// wxpuppet.cpp:115
void WxPuppet::SetAlpha(unsigned char ucAlpha) {
	if (!m_xpTnLBuf) {
		m_rootbone->SetAlpha(ucAlpha, 1);
	} else {
		m_rootbone->xSetAlpha(ucAlpha, 1);
	}
}

// wxpuppet.cpp:123
void WxPuppet::UpdateLightSource(LightSet* light, bool bFlag, WScene* scene) {
	m_rootbone->SetLight(light);

	if (!m_xpTnLBuf) {
		m_rootbone->CalcLight(light, true, bFlag ? m_lightmode | 0x8000000 : m_lightmode, scene);
	}
}

// wxpuppet.cpp:131
void WxPuppet::UpdateMeshBonePtr(WPuppet* pSrc) {
	WPuppet::UpdateMeshBonePtr(pSrc);

	if (m_xpTnLBuf) {
		m_xpTnLBuf->xUpdateMatrixIdx(pSrc);
	}
}

// wxpuppet.cpp:139
void WxPuppet::Render(WView* pView, bool bTransform, float t, bool bNoClip, bool bNoUpdateMat, bool bOriginal) {
	if (bOriginal) {
		WPuppet::Render(pView, bTransform, t, bNoClip, bNoUpdateMat, false);
		return;
	}

	if (m_xpTnLBuf) {
		if (bTransform) {
			Transform(pView, t, (bNoClip ? WBone::NOCLIP : 0) | (bNoUpdateMat ? 0 : WBone::NOUPDATEMAT));
		}
		pView->xSetLight(0, m_rootbone->GetLight());

		m_xpTnLBuf->xRender(pView);
	}
}

// wxpuppet.cpp:174
WxStaticPuppetGrp::WxStaticPuppetGrp() : m_xnPets(0), m_xpapPets(NULL) {
}

// wxpuppet.cpp:178
WxStaticPuppetGrp::~WxStaticPuppetGrp() {
	xReleaseTnLBuffer();

	if (m_xpapPets) { delete[] m_xpapPets; m_xpapPets = NULL; }
	m_xnPets = 0;
}

// wxpuppet.cpp:186
void WxStaticPuppetGrp::xReleaseTnLBuffer() {
	m_xTnLBuf.xClear();
}

// wxpuppet.cpp:191
int WxStaticPuppetGrp::xLoad(int nSPets, const char* FileName, WPuppet** ppaSPets) {
	m_xnPets = 0;
	m_xpapPets = new WPuppet*[nSPets];
	memcpy(m_xpapPets, ppaSPets, nSPets * sizeof(WPuppet*));

	m_xTnLBuf.xCreateStaticBuffers(GetResrcManager()->VideoReference(), ppaSPets, nSPets);

	m_xTnLBuf.xUpdateStaticBufferInfo(ppaSPets, nSPets);

	m_xTnLBuf.xFillBuffers();

	m_xnPets = nSPets;

	return 0;
}

// wxpuppet.cpp:211
void WxStaticPuppetGrp::xUpdateCullFlag(WView* pView) {
	for (int i = 0; i < m_xnPets; i++) {
		if (m_xpapPets[i]->GetRootBone()) {
			m_xpapPets[i]->Transform(pView, m_xpapPets[i]->GetMatrix().xa.Magnitude(), WBone::NOUPDATEMAT);
		}
	}
}

// wxpuppet.cpp:224
void WxStaticPuppetGrp::xRender(WView* pView) {
	m_xTnLBuf.xRender(pView);
}
