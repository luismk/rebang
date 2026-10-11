// Original: d:\build\custom\temp\code\client\wangreal\include\wtextureview.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WTEXTUREVIEW_H
#define PG_CLIENT_WANGREAL_INCLUDE_WTEXTUREVIEW_H

#include "..\..\defines.h"
#include "wview.h"
#include "wvideo.h"

class WPuppet;

// Render-to-texture descriptors of the video device (PDB layouts). They
// belong to wvideo.h, which only forward-declares them so far.
struct WRenderToTextureSizeInfo {
	bool m_isAbsolute;                      // 0x0
	float m_width;                          // 0x4
	float m_height;                         // 0x8

	WRenderToTextureSizeInfo(void) {
		Reset();
	}
	WRenderToTextureSizeInfo(bool isAbsolute, float width, float height)
		: m_isAbsolute(isAbsolute), m_width(width), m_height(height) {
	}
	void Reset(void) {
		m_isAbsolute = true;
		m_width = 0;
		m_height = 0;
	}

	static const WRenderToTextureSizeInfo SIZE_FULL;
	static const WRenderToTextureSizeInfo SIZE_HALF;
	static const WRenderToTextureSizeInfo SIZE_ABS;
};
CHECK_SIZE(WRenderToTextureSizeInfo, 0xc);
CHECK_FIELD(WRenderToTextureSizeInfo, m_isAbsolute, 0x0);
CHECK_FIELD(WRenderToTextureSizeInfo, m_width, 0x4);
CHECK_FIELD(WRenderToTextureSizeInfo, m_height, 0x8);

struct WRenderToTextureParam {
	enum {
		NUM_SIMULTANEOUSRTS = 2
	};

	struct RtTexInfo {
		int m_hTex;                         // 0x0
		bool m_needToClear;                 // 0x4
		unsigned long m_clearClr;           // 0x8

		RtTexInfo(void) : m_hTex(0), m_needToClear(false), m_clearClr(0) {
		}
	};

	struct DepthSurfInfo {
		enum SurfaceUsage {
			USE_NOT_NEEDED = 0,
			USE_MAIN_SURFACE = 1,
			USE_SHARED_SURFACE = 2,
			USE_EXCLUSIVE_SURFACE = 3
		};

		SurfaceUsage m_surfUsage;           // 0x0
		int m_hTex;                         // 0x4
		bool m_needToClear;                 // 0x8
		float m_clearZ;                     // 0xc

		DepthSurfInfo(void) : m_surfUsage(USE_NOT_NEEDED), m_hTex(0), m_needToClear(false), m_clearZ(1.0f) {
		}
	};

	RtTexInfo m_rtTexInfo[NUM_SIMULTANEOUSRTS];  // 0x0
	DepthSurfInfo m_depthSurfInfo;          // 0x18

	// wvideo.h:509
	int GetTexture(unsigned int index) const {
		if (index < NUM_SIMULTANEOUSRTS)
			return m_rtTexInfo[index].m_hTex;
		else
			return 0;
	}
	bool CanClearAtOnce(void);
};
CHECK_SIZE(WRenderToTextureParam::RtTexInfo, 0xc);
CHECK_SIZE(WRenderToTextureParam::DepthSurfInfo, 0x10);
CHECK_SIZE(WRenderToTextureParam, 0x28);
CHECK_FIELD(WRenderToTextureParam, m_rtTexInfo, 0x0);
CHECK_FIELD(WRenderToTextureParam, m_depthSurfInfo, 0x18);

// A view rendering into textures (PDB layout, size 0x1b94).
class WTextureView : public WView {
	LAYOUT_CHECK_FRIEND;
	// WShadowViewHw::Render reads m_filteredTexView->SCREEN_XS/SCREEN_YS directly
	// (no inline-return spill, unlike GetWidth()); the PDB tools do not list friends.
	friend class WShadowViewHw;
public:
	struct TextureParam {
		struct RtTexInfo {
			unsigned long m_texStyle;               // 0x00
			WRenderToTextureSizeInfo m_sizeInfo;    // 0x04
			bool m_needToClear;                     // 0x10
			unsigned long m_clearClr;               // 0x14

			RtTexInfo(void) {
				Reset();
			}
			void Reset(void) {
				m_texStyle = 0;
				m_sizeInfo.Reset();
				m_needToClear = false;
				m_clearClr = 0;
			}
			// wtextureview.h:35
			void Set(unsigned long texStyle, const WRenderToTextureSizeInfo& sizeInfo, bool needToClear, unsigned long clearClr) {
				m_texStyle = texStyle;
				m_sizeInfo = sizeInfo;
				m_needToClear = needToClear;
				m_clearClr = clearClr;
			}
		};
		struct DepthSurfInfo {
			WRenderToTextureParam::DepthSurfInfo::SurfaceUsage m_surfUsage;  // 0x00
			WRenderToTextureSizeInfo m_sizeInfo;    // 0x04
			bool m_needToClear;                     // 0x10
			float m_clearZ;                         // 0x14

			DepthSurfInfo(void) {
				Reset();
			}
			void Reset(void) {
				m_surfUsage = WRenderToTextureParam::DepthSurfInfo::USE_NOT_NEEDED;
				m_sizeInfo.Reset();
				m_needToClear = false;
				m_clearZ = 1.0f;
			}
			void Set(WRenderToTextureParam::DepthSurfInfo::SurfaceUsage surfUsage, const WRenderToTextureSizeInfo& sizeInfo, bool needToClear, float clearZ) {
				m_surfUsage = surfUsage;
				m_sizeInfo = sizeInfo;
				m_needToClear = needToClear;
				m_clearZ = clearZ;
			}
		};

		unsigned int m_numRts;                  // 0x00
		RtTexInfo m_rtTexInfo[2];               // 0x04
		DepthSurfInfo m_depthSurfInfo;          // 0x34

		// wtextureview.h:76
		TextureParam(void) {
			Reset();
		}
		void Reset(void) {
			m_numRts = 0;
			for (int i = 0; i < 2; ++i) {
				m_rtTexInfo[i].Reset();
			}
			m_depthSurfInfo.Reset();
		}
	};

	WTextureView(void);
	virtual ~WTextureView(void);

	void SetTextureParam(const TextureParam& param);
	// wtextureview.h:99
	int GetTexture(unsigned int index) const { return m_r2tParam.GetTexture(index); }
	void Begin(void);
	void End(void);

protected:
	void CreateTexture(void);

	TextureParam m_texParam;                // 0x1b1c
	WRenderToTextureParam m_r2tParam;       // 0x1b68
	bool m_r2tBegun;                        // 0x1b90
};
CHECK_SIZE(WTextureView::TextureParam::RtTexInfo, 0x18);
CHECK_SIZE(WTextureView::TextureParam::DepthSurfInfo, 0x18);
CHECK_SIZE(WTextureView::TextureParam, 0x4c);
CHECK_SIZE(WTextureView, 0x1b94);
BEGIN_LAYOUT_CHECK(WTextureView)
	CHECK_FIELD(WTextureView, m_texParam, 0x1b1c);
	CHECK_FIELD(WTextureView, m_r2tParam, 0x1b68);
	CHECK_FIELD(WTextureView, m_r2tBegun, 0x1b90);
END_LAYOUT_CHECK

// A texture view holding an impostor of one puppet (PDB layout, size
// 0x1b9c).
class WTextureImposterView : public WTextureView {
	LAYOUT_CHECK_FRIEND;
public:
	WTextureImposterView(WPuppet* target);

	bool NeedToUpdate(const WMatrix& cam);

private:
	const WPuppet* m_target;                // 0x1b94
	float m_tolerance;                      // 0x1b98
};
CHECK_SIZE(WTextureImposterView, 0x1b9c);
BEGIN_LAYOUT_CHECK(WTextureImposterView)
	CHECK_FIELD(WTextureImposterView, m_target, 0x1b94);
	CHECK_FIELD(WTextureImposterView, m_tolerance, 0x1b98);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WTEXTUREVIEW_H
