// Original: d:\build\custom\temp\code\client\wangreal\source\wtextureview.cpp
#include <math.h>

#include "../include/wtextureview.h"
#include "../include/wpuppet.h"
#include "../include/bitmap.h"
#include "../include/wutil.h"

// The original's max compiles as `a < b ? b : a` (wutil.h Max uses >= and
// gives the other branch); its header is unknown, as for river.cpp's MaxOf.
template <class T> T MaxOf(T a, T b) { return a < b ? b : a; }

// wtextureview.cpp:19
WTextureView::WTextureView(void) {
	SetCamera(WMatrix::IDENTITY);
	m_r2tBegun = false;
	m_bDisableFog = true;
}

// wtextureview.cpp:26
WTextureView::~WTextureView(void) {
	for (int i = 0; i < WRenderToTextureParam::NUM_SIMULTANEOUSRTS; ++i) {
		if (GetResrcManager() && m_r2tParam.m_rtTexInfo[i].m_hTex) { GetResrcManager()->Release(m_r2tParam.m_rtTexInfo[i].m_hTex); m_r2tParam.m_rtTexInfo[i].m_hTex = 0; }
	}
	if (GetResrcManager() && m_r2tParam.m_depthSurfInfo.m_hTex) { GetResrcManager()->Release(m_r2tParam.m_depthSurfInfo.m_hTex); m_r2tParam.m_depthSurfInfo.m_hTex = 0; }
}

// wtextureview.cpp:36
void WTextureView::SetTextureParam(const TextureParam& param) {
	m_texParam = param;
}

// wtextureview.cpp:74
void WTextureView::Begin(void) {
	WVideoDev* video = GetVideoDevice();
	if (!m_texParam.m_rtTexInfo[0].m_sizeInfo.m_isAbsolute) {
		float width = floor((float)video->GetWidth() * m_texParam.m_rtTexInfo[0].m_sizeInfo.m_width + 0.5f);
		float height = floor((float)video->GetHeight() * m_texParam.m_rtTexInfo[0].m_sizeInfo.m_width + 0.5f);

		if (GetWidth() != width || GetHeight() != height) {
			SetViewport(width, height);
		}
	}
	if (m_r2tParam.m_rtTexInfo[0].m_hTex == 0) {
		CreateTexture();
	}

	video->EndScene();

	m_r2tBegun = false;
	if (m_r2tParam.m_rtTexInfo[0].m_hTex > 0) {
		m_r2tBegun = video->BeginRenderToTexture(m_r2tParam);
	}

	if (m_r2tBegun) {
		m_r2tBegun = video->BeginScene();
	}
}

// wtextureview.cpp:105
void WTextureView::End(void) {
	WVideoDev* video = GetVideoDevice();

	if (m_r2tBegun) {
		video->EndScene();

		video->EndRenderToTexture(m_r2tParam);
	}

	video->BeginScene();

	m_r2tBegun = false;
}

// wtextureview.cpp:125
void WTextureView::CreateTexture(void) {
	tagBITMAPINFO bi;
	WVideoDev* video = GetVideoDevice();
	int width;
	int height;

	if (video->GetDeviceState() == W_VDEVSTATE_LOST) {
		return;
	}

	Bitmap bitmap;

	unsigned int i;
	for (i = 0; i < m_texParam.m_numRts; ++i) {
		if (m_texParam.m_rtTexInfo[i].m_sizeInfo.m_isAbsolute) {
			width = (int)GetWidth();
			height = (int)GetHeight();
		} else {
			width = (int)floor(video->GetWidth() * m_texParam.m_rtTexInfo[i].m_sizeInfo.m_width + 0.5f);
			height = (int)floor(video->GetHeight() * m_texParam.m_rtTexInfo[i].m_sizeInfo.m_height + 0.5f);
		}

		bi.bmiHeader.biWidth = width;
		bi.bmiHeader.biHeight = height;
		bi.bmiHeader.biBitCount = 24;
		bitmap.SetBITMAPINFO(&bi, NULL);

		m_r2tParam.m_rtTexInfo[i].m_hTex = GetResrcManager()->UploadTexture(NULL, &bitmap, m_texParam.m_rtTexInfo[i].m_texStyle, NULL);

		if (m_r2tParam.m_rtTexInfo[i].m_hTex > 0) {
			video->SetRenderTargetSizeInfo(m_r2tParam.m_rtTexInfo[i].m_hTex, m_texParam.m_rtTexInfo[i].m_sizeInfo);
		}
		m_r2tParam.m_rtTexInfo[i].m_needToClear = m_texParam.m_rtTexInfo[i].m_needToClear;
		m_r2tParam.m_rtTexInfo[i].m_clearClr = m_texParam.m_rtTexInfo[i].m_clearClr;
	}

	if (m_texParam.m_depthSurfInfo.m_surfUsage == WRenderToTextureParam::DepthSurfInfo::USE_EXCLUSIVE_SURFACE) {
		TextureParam::DepthSurfInfo& requested = m_texParam.m_depthSurfInfo;
		if (requested.m_sizeInfo.m_isAbsolute) {
			width = (int)GetWidth();
			height = (int)GetHeight();
		} else {
			width = (int)floor(video->GetWidth() * requested.m_sizeInfo.m_width + 0.5f);
			height = (int)floor(video->GetHeight() * requested.m_sizeInfo.m_height + 0.5f);
		}

		bi.bmiHeader.biWidth = width;
		bi.bmiHeader.biHeight = height;
		bi.bmiHeader.biBitCount = 24;
		bitmap.SetBITMAPINFO(&bi, NULL);

		m_r2tParam.m_depthSurfInfo.m_hTex = GetResrcManager()->UploadTexture(NULL, &bitmap, 0x8000, NULL);

		if (m_r2tParam.m_depthSurfInfo.m_hTex > 0) {
			video->SetRenderTargetSizeInfo(m_r2tParam.m_depthSurfInfo.m_hTex, requested.m_sizeInfo);
		}
	}

	m_r2tParam.m_depthSurfInfo.m_surfUsage = m_texParam.m_depthSurfInfo.m_surfUsage;
	m_r2tParam.m_depthSurfInfo.m_needToClear = m_texParam.m_depthSurfInfo.m_needToClear;
	m_r2tParam.m_depthSurfInfo.m_clearZ = m_texParam.m_depthSurfInfo.m_clearZ;
}

// wtextureview.cpp:204
WTextureImposterView::WTextureImposterView(WPuppet* target) : m_target(target), m_tolerance(0.17453292f) {
	float size;
	for (size = 16.0f; !(size > m_target->GetBoundSphere().radius); size += size) {
	}
	SetViewport(size, size);
}

// wtextureview.cpp:222
bool WTextureImposterView::NeedToUpdate(const WMatrix& cam) {
	WMatrix camMat = cam;

	const WSphere& sphere = m_target->GetBoundSphere();
	WVector toTarget = sphere.pos - camMat.pivot;

	camMat.za = toTarget;
	camMat.za.Normalize();

	if (m_r2tParam.m_rtTexInfo[0].m_hTex > 0
		&& GetVideoDevice()->IsTextureFilled(m_r2tParam.m_rtTexInfo[0].m_hTex)) {
		float dot = camMat.za * GetLastCamera().za;
		dot = dot <= 0.0f ? -dot : dot;
		if (dot > cos(m_tolerance)) {
			return false;
		}
	}

	camMat.ya = WVector::UNIT_POS_Y;
	camMat.xa = WCrossProduct(camMat.ya, camMat.za);
	camMat.ya = WCrossProduct(camMat.za, camMat.xa);
	float dist = toTarget.Magnitude();

	SetFOV(asinf(sphere.radius / dist) * 1280.0f / 480.0f);
	float nearClip = dist - sphere.radius;
	SetClip(nearClip < 0.0f ? 0.0f : nearClip, dist + sphere.radius, false);

	SetCamera(camMat);
	UpdateCamera();

	return true;
}
