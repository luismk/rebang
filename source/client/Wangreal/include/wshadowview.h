// Original: d:\build\custom\temp\code\client\wangreal\include\wshadowview.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WSHADOWVIEW_H
#define PG_CLIENT_WANGREAL_INCLUDE_WSHADOWVIEW_H

#include "..\..\defines.h"
#include "wview.h"
#include "wtextureview.h"

class WPuppet;
class Bitmap;
struct WTVertex;

// A view that renders shadow casters into a texture (PDB layout, size
// 0x1b20). The inline virtuals carry their wshadowview.h line.
class WShadowView : public WView {
	LAYOUT_CHECK_FRIEND;
public:
	WShadowView(unsigned char dense);
	virtual ~WShadowView(void);

	void SetShadowDense(unsigned char dense);
	virtual void BeginScene(void) {         // wshadowview.h:20
		update |= UPDATE_VIEW | UPDATE_PROJ;
		UpdateCamera();
	}
	virtual void EndScene(void) {}          // wshadowview.h:24
	virtual bool IsShadowView(void) { return true; }  // wshadowview.h:26

	virtual void SetTextureSize(int width, int height) = 0;
	virtual int GetTexture(void) = 0;
	virtual void Render(void) = 0;
	virtual void Render(WPuppet* puppet, bool, float, bool, bool) = 0;

protected:
	unsigned char m_fgColor;                // 0x1b1c
};
CHECK_SIZE(WShadowView, 0x1b20);
BEGIN_LAYOUT_CHECK(WShadowView)
	CHECK_FIELD(WShadowView, m_fgColor, 0x1b1c);
END_LAYOUT_CHECK

// The software shadow view: rasterises the casters into a system-memory
// buffer (PDB layout, size 0x1b2c).
class WShadowViewSw : public WShadowView {
	LAYOUT_CHECK_FRIEND;
public:
	WShadowViewSw(unsigned char dense);
	virtual ~WShadowViewSw(void);

	virtual void SetTextureSize(int width, int height);
	virtual int GetTexture(void) { return m_texture; }  // wshadowview.h:42
	virtual void Clear(unsigned long clearColor, int mode);
	virtual void Render(void);
	virtual void Render(WPuppet* pet, bool bTransform, float matscale, bool noclipping, bool updatemat);

protected:
	virtual void DrawIndexedTrianglesDirect(WTVertex* vtx, int nvtx, unsigned short* idx, int nidx, int flags, int texhandle);
	void DrawPolygonFanDirect(WTVertex** vl, int drawoption, int n, int drawoption2);
	void Filtering(void);

private:
	void PutPolygonColor(WTVertex** vl);

	int m_texture;                          // 0x1b20
	unsigned char* m_paTargetBuff;          // 0x1b24
	Bitmap* m_pUpdate;                      // 0x1b28
};
CHECK_SIZE(WShadowViewSw, 0x1b2c);
BEGIN_LAYOUT_CHECK(WShadowViewSw)
	CHECK_FIELD(WShadowViewSw, m_texture, 0x1b20);
	CHECK_FIELD(WShadowViewSw, m_paTargetBuff, 0x1b24);
	CHECK_FIELD(WShadowViewSw, m_pUpdate, 0x1b28);
END_LAYOUT_CHECK

// The hardware shadow view: renders the casters into a render target, then
// filters it into a smaller texture view (PDB layout, size 0x1b50).
class WShadowViewHw : public WShadowView {
	LAYOUT_CHECK_FRIEND;
public:
	WShadowViewHw(unsigned char dense);
	virtual ~WShadowViewHw(void);

	virtual void SetTextureSize(int w, int h);
	virtual int GetTexture(void) {          // wshadowview.h:76
		return m_filteredTexView ? m_filteredTexView->GetTexture(0) : 0;
	}
	virtual void Clear(unsigned long clearColor, int mode);
	virtual void Render(void);
	virtual void Render(WPuppet* pet, bool bTransform, float matscale, bool noclipping, bool updatemat);

private:
	static WTVertex m_vtx[4];
	static WTVertex* m_vl[5];
	WRenderToTextureParam m_orgTexParam;    // 0x1b20
	WTextureView* m_filteredTexView;        // 0x1b48
	bool m_r2tBegun;                        // 0x1b4c
};
CHECK_SIZE(WShadowViewHw, 0x1b50);
BEGIN_LAYOUT_CHECK(WShadowViewHw)
	CHECK_FIELD(WShadowViewHw, m_orgTexParam, 0x1b20);
	CHECK_FIELD(WShadowViewHw, m_filteredTexView, 0x1b48);
	CHECK_FIELD(WShadowViewHw, m_r2tBegun, 0x1b4c);
END_LAYOUT_CHECK

// A view that collects and renders the triangles receiving a shadow (PDB
// layout, size 0x1b20).
class WRecvView : public WView {
	LAYOUT_CHECK_FRIEND;
public:
	WRecvView(void);
	virtual ~WRecvView(void);

	void SetRenderView(WView* view);
	WView* GetRenderView(void);
	virtual void BeginScene(void) {         // wshadowview.h:107
		update |= UPDATE_VIEW | UPDATE_PROJ;
		UpdateCamera();
	}
	virtual void Clear(unsigned long flags, int color) = 0;
	virtual void EndScene(void) {}          // wshadowview.h:112
	virtual bool IsShadowView(void) { return true; }  // wshadowview.h:114

	virtual void CollectRecvTris(WPuppet* puppet, bool, float, bool, bool) = 0;
	virtual bool HasRecver(void) = 0;
	virtual void Render(WShadowView* shadow, WPuppet* puppet) = 0;

protected:
	WView* m_renderview;                    // 0x1b1c
};
CHECK_SIZE(WRecvView, 0x1b20);
BEGIN_LAYOUT_CHECK(WRecvView)
	CHECK_FIELD(WRecvView, m_renderview, 0x1b1c);
END_LAYOUT_CHECK

// The software receiver view: collects the receiving triangles into its own
// vertex and face lists (PDB layout, size 0x1b38).
class WRecvViewSw : public WRecvView {
	LAYOUT_CHECK_FRIEND;
public:
	WRecvViewSw(void);
	virtual ~WRecvViewSw(void);

	virtual void Clear(unsigned long clearColor, int mode);
	void DrawPolygonFan(WTVertex** vl, int clipoption, int drawoption);
	virtual void DrawIndexedTriangles(WTVertex* vtx, int nvtx, unsigned short* idx, int nidx, int flags, int texhandle);
	virtual void CollectRecvTris(WPuppet* puppet, bool, float, bool, bool);
	virtual bool HasRecver(void) { return m_cur_face > 0 ? true : false; }  // wshadowview.h:146
	virtual void Render(WShadowView* shadow, WPuppet* puppet);

private:
	void AllocVtxBuffMore(void);
	void AddPolygonList(WTVertex** vl);
	void AddPolygonIdxList(WTVertex** vl, int nvtx, unsigned short* idx, int nidx);
	static int __cdecl CompareDepthPoly(const void* elem1, const void* elem2);
	void SortFace(void);
	int GetClipCode(float x, float y, float z);
	void ClipEdge(WTVertex* out, const WTVertex* a, const WTVertex* b, float t);

	static WTVertex** g_vtxIdxList;
	int m_cur_vtx;                          // 0x1b20
	int m_cur_face;                         // 0x1b24
	WTVertex** m_vtxIdxList;                // 0x1b28
	WTVertex* m_vtxList;                    // 0x1b2c
	unsigned short* m_faceList;             // 0x1b30
	int m_allocLen;                         // 0x1b34
	static WTVertex** plist;
	static WTVertex* pl;
	static unsigned short* flist;
	static unsigned short* fl;
	static int m_count;
};
CHECK_SIZE(WRecvViewSw, 0x1b38);
BEGIN_LAYOUT_CHECK(WRecvViewSw)
	CHECK_FIELD(WRecvViewSw, m_cur_vtx, 0x1b20);
	CHECK_FIELD(WRecvViewSw, m_cur_face, 0x1b24);
	CHECK_FIELD(WRecvViewSw, m_vtxIdxList, 0x1b28);
	CHECK_FIELD(WRecvViewSw, m_vtxList, 0x1b2c);
	CHECK_FIELD(WRecvViewSw, m_faceList, 0x1b30);
	CHECK_FIELD(WRecvViewSw, m_allocLen, 0x1b34);
END_LAYOUT_CHECK

// The hardware receiver view: renders the receivers again with the shadow
// texture projected on them (PDB layout, size 0x1b24).
class WRecvViewHw : public WRecvView {
	LAYOUT_CHECK_FRIEND;
public:
	WRecvViewHw(void);
	virtual ~WRecvViewHw(void);

	virtual void Clear(unsigned long clearColor, int mode);
	virtual void CollectRecvTris(WPuppet* puppet, bool, float, bool, bool);
	virtual bool HasRecver(void) { return m_hasRecver; }  // wshadowview.h:197
	virtual void Render(WShadowView* shadow, WPuppet* puppet);

private:
	void MakeEqualToRenderView(void);

	bool m_hasRecver;                       // 0x1b20
};
CHECK_SIZE(WRecvViewHw, 0x1b24);
BEGIN_LAYOUT_CHECK(WRecvViewHw)
	CHECK_FIELD(WRecvViewHw, m_hasRecver, 0x1b20);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WSHADOWVIEW_H
