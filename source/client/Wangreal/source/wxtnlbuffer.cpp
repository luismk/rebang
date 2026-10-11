// Original: d:\build\custom\temp\code\client\wangreal\source\wxtnlbuffer.cpp
#include "../include/wxtnlbuffer.h"
#include "../include/wbone.h"
#include "../include/wpuppet.h"
#include "../include/wvideo.h"
#include "../include/wview.h"

#include <string.h>

// wxtnlbuffer.cpp:23
WxVb::WxVb(unsigned long dwFVF, unsigned long dwUsage)
	: xhVb(0), xnVtxs(0), xdwFVF(dwFVF), xdwUsage(dwUsage), xpNext(NULL) {
}

// wxtnlbuffer.cpp:28
WxVb::~WxVb() {
	xhVb = 0; xnVtxs = 0; xdwFVF = 0; xdwUsage = 0; xpNext = NULL;
}

// wxtnlbuffer.cpp:33
void WxVb::xReset(unsigned long dwFVF, unsigned long dwUsage) {
	xhVb = 0;
	xnVtxs = 0;
	xdwFVF = dwFVF;
	xdwUsage = dwUsage;
	xpNext = NULL;
}

// wxtnlbuffer.cpp:42
void WxVb::xClear(WVideoDev* pVideo) {
	if (xpNext) {
		xpNext->xClear(pVideo);
		delete xpNext;
		xpNext = NULL;
	}

	pVideo->xReleaseVertexBuffer(xhVb);
}

// wxtnlbuffer.cpp:56
WxIb::WxIb() : xhIb(0), xnIdxs(0), xdwUsage(0) {
}

// wxtnlbuffer.cpp:64
WxBatch::WxBatch(WBone* pBone, w_mesh* pMesh, unsigned long dwFVF, int iBaseVtxIdx, int nVtxs, int iBaseIdxIdx, int nIdxs) : xpBone(pBone), xpMesh(pMesh), xiBaseVtxIdx(iBaseVtxIdx), xnVtxs(nVtxs), xiBaseIdxIdx(iBaseIdxIdx), xnIdxs(nIdxs), xdwFVF(dwFVF), xpNext(NULL) {
}

// wxtnlbuffer.cpp:68
void WxBatch::xClear() {
	if (xpNext) {
		xpNext->xClear();
		delete xpNext;
		xpNext = NULL;
	}
}

// wxtnlbuffer.cpp:78
void WxBatch::xCopy(WxBatch** ppBatch) {
	*ppBatch = new WxBatch(xpBone, xpMesh, xdwFVF, xiBaseVtxIdx, xnVtxs, xiBaseIdxIdx, xnIdxs);
}

// wxtnlbuffer.cpp:83
void WxBatch::xProcessShader(WView* pView) {
	if (!xpMesh->bSpec) {
		return;
	}

	WVideoDev* pVideo = pView->GetVideoDevice();

	if (pVideo == NULL) {
		return;
	}

	int i;
	int iOffset = pVideo->xGetVertexElemOffset(xdwFVF, WX_VELEM_TEX2);

	if (iOffset < 0) {
		return;
	}

	unsigned char stride = pVideo->xGetStride((unsigned int)xpMesh->xiDrawFlag2 >> 22);
	unsigned char* pData = pVideo->xLockVertexBuffer((unsigned int)xpMesh->xiDrawFlag2 >> 22, xiBaseVtxIdx * stride, xnVtxs * stride);

	WBone* pBone = xpBone;
	WMatrix mInv = ~pBone->GetMatrix();

	if (pBone->GetLight().type == LightSet::W_LIGHT_POINT) {
	} else if (pBone->GetLight().type == LightSet::W_LIGHT_DIRECTIONAL) {
		WVector vLocalE;
		WVector vLocalL;
		WVector vXA;
		WVector vYA;
		WVector vZA;
		WVector* normList = xpMesh->myNormalList ? xpMesh->myNormalList : xpMesh->normList;

		vLocalE = pView->camera.pivot * mInv;
		vLocalL = RotVec(pBone->GetLight().nearone, mInv).Normalize();

		for (i = 0; i < xnVtxs; i++, pData += stride) {
			if (vLocalL * normList[i] >= 0.0f) {
				*(float*)(pData + iOffset) = *(float*)(pData + iOffset + 4) = 0.75f;
			} else {
				vZA = ((xpMesh->vecList[i] - vLocalE).Normalize() + vLocalL).Normalize();

				if (vZA * normList[i] >= 0.0f) {
					*(float*)(pData + iOffset) = *(float*)(pData + iOffset + 4) = 0.75f;
				} else {
					if (WisEqual(vZA.y, -1.0f, 1e-5f) || WisEqual(vZA.y, 1.0f, 1e-5f)) {
						vXA = WCrossProduct(WVector::UNIT_POS_X, vZA).Normalize();
						vYA = WCrossProduct(vZA, vXA).Normalize();
					} else {
						vXA = WCrossProduct(WVector::UNIT_POS_Y, vZA).Normalize();
						vYA = WCrossProduct(vZA, vXA).Normalize();
					}

					*(float*)(pData + iOffset) = Wabs(vXA * normList[i]);
					*(float*)(pData + iOffset + 4) = Wabs(vYA * normList[i]);
				}
			}
		}
	}

	pVideo->xUnlockVertexBuffer((unsigned int)xpMesh->xiDrawFlag2 >> 22);
}

// wxtnlbuffer.cpp:169
WxBatchGrp::WxBatchGrp(WxVb* pVb, WxIb* pIb, int iFlag0, int iFlag1, int iBaseVtxIdx, int nVtxs, int iBaseIdxIdx, int nIdxs) : xpVb(pVb), xpIb(pIb), xiFlag0(iFlag0), xiFlag1(iFlag1), xiBaseVtxIdx(iBaseVtxIdx), xnVtxs(nVtxs), xiBaseIdxIdx(iBaseIdxIdx), xnIdxs(nIdxs), xpaBatches(NULL), xpNext(NULL) {
}

// wxtnlbuffer.cpp:173
void WxBatchGrp::xClear() {
	if (xpNext) {
		xpNext->xClear();
		delete xpNext;
		xpNext = NULL;
	}

	if (xpaBatches) {
		xpaBatches->xClear();
		delete xpaBatches;
		xpaBatches = NULL;
	}
}

// wxtnlbuffer.cpp:190
void WxBatchGrp::xCopy(WxBatchGrp** ppGrp) {
	*ppGrp = new WxBatchGrp(xpVb, xpIb, xiFlag0, xiFlag1, xiBaseVtxIdx, xnVtxs, xiBaseIdxIdx, xnIdxs);
}

// wxtnlbuffer.cpp:196
void WxBatchGrp::xRenderByBatch(WView* pView, WxBatchState* pBState) {
	for (WxBatch* pBatch = xpaBatches; pBatch; pBatch = pBatch->xpNext) {
		if (!WBone::IsMeshVisible(pBatch->xpBone, pBatch->xpMesh) || (pView->IsShadowView() && (pBatch->xpMesh->drawFlag & 0x300000) == 0x100000)) {
			continue;
		}

		pBState->xmW = pBatch->xpBone->GetMatrix();

		pBState->xnmTransfs = 0;
		xRenderSubset(pView, pBState, pBatch, 1);
	}
}

// wxtnlbuffer.cpp:220
void WxBatchGrp::xRenderAtOnce(WView* pView, WxBatchState* pBState, bool bPerTranspMesh, int* paiBatchIdxs, int nIdxs) {
	if (paiBatchIdxs == NULL) {
		WxBatch* pFirst = NULL;
		int nBatches = 0;
		for (WxBatch* pBatch = xpaBatches; pBatch; pBatch = pBatch->xpNext) {
			if (!pBatch->xpBone || !pBatch->xpMesh || !WBone::IsMeshVisible(pBatch->xpBone, pBatch->xpMesh) || (pView->IsShadowView() && (pBatch->xpMesh->drawFlag & 0x300000) == 0x100000)) {
				if (pFirst) {
					xRenderSubset(pView, pBState, pFirst, nBatches);
					pFirst = NULL;
					nBatches = 0;
				}
			} else if (bPerTranspMesh && (pBatch->xpMesh->drawFlag & 0x400000)) {
				if (pFirst) {
					xRenderSubset(pView, pBState, pFirst, nBatches);
				}
				pFirst = pBatch;
				nBatches = 1;
			} else if (pFirst) {
				if (pBatch->xpBone->GetAlpha() != pFirst->xpBone->GetAlpha() || pBatch->xpMesh->drawFlag != pFirst->xpMesh->drawFlag || pBatch->xpMesh->xiDrawFlag2 != pFirst->xpMesh->xiDrawFlag2 || stricmp(pBatch->xpMesh->mpetName, pFirst->xpMesh->mpetName) != 0) {
					xRenderSubset(pView, pBState, pFirst, nBatches);
					pFirst = pBatch;
					nBatches = 1;
				} else {
					nBatches++;
				}
			} else {
				pFirst = pBatch;
				nBatches = 1;
			}
		}

		if (pFirst) {
			xRenderSubset(pView, pBState, pFirst, nBatches);
		}
	}
}

// wxtnlbuffer.cpp:295
void WxBatchGrp::xRenderSubset(WView* pView, WxBatchState* pBState, WxBatch* pBatch, int nBatches) {
	int nVtxs = 0;
	int nIdxs = 0;
	WxBatch* p = pBatch;
	int i;
	for (i = 0; i < nBatches; i++) {
		nVtxs += p->xnVtxs;
		nIdxs += p->xnIdxs;
		p = p->xpNext;
	}

	if (pView->ProcessEffect()) {
		p = pBatch;
		for (i = 0; i < nBatches; i++) {
			p->xProcessShader(pView);
			p = p->xpNext;
		}
	}

	pBState->xiFlag0 = pBatch->xpMesh->drawFlag;
	pBState->xiFlag1 = pBatch->xpMesh->xiDrawFlag2;
	pBState->xiBaseVtxIdx = pBatch->xiBaseVtxIdx;
	pBState->xiBaseIdxIdx = pBatch->xiBaseIdxIdx;
	pBState->xnVtxs = nVtxs;
	pBState->xnIdxs = nIdxs;

	xRender(pView, pBState, pBatch->xpBone, pBatch->xpMesh);
}

// wxtnlbuffer.cpp:361
WxTnLBuffer::WxTnLBuffer() : m_xiFlag(0), m_xpaVbs(NULL), m_xpaBatchGrps(NULL), m_xpPet(NULL), m_xpVideo(NULL) {
	memset(m_xaIbs, 0, sizeof(m_xaIbs));
}

// wxtnlbuffer.cpp:371
WxTnLBuffer::~WxTnLBuffer() {
	xClear();
}

// wxtnlbuffer.cpp:376
void WxTnLBuffer::xClear() {
	if (!(m_xiFlag & WX_BUF_CLONE) && m_xpaVbs) {
		m_xpaVbs->xClear(m_xpVideo);
		if (m_xpaVbs) { delete m_xpaVbs; m_xpaVbs = NULL; }
	}

	for (int i = 0; i < 32; i++) {
		if (m_xaIbs[i].xnIdxs && !(m_xiFlag & WX_BUF_CLONE)) {
			m_xpVideo->xReleaseIndexBuffer(m_xaIbs[i].xhIb);
		}
		m_xaIbs[i].xnIdxs = 0;
		m_xaIbs[i].xhIb = 0;
		m_xaIbs[i].xdwUsage = 0;
	}

	if (m_xpaBatchGrps) {
		m_xpaBatchGrps->xClear();
		if (m_xpaBatchGrps) { delete m_xpaBatchGrps; m_xpaBatchGrps = NULL; }
	}
}

// wxtnlbuffer.cpp:403
void WxTnLBuffer::xCopy(WxTnLBuffer** ppBuffer, WVideoDev* pVideo) {
	*ppBuffer = new WxTnLBuffer;

	(*ppBuffer)->m_xiFlag |= m_xiFlag | WX_BUF_CLONE;
	(*ppBuffer)->m_xpVideo = pVideo;
	(*ppBuffer)->m_xpaVbs = m_xpaVbs;

	for (int i = 0; i < sizeof(m_xaIbs) / sizeof(m_xaIbs[0]); i++) {
		(*ppBuffer)->m_xaIbs[i] = m_xaIbs[i];
	}

	WxBatchGrp** ppGrp = &(*ppBuffer)->m_xpaBatchGrps;
	for (WxBatchGrp* pGrp = m_xpaBatchGrps; pGrp; pGrp = pGrp->xpNext) {
		pGrp->xCopy(ppGrp);

		WxBatch** ppBatch = &(*ppGrp)->xpaBatches;
		for (WxBatch* pBatch = pGrp->xpaBatches; pBatch; pBatch = pBatch->xpNext) {
			pBatch->xCopy(ppBatch);
			ppBatch = &(*ppBatch)->xpNext;
		}

		ppGrp = &(*ppGrp)->xpNext;
	}
}

// wxtnlbuffer.cpp:435
void WxTnLBuffer::xSetRootBone(WPuppet* pPet) {
	m_xpPet = pPet;

	xUpdateBones(pPet->GetRootBone());
}

// wxtnlbuffer.cpp:442
void WxTnLBuffer::xUpdateBones(WBone* pBone) {
	for (WxBatchGrp* pGrp = m_xpaBatchGrps; pGrp; pGrp = pGrp->xpNext) {
		for (WxBatch* pBatch = pGrp->xpaBatches; pBatch; pBatch = pBatch->xpNext) {
			if (pBatch->xpBone->m_id == pBone->m_id) {
				w_mesh* pMesh = pBatch->xpBone->m_mesh;
				w_mesh* pNewMesh = pBone->m_mesh;
				for (; pMesh; pMesh = pMesh->next, pNewMesh = pNewMesh->next) {
					if (pMesh == pBatch->xpMesh) {
						pBatch->xpMesh = pNewMesh;
						break;
					}
				}
				pBatch->xpBone = pBone;
			}
		}
	}

	if (pBone->m_next) {
		xUpdateBones(pBone->m_next);
	}
	if (pBone->m_child) {
		xUpdateBones(pBone->m_child);
	}
}

// wxtnlbuffer.cpp:477
WxBatchGrp* WxTnLBuffer::xGetBatchGrps() {
	return m_xpaBatchGrps;
}

// wxtnlbuffer.cpp:482
int WxTnLBuffer::xCreateBuffers(WVideoDev* pVideo, WPuppet* pPet) {
	m_xpPet = pPet;
	m_xpVideo = pVideo;
	int ret = xDetermineProperty(pPet->GetRootBone(), 1, pPet->GetLightMode());
	ret |= xAllocBuffers(1);
	return ret;
}

// wxtnlbuffer.cpp:559
int WxTnLBuffer::xCreateStaticBuffers(WVideoDev* pVideo, WPuppet** ppPets, int nPets) {
	m_xiFlag |= WX_BUF_STATIC;
	m_xpPet = ppPets[0]; m_xpVideo = pVideo;
	int ret = xDetermineProperty(m_xpPet->GetRootBone(), nPets, ppPets[0]->GetLightMode());
	ret |= xAllocBuffers(nPets);
	return ret;
}

// wxtnlbuffer.cpp:573
int WxTnLBuffer::xDetermineProperty(WBone* pBone, int nPets, int iLightMode) {
	WxVb* pVb;
	WxBatchGrp* pBatchGrp;
	WxIb* pIb;
	WxBatch* pBatch;

	for (w_mesh* pMesh = pBone->m_mesh; pMesh; pMesh = pMesh->next) {
		int iDrawFlag2 = pMesh->xiDrawFlag2;

		pMesh->xiDrawFlag2 = (pMesh->maxBoneNum > 0 ? 0x1000 : 0) | (iDrawFlag2 & 0xc14) | ((iLightMode & 0x1000000) ? 0 : 0x2000);

		if (!(pMesh->xiDrawFlag2 & 0x2000))
			pMesh->xiDrawFlag2 |= 4;

		pVb = NULL; pIb = NULL; pBatchGrp = NULL; pBatch = NULL;
		xGetMatchingBuffers(&pVb, &pIb, pMesh);
		xGetMatchingBatches(&pBatchGrp, &pBatch, pVb, pIb, pBone, pMesh, nPets);

		pVb->xnVtxs += pMesh->vtxNum * nPets;
		pIb->xnIdxs += pMesh->indexNum * nPets;

		pMesh->xpBatchGrp = pBatchGrp;
		pMesh->xpBatch = pBatch;
	}

	if (pBone->m_next)
		xDetermineProperty(pBone->m_next, nPets, iLightMode);

	if (pBone->m_child)
		xDetermineProperty(pBone->m_child, nPets, iLightMode);

	return 0;
}

// wxtnlbuffer.cpp:625
void WxTnLBuffer::xGetMatchingBuffers(WxVb** ppVb, WxIb** ppIb, w_mesh* pMesh) {
	WxVb* pPrevVb = NULL;
	WxIb* pIb = NULL;
	unsigned long dwFVF = m_xpVideo->xDetermineFVF(pMesh->drawFlag, pMesh->xiDrawFlag2, pMesh->maxBoneNum);
	unsigned long dwUsage = m_xpVideo->xDetermineBufferUsage(dwFVF) | 8;
	unsigned long dwVbUsage = dwUsage;
	if (pMesh->bSpec)
		dwVbUsage |= 0x200;

	WxVb* pVb = NULL;
	if (m_xpaVbs) {
		pVb = m_xpaVbs;
		do {
			if (dwFVF == pVb->xdwFVF && dwVbUsage == pVb->xdwUsage)
				break;
			pPrevVb = pVb;
			pVb = pVb->xpNext;
		} while (pVb);
	}

	if (!pVb) {
		pVb = new WxVb(dwFVF, dwVbUsage);
		if (pPrevVb)
			pPrevVb->xpNext = pVb;
		else
			m_xpaVbs = pVb;
	}

	for (unsigned int i = 0; i < 32; ++i) {
		if (m_xaIbs[i].xnIdxs == 0) {
			pIb = &m_xaIbs[i];
			pIb->xdwUsage = dwUsage;
			break;
		}
		if (dwUsage == m_xaIbs[i].xdwUsage && m_xaIbs[i].xnIdxs + pMesh->indexNum < 9000) {
			pIb = &m_xaIbs[i];
			break;
		}
	}

	*ppVb = pVb;
	*ppIb = pIb;
}

// wxtnlbuffer.cpp:682
void WxTnLBuffer::xGetMatchingBatches(WxBatchGrp** ppBatchGrp, WxBatch** ppBatch, WxVb* pVb, WxIb* pIb, WBone* pBone, w_mesh* pMesh, int nPets) {
	WxBatch* pBatch = NULL;
	WxBatchGrp* pPrev = NULL;
	WxBatchGrp* pGrp = NULL;
	if (m_xpaBatchGrps) {
		pGrp = m_xpaBatchGrps;
		do {
			if (pGrp->xpVb == pVb && pGrp->xpIb == pIb && pGrp->xiFlag0 == pMesh->drawFlag && pGrp->xiFlag1 == pMesh->xiDrawFlag2)
				break;
			pPrev = pGrp;
			pGrp = pGrp->xpNext;
		} while (pGrp);
	}

	if (!pGrp) {
		pGrp = new WxBatchGrp(pVb, pIb, pMesh->drawFlag, pMesh->xiDrawFlag2, -1, 0, -1, 0);
		if (pPrev)
			pPrev->xpNext = pGrp;
		else
			m_xpaBatchGrps = pGrp;
	}

	WxBatch* pTail = pGrp->xpaBatches;
	if (!pTail)
		pTail = NULL;
	else
		while (pTail->xpNext)
			pTail = pTail->xpNext;

	for (int i = 0; i < nPets; i++) {
		WxBatch* pNextBatch = pBatch;
		pBatch = new WxBatch(pBone, pMesh, pVb->xdwFVF, -1, 0, -1, 0);
		pBatch->xpNext = pNextBatch;
		if (!pTail)
			pGrp->xpaBatches = pBatch;
		else
			pTail->xpNext = pBatch;
	}

	*ppBatchGrp = pGrp;
	*ppBatch = pBatch;
}

// wxtnlbuffer.cpp:743
void WxTnLBuffer::xSortBatches() {
	// The original's body (lines 744-783) compiles to nothing.
}

// wxtnlbuffer.cpp:787
int WxTnLBuffer::xAllocBuffers(int nPets) {
	WxBatchGrp* pGrp;
	WxBatch* pBatch;
	int nPetElems;
	int i;

	int nVtxs = 0;
	for (WxVb* pVb = m_xpaVbs; pVb; pVb = pVb->xpNext) {
		pVb->xhVb = m_xpVideo->xCreateVertexBuffer(pVb->xnVtxs, pVb->xdwFVF, pVb->xdwUsage);

		for (pGrp = m_xpaBatchGrps; pGrp; pGrp = pGrp->xpNext) {
			if (pGrp->xpVb != pVb) {
				continue;
			}

			pGrp->xiFlag1 |= pVb->xhVb << 22;
			pGrp->xiBaseVtxIdx = nVtxs;

			for (pBatch = pGrp->xpaBatches; pBatch; pBatch = pBatch->xpNext) {
				pBatch->xiBaseVtxIdx = nVtxs;
				xCount(true, pGrp, pBatch, m_xpPet->GetRootBone());

				nPetElems = pBatch->xnVtxs;
				for (i = 1; i < nPets; i++) {
					pBatch = pBatch->xpNext;
					pBatch->xiBaseVtxIdx = nVtxs + i * nPetElems;
					pBatch->xnVtxs = nPetElems;
				}

				nPetElems *= nPets;
				pGrp->xnVtxs += nPetElems;
				nVtxs += nPetElems;
			}
		}

		nVtxs = 0;
	}

	int nIdxs = 0;
	for (int iIb = 0; iIb < 32; iIb++) {
		WxIb* pIb = &m_xaIbs[iIb];
		if (pIb->xnIdxs <= 0) {
			continue;
		}

		pIb->xhIb = m_xpVideo->xCreateIndexBuffer(pIb->xnIdxs, pIb->xdwUsage);

		for (pGrp = m_xpaBatchGrps; pGrp; pGrp = pGrp->xpNext) {
			if (pGrp->xpIb != pIb) {
				continue;
			}

			pGrp->xiFlag1 |= (pIb->xhIb & 0xff) << 14;
			pGrp->xiBaseIdxIdx = nIdxs;

			for (pBatch = pGrp->xpaBatches; pBatch; pBatch = pBatch->xpNext) {
				pBatch->xiBaseIdxIdx = nIdxs;
				xCount(false, pGrp, pBatch, m_xpPet->GetRootBone());

				nPetElems = pBatch->xnIdxs;
				for (i = 1; i < nPets; i++) {
					pBatch = pBatch->xpNext;
					pBatch->xiBaseIdxIdx = nIdxs + i * nPetElems;
					pBatch->xnIdxs = nPetElems;
				}

				nPetElems *= nPets;
				pGrp->xnIdxs += nPetElems;
				nIdxs += nPetElems;
			}
		}

		nIdxs = 0;
	}

	return 0;
}

// wxtnlbuffer.cpp:870
void WxTnLBuffer::xCount(bool b, WxBatchGrp* pGrp, WxBatch* pBatch, WBone* pBone) {
	for (w_mesh* pMesh = pBone->m_mesh; pMesh; pMesh = pMesh->next) {
		if (pMesh->xpBatch == pBatch) {
			if (b) {
				pBatch->xnVtxs += pMesh->vtxNum;
			} else {
				pBatch->xnIdxs += pMesh->indexNum;
			}
			break;
		}
	}

	if (pBone->m_next) {
		xCount(b, pGrp, pBatch, pBone->m_next);
	}
	if (pBone->m_child) {
		xCount(b, pGrp, pBatch, pBone->m_child);
	}
}

// wxtnlbuffer.cpp:894
int WxTnLBuffer::xFillBuffers() {
	for (WxVb* pVb = m_xpaVbs; pVb; pVb = pVb->xpNext) {
		xFillVertexBuffer(*pVb);
	}

	for (int i = 0; i < 32; i++) {
		if (m_xaIbs[i].xnIdxs) {
			xFillIndexBuffer(m_xaIbs[i]);
		}
	}

	return 0;
}

// wxtnlbuffer.cpp:913
void WxTnLBuffer::xFillVertexBuffer(const WxVb& vb) {
	unsigned char* paData;
	int iStride;

	paData = m_xpVideo->xLockVertexBuffer(vb.xhVb, 0, 0);

	for (WxBatchGrp* pGrp = m_xpaBatchGrps; pGrp; pGrp = pGrp->xpNext) {
		if (pGrp->xpVb != &vb)
			continue;

		iStride = m_xpVideo->VertexSize(pGrp->xpVb->xdwFVF);

		for (WxBatch* pBatch = pGrp->xpaBatches; pBatch; pBatch = pBatch->xpNext)
			xFillVertexBufferByBone(paData + pBatch->xiBaseVtxIdx * iStride, pGrp, pBatch, pBatch->xpBone);
	}

	m_xpVideo->xUnlockVertexBuffer(vb.xhVb);
}

// wxtnlbuffer.cpp:935
void WxTnLBuffer::xFillVertexBufferByBone(unsigned char* pData, WxBatchGrp* pGrp, WxBatch* pBatch, WBone* pBone) {
	for (w_mesh* pMesh = pBone->m_mesh; pMesh; pMesh = pMesh->next) {
		if (pMesh->xpBatch == pBatch) {
			int hVb = pGrp->xpVb->xhVb;

			pMesh->xiDrawFlag2 |= hVb << 22;
			pMesh->xiBaseVtxIdx = pBatch->xiBaseVtxIdx;
			xFillVertexBufferByMesh(pData, pGrp, pBatch, pMesh);
			break;
		}
	}

	if (pBone->m_next) {
		xFillVertexBufferByBone(pData, pGrp, pBatch, pBone->m_next);
	}
	if (pBone->m_child) {
		xFillVertexBufferByBone(pData, pGrp, pBatch, pBone->m_child);
	}
}

// wxtnlbuffer.cpp:959
void WxTnLBuffer::xFillVertexBufferByMesh(unsigned char* pData, WxBatchGrp* pBatchGrp, WxBatch* pBatch, w_mesh* pMesh) {
	unsigned long dwFVF = pBatchGrp->xpVb->xdwFVF;
	int iOffs = 0;
	const std::vector<const WMatrix*>* orgMatPtrList = 0;
	int iStride = m_xpVideo->VertexSize(dwFVF);
	int iWeightSize = m_xpVideo->xGetBlendWeightSize(dwFVF);
	bool hasNormal = m_xpVideo->xHasVertexElem(dwFVF, WX_VELEM_NORMAL);
	bool hasDiffuse = m_xpVideo->xHasVertexElem(dwFVF, WX_VELEM_DIFFUSE);
	bool hasTex1 = m_xpVideo->xHasVertexElem(dwFVF, WX_VELEM_TEX1);
	bool hasTex2 = m_xpVideo->xHasVertexElem(dwFVF, WX_VELEM_TEX2);
	const WMatrix* mTransf;

	if ((m_xiFlag & 2) == 0) {
		if (strlen(pMesh->mpetName) != 0)
			orgMatPtrList = &WPuppet::GetOrginalMatrixPtrList(pMesh->mpetName);
		else
			orgMatPtrList = &m_xpPet->xGetTransfMatPtrList();

		int iId = pBatch->xpBone->m_id;
		if (iId < (int)orgMatPtrList->size())
			mTransf = (*orgMatPtrList)[iId];
		else
			mTransf = &WMatrix::IDENTITY;
	} else {
		mTransf = &pBatch->xpBone->m_matrix;
	}

	WMatrix mRot;
	mTransf->GetRotMatrix(&mRot);

	WVector* normList = pMesh->myNormalList ? pMesh->myNormalList : pMesh->normList;

	int i = 0;
	for (; i < pMesh->vtxNum; ++i) {
		if (!(pMesh->xiDrawFlag2 & 0x1000)) {
			if (!(m_xiFlag & 2))
				memcpy(pData + iOffs, &pMesh->vecList[i], sizeof(WVector));
			else
				*(WVector*)(pData + iOffs) = pMesh->vecList[i] * *mTransf;
			iOffs += 12;
		} else {
			if (i < pMesh->rigidNum)
				*(WVector*)(pData + iOffs) = pMesh->vecList[i] * *(*orgMatPtrList)[pMesh->boneList[i]->m_id];
			else
				*(WVector*)(pData + iOffs) = pMesh->vecList[i] * *mTransf;
			iOffs += 0x10 + iWeightSize;
		}

		if (hasNormal) {
			if (!(pMesh->xiDrawFlag2 & 0x1000)) {
				if (!(m_xiFlag & 2))
					memcpy(pData + iOffs, &normList[i], sizeof(WVector));
				else
					*(WVector*)(pData + iOffs) = normList[i] * mRot;
			} else if (i < pMesh->rigidNum) {
				WMatrix mBoneRot;
				(*orgMatPtrList)[pMesh->boneList[i]->m_id]->GetRotMatrix(&mBoneRot);
				*(WVector*)(pData + iOffs) = normList[i] * mBoneRot;
			} else {
				*(WVector*)(pData + iOffs) = normList[i] * mRot;
			}
			iOffs += 12;
		}

		if (hasDiffuse) {
			if (pMesh->vtxColorPtrList)
				*(unsigned int*)(pData + iOffs) = *pMesh->vtxColorPtrList[i];
			else
				*(unsigned int*)(pData + iOffs) = pMesh->vtxColorList[i];
			iOffs += 4;
		}

		if (hasTex1) {
			memcpy(pData + iOffs, pMesh->uvData[i], sizeof(float) * 2);
			iOffs += 8;
		}

		if (hasTex2) {
			memcpy(pData + iOffs, pMesh->uvBackup[i], sizeof(float) * 2);
			iOffs += 8;
		}
	}

	if (pMesh->xiDrawFlag2 & 0x1000) {
		int i = 0;
		int iOffs = 12;
		int nRigid = 0;
		float afWeights[3];

		if (pMesh->blendedRigidNum > 0) {
			do {
				unsigned long dwMatIdx = 0;
				memset(afWeights, 0, sizeof(afWeights));
				float fTotalWeight = 0.0f;
				int j = 0;
				for (; fTotalWeight < 0.999f && j < 3; ++j, ++nRigid) {
					if (pMesh->blendedBoneList[nRigid]) {
						dwMatIdx |= pMesh->blendedBoneList[nRigid]->m_id << (j * 8);
						afWeights[j] = pMesh->blendWeightList[nRigid];
						fTotalWeight += pMesh->blendWeightList[nRigid];
					}
				}
				if (j == 3 && fTotalWeight < 0.999f && pMesh->blendedBoneList[nRigid]) {
					dwMatIdx |= pMesh->blendedBoneList[nRigid]->m_id << 24;
					++nRigid;
				}

				memcpy(&pData[iOffs], afWeights, iWeightSize);
				*(unsigned int*)(&pData[iOffs] + iWeightSize) = dwMatIdx;
				iOffs += iStride;
				++i;
			} while (i < pMesh->blendedRigidNum);
		}

		afWeights[0] = 1.0f;
		afWeights[2] = 0.0f;
		afWeights[1] = 0.0f;

		if ((int)iWeightSize > 0) {
			if (i < pMesh->rigidNum) {
				do {
					int iId = pMesh->boneList[i]->m_id;
					memcpy(&pData[iOffs], afWeights, iWeightSize);
					*(int*)(pData + iWeightSize + iOffs) = iId;
					++i;
					iOffs += iStride;
				} while (i < pMesh->rigidNum);
			}
			int iId = pBatch->xpBone->m_id;
			if (i < pMesh->vtxNum) {
				do {
					memcpy(&pData[iOffs], afWeights, iWeightSize);
					*(int*)(pData + iWeightSize + iOffs) = iId;
					++i;
					iOffs += iStride;
				} while (i < pMesh->vtxNum);
			}
		} else {
			if (i < pMesh->rigidNum) {
				do {
					*(int*)(pData + iWeightSize + iOffs) = pMesh->boneList[i]->m_id;
					++i;
					iOffs += iStride;
				} while (i < pMesh->rigidNum);
			}
			int iId = pBatch->xpBone->m_id;
			if (i < pMesh->vtxNum) {
				do {
					*(int*)(pData + iOffs + iWeightSize) = iId;
					++i;
					iOffs += iStride;
				} while (i < pMesh->vtxNum);
			}
		}
	}
}

// wxtnlbuffer.cpp:1146
void WxTnLBuffer::xFillIndexBuffer(const WxIb& ib) {
	unsigned char* paData;

	paData = m_xpVideo->xLockIndexBuffer(ib.xhIb, 0, 0);

	for (WxBatchGrp* pGrp = m_xpaBatchGrps; pGrp; pGrp = pGrp->xpNext) {
		if (pGrp->xpIb != &ib)
			continue;

		for (WxBatch* pBatch = pGrp->xpaBatches; pBatch; pBatch = pBatch->xpNext)
			xFillIndexBufferByBone(paData + pBatch->xiBaseIdxIdx * 2, pGrp, pBatch, pBatch->xpBone);
	}

	m_xpVideo->xUnlockIndexBuffer(ib.xhIb);
}

// wxtnlbuffer.cpp:1166
void WxTnLBuffer::xFillIndexBufferByBone(unsigned char* pData, WxBatchGrp* pGrp, WxBatch* pBatch, WBone* pBone) {
	unsigned short* pIdx = (unsigned short*)pData;

	for (w_mesh* pMesh = pBone->m_mesh; pMesh; pMesh = pMesh->next) {
		if (pMesh->xpBatch == pBatch) {
			pMesh->xiDrawFlag2 |= (pGrp->xpIb->xhIb & 0xff) << 14;
			pMesh->xiBaseIdxIdx = pBatch->xiBaseIdxIdx;

			for (int i = 0; i < pMesh->indexNum; i++) {
				*pIdx = pMesh->indexList[i] + (unsigned short)pMesh->xiBaseVtxIdx;
				pIdx++;
			}
			break;
		}
	}

	if (pBone->m_next) {
		xFillIndexBufferByBone(pData, pGrp, pBatch, pBone->m_next);
	}
	if (pBone->m_child) {
		xFillIndexBufferByBone(pData, pGrp, pBatch, pBone->m_child);
	}
}

// wxtnlbuffer.cpp:1196
void WxTnLBuffer::xUpdateStaticBufferInfo(WPuppet** ppPets, int nPets) {
	WBone* pRoot = ppPets[0]->GetRootBone();

	for (int i = 1; i < nPets; i++) {
		xUpdateStaticBufferInfo(pRoot, ppPets[i]->GetRootBone(), i);
	}
}

// wxtnlbuffer.cpp:1207
void WxTnLBuffer::xUpdateStaticBufferInfo(WBone* pBone, WBone* pRoot, int index) {
	WBone* pFound = pRoot->FindBone(pBone->m_name, 0);

	if (pFound) {
		w_mesh* pSrc = pBone->m_mesh;
		for (w_mesh* pDst = pFound->m_mesh; pSrc && pDst; pSrc = pSrc->next, pDst = pDst->next) {
			WxBatchGrp* pGrp = pSrc->xpBatchGrp;
			WxBatch* pBatch = pSrc->xpBatch;
			for (int i = 0; i < index; i++) {
				pBatch = pBatch->xpNext;
			}

			pDst->xpBatchGrp = pGrp;
			pDst->xiDrawFlag2 = pGrp->xiFlag1;

			pDst->xpBatch = pBatch;
			pDst->xiBaseVtxIdx = pBatch->xiBaseVtxIdx;
			pDst->xiBaseIdxIdx = pBatch->xiBaseIdxIdx;

			pBatch->xpBone = pFound;
			pBatch->xpMesh = pDst;
		}
	}

	if (pBone->m_next) {
		xUpdateStaticBufferInfo(pBone->m_next, pRoot, index);
	}
	if (pBone->m_child) {
		xUpdateStaticBufferInfo(pBone->m_child, pRoot, index);
	}
}

// wxtnlbuffer.cpp:1247
void WxTnLBuffer::xUpdateMatrixIdx(WPuppet* pPet) {
	for (WxBatchGrp* pGrp = m_xpaBatchGrps; pGrp; pGrp = pGrp->xpNext) {
		if (pGrp->xiFlag1 & 0x1000) {
			unsigned long stride = m_xpVideo->VertexSize(pGrp->xpVb->xdwFVF);

			unsigned char* pData = m_xpVideo->xLockVertexBuffer(pGrp->xpVb->xhVb, 0, 0);

			for (WxBatch* pBatch = pGrp->xpaBatches; pBatch; pBatch = pBatch->xpNext) {
				xUpdateMatrixIdx(pData + pBatch->xiBaseVtxIdx * stride, pGrp, pBatch, m_xpPet->GetRootBone(), pPet);
			}

			m_xpVideo->xUnlockVertexBuffer(pGrp->xpVb->xhVb);
		}
	}
}

// wxtnlbuffer.cpp:1275
void WxTnLBuffer::xUpdateMatrixIdx(unsigned char* paData, WxBatchGrp* pBatchGrp, WxBatch* pBatch, WBone* pBone, WPuppet* pNewPet) {
	w_mesh* pMesh = pBone->m_mesh;
	while (pMesh && pMesh->xpBatch != pBatch)
		pMesh = pMesh->next;

	if (pMesh) {
		unsigned long dwFVF = pBatchGrp->xpVb->xdwFVF;
		int iStride = m_xpVideo->VertexSize(dwFVF);
		int i;
		int iBone = 0;
		int iOffs = 0x18;

		for (i = 0; i < pMesh->blendedRigidNum; ++i) {
			unsigned int dwMatIdx = 0;
			float fTotalWeight = 0.0f;
			int j = 0;
			do {
				if (j >= 3)
					break;
				dwMatIdx |= pMesh->blendedBoneList[iBone]->m_id << (j * 8);
				fTotalWeight += pMesh->blendWeightList[iBone];
				++j;
				++iBone;
			} while (fTotalWeight < 0.999f);

			if (j == 3 && fTotalWeight < 0.999f) {
				dwMatIdx |= pMesh->blendedBoneList[iBone++]->m_id << 24;
			}
			*(unsigned int*)(paData + iOffs) = dwMatIdx;
			iOffs += iStride;
		}

		if (i < pMesh->rigidNum) {
			int* pIdx = (int*)(paData + iOffs);
			do {
				*pIdx = pMesh->boneList[i]->m_id;
				++i;
				pIdx = (int*)((char*)pIdx + iStride);
			} while (i < pMesh->rigidNum);
		}
	}

	if (pBone->m_next)
		xUpdateMatrixIdx(paData, pBatchGrp, pBatch, pBone->m_next, pNewPet);
	if (pBone->m_child)
		xUpdateMatrixIdx(paData, pBatchGrp, pBatch, pBone->m_child, pNewPet);
}

// wxtnlbuffer.cpp:1327
void WxTnLBuffer::xRender(WView* pView) {
	WxBatchState BatchState;
	WxBatchGrp* pGrp;

	if (m_xiFlag & WX_BUF_STATIC) {
		BatchState.xmW.Reset();
		BatchState.xnmTransfs = 0;

		for (pGrp = m_xpaBatchGrps; pGrp; pGrp = pGrp->xpNext) {
			pGrp->xRenderAtOnce(pView, &BatchState, true, NULL, 0);
		}
	} else {
		bool bMPS = false;

		for (pGrp = m_xpaBatchGrps; pGrp; pGrp = pGrp->xpNext) {
			if (!(pGrp->xiFlag1 & 0x1000)) {
				pGrp->xRenderByBatch(pView, &BatchState);
			} else {
				bMPS = true;
			}
		}

		if (bMPS) {
			BatchState.xnmTransfs = m_xpPet->xGetTransfMatPtrList().size();

			if (BatchState.xnmTransfs > 0) {
				BatchState.xpapmW = &m_xpPet->xGetTransfMatPtrList()[0];
				BatchState.xpapmO = &WPuppet::GetOrginalMatrixPtrList(m_xpPet->GetPuppetName())[0];
			}

			for (pGrp = m_xpaBatchGrps; pGrp; pGrp = pGrp->xpNext) {
				if (pGrp->xiFlag1 & 0x1000) {
					pGrp->xRenderAtOnce(pView, &BatchState, false, NULL, 0);
				}
			}
		}
	}
}

// wxtnlbuffer.cpp:1409
void xRender(WView* pView, WxBatchState* pBState, WBone* pBone, w_mesh* pMesh) {
	const WMatrix* const* orgMatBackup = pBState->xpapmO;

	if (pBone && pMesh) {
		if (pBState->xiFlag1 & 0x1000) {
			if (strlen(pMesh->mpetName) != 0) {
				pBState->xpapmO = &WPuppet::GetOrginalMatrixPtrList(pMesh->mpetName)[0];
			}
		}

		if (pBState->xiFlag1 & 0x4) {
			LightSet* pLight = pView->xGetLight();
			pBState->xdwDiffuse = (pBone->GetAlpha() << 24) | pLight->ambient2;
		} else {
			pBState->xdwDiffuse = (pBone->GetAlpha() << 24) | 0xffffff;
		}

		if (pBState->xiFlag0 & 0x400000) {
			WVector v[8] = {
				WVector(pMesh->localAabb.min.x, pMesh->localAabb.min.y, pMesh->localAabb.min.z),
				WVector(pMesh->localAabb.max.x, pMesh->localAabb.min.y, pMesh->localAabb.min.z),
				WVector(pMesh->localAabb.max.x, pMesh->localAabb.min.y, pMesh->localAabb.max.z),
				WVector(pMesh->localAabb.min.x, pMesh->localAabb.min.y, pMesh->localAabb.max.z),
				WVector(pMesh->localAabb.min.x, pMesh->localAabb.max.y, pMesh->localAabb.min.z),
				WVector(pMesh->localAabb.max.x, pMesh->localAabb.max.y, pMesh->localAabb.min.z),
				WVector(pMesh->localAabb.max.x, pMesh->localAabb.max.y, pMesh->localAabb.max.z),
				WVector(pMesh->localAabb.min.x, pMesh->localAabb.max.y, pMesh->localAabb.max.z),
			};
			float minZ = 3.402823466e+38F;
			float maxZ = -3.402823466e+38F;
			WMatrix m;

			if (pMesh->aabbBone)
				m = pMesh->aabbBone->GetMatrix() * pView->GetInvCamera();
			else
				m = pBone->GetMatrix() * pView->GetInvCamera();

			for (int i = 0; i < 8; ++i) {
				v[i].z = TransformZ(v[i], m);
				if (v[i].z < minZ)
					minZ = v[i].z;
				if (v[i].z > maxZ)
					maxZ = v[i].z;
			}

			pBState->xfDepth = maxZ * 0.3f + minZ * 0.7f;
		}
	}

	pView->xDrawIndexedTriangles(*pBState);
	pBState->xpapmO = orgMatBackup;
}
