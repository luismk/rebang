// Original: d:\build\custom\temp\code\client\wangreal\include\wxtnlbuffer.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WXTNLBUFFER_H
#define PG_CLIENT_WANGREAL_INCLUDE_WXTNLBUFFER_H

#include "..\..\defines.h"

class WBone;
class WPuppet;
class WVideoDev;
class WView;
struct WxBatchState;
struct w_mesh;

struct WxVb {
	WxVb(unsigned long dwFVF, unsigned long dwUsage);
	~WxVb();
	void xReset(unsigned long dwFVF, unsigned long dwUsage);
	void xClear(WVideoDev* pVideo);

	int xhVb;                 // 0x0
	int xnVtxs;               // 0x4
	unsigned long xdwFVF;     // 0x8
	unsigned long xdwUsage;   // 0xc
	WxVb* xpNext;             // 0x10
};
CHECK_SIZE(WxVb, 0x14);
CHECK_FIELD(WxVb, xhVb, 0x0);
CHECK_FIELD(WxVb, xnVtxs, 0x4);
CHECK_FIELD(WxVb, xdwFVF, 0x8);
CHECK_FIELD(WxVb, xdwUsage, 0xc);
CHECK_FIELD(WxVb, xpNext, 0x10);

struct WxIb {
	WxIb();

	int xhIb;                 // 0x0
	int xnIdxs;               // 0x4
	unsigned long xdwUsage;   // 0x8
};
CHECK_SIZE(WxIb, 0xc);
CHECK_FIELD(WxIb, xhIb, 0x0);
CHECK_FIELD(WxIb, xnIdxs, 0x4);
CHECK_FIELD(WxIb, xdwUsage, 0x8);

struct WxBatch {
	WxBatch(WBone* pBone, w_mesh* pMesh, unsigned long dwFVF, int iBaseVtxIdx, int nVtxs, int iBaseIdxIdx, int nIdxs);
	void xClear();
	void xCopy(WxBatch** ppBatch);
	void xProcessShader(WView* pView);

	WBone* xpBone;            // 0x0
	w_mesh* xpMesh;           // 0x4
	int xiBaseVtxIdx;         // 0x8
	int xnVtxs;               // 0xc
	int xiBaseIdxIdx;         // 0x10
	int xnIdxs;               // 0x14
	unsigned long xdwFVF;     // 0x18
	WxBatch* xpNext;          // 0x1c
};
CHECK_SIZE(WxBatch, 0x20);
CHECK_FIELD(WxBatch, xpBone, 0x0);
CHECK_FIELD(WxBatch, xpMesh, 0x4);
CHECK_FIELD(WxBatch, xiBaseVtxIdx, 0x8);
CHECK_FIELD(WxBatch, xnVtxs, 0xc);
CHECK_FIELD(WxBatch, xiBaseIdxIdx, 0x10);
CHECK_FIELD(WxBatch, xnIdxs, 0x14);
CHECK_FIELD(WxBatch, xdwFVF, 0x18);
CHECK_FIELD(WxBatch, xpNext, 0x1c);

struct WxBatchGrp {
	WxBatchGrp(WxVb* pVb, WxIb* pIb, int iFlag0, int iFlag1, int iBaseVtxIdx, int nVtxs, int iBaseIdxIdx, int nIdxs);
	void xClear();
	void xCopy(WxBatchGrp** ppGrp);
	void xRenderByBatch(WView* pView, WxBatchState* pState);
	void xRenderAtOnce(WView* pView, WxBatchState* pState, bool b, int* pi, int n);
	void xRenderSubset(WView* pView, WxBatchState* pState, WxBatch* pBatch, int n);

	WxVb* xpVb;               // 0x0
	WxIb* xpIb;               // 0x4
	int xiFlag0;              // 0x8
	int xiFlag1;              // 0xc
	int xiBaseVtxIdx;         // 0x10
	int xnVtxs;               // 0x14
	int xiBaseIdxIdx;         // 0x18
	int xnIdxs;               // 0x1c
	WxBatch* xpaBatches;      // 0x20
	WxBatchGrp* xpNext;       // 0x24
};
CHECK_SIZE(WxBatchGrp, 0x28);
CHECK_FIELD(WxBatchGrp, xpVb, 0x0);
CHECK_FIELD(WxBatchGrp, xpIb, 0x4);
CHECK_FIELD(WxBatchGrp, xiFlag0, 0x8);
CHECK_FIELD(WxBatchGrp, xiFlag1, 0xc);
CHECK_FIELD(WxBatchGrp, xiBaseVtxIdx, 0x10);
CHECK_FIELD(WxBatchGrp, xnVtxs, 0x14);
CHECK_FIELD(WxBatchGrp, xiBaseIdxIdx, 0x18);
CHECK_FIELD(WxBatchGrp, xnIdxs, 0x1c);
CHECK_FIELD(WxBatchGrp, xpaBatches, 0x20);
CHECK_FIELD(WxBatchGrp, xpNext, 0x24);

class WxTnLBuffer {
	LAYOUT_CHECK_FRIEND;
public:
	enum {
		WX_BUF_CLONE = 1,
		WX_BUF_STATIC = 2
	};

	WxTnLBuffer();
	~WxTnLBuffer();

	void xClear();
	void xCopy(WxTnLBuffer** ppBuffer, WVideoDev* pVideo);
	void xSetRootBone(WPuppet* pPet);
	WxBatchGrp* xGetBatchGrps();
	int xCreateBuffers(WVideoDev* pVideo, WPuppet* pPet);
	int xCreateStaticBuffers(WVideoDev* pVideo, WPuppet** ppPets, int nPets);
	int xFillBuffers();
	void xUpdateStaticBufferInfo(WBone* pBone, WBone* pRoot, int index);
	void xUpdateStaticBufferInfo(WPuppet** ppPets, int nPets);
	void xUpdateMatrixIdx(unsigned char* pData, WxBatchGrp* pGrp, WxBatch* pBatch, WBone* pBone, WPuppet* pPet);
	void xUpdateMatrixIdx(WPuppet* pPet);
	void xRender(WView* pView);

private:
	void xUpdateBones(WBone* pBone);
	int xDetermineProperty(WBone* pBone, int a, int b);
	void xSortBatches();
	int xAllocBuffers(int n);
	void xCount(bool b, WxBatchGrp* pGrp, WxBatch* pBatch, WBone* pBone);
	void xGetMatchingBuffers(WxVb** ppVb, WxIb** ppIb, w_mesh* pMesh);
	void xGetMatchingBatches(WxBatchGrp** ppGrp, WxBatch** ppBatch, WxVb* pVb, WxIb* pIb, WBone* pBone, w_mesh* pMesh, int n);
	void xFillVertexBuffer(const WxVb& vb);
	void xFillVertexBufferByBone(unsigned char* pData, WxBatchGrp* pGrp, WxBatch* pBatch, WBone* pBone);
	void xFillVertexBufferByMesh(unsigned char* pData, WxBatchGrp* pGrp, WxBatch* pBatch, w_mesh* pMesh);
	void xFillIndexBuffer(const WxIb& ib);
	void xFillIndexBufferByBone(unsigned char* pData, WxBatchGrp* pGrp, WxBatch* pBatch, WBone* pBone);

	int m_xiFlag;                   // 0x0
	WxVb* m_xpaVbs;                 // 0x4
	WxIb m_xaIbs[32];               // 0x8
	WxBatchGrp* m_xpaBatchGrps;     // 0x188
	WPuppet* m_xpPet;               // 0x18c
	WVideoDev* m_xpVideo;           // 0x190
};
CHECK_SIZE(WxTnLBuffer, 0x194);
BEGIN_LAYOUT_CHECK(WxTnLBuffer)
	CHECK_FIELD(WxTnLBuffer, m_xiFlag, 0x0);
	CHECK_FIELD(WxTnLBuffer, m_xpaVbs, 0x4);
	CHECK_FIELD(WxTnLBuffer, m_xaIbs, 0x8);
	CHECK_FIELD(WxTnLBuffer, m_xpaBatchGrps, 0x188);
	CHECK_FIELD(WxTnLBuffer, m_xpPet, 0x18c);
	CHECK_FIELD(WxTnLBuffer, m_xpVideo, 0x190);
END_LAYOUT_CHECK

void xRender(WView* pView, WxBatchState* pBState, WBone* pBone, w_mesh* pMesh);

#endif
