// Original: d:\build\custom\temp\code\client\projectg\sky.h
#ifndef PG_CLIENT_PROJECTG_SKY_H
#define PG_CLIENT_PROJECTG_SKY_H

#include "..\..\defines.h"
#include "wvideo.h"

class CFxBox;
class CLensFlare;
class WView;

class sCloud {
	LAYOUT_CHECK_FRIEND;
public:
	sCloud(void);

	float m_rotSpeed;      // 0x0, random in [-0.1, 0.1]
	float m_size;          // 0x4, random in [256, 512]
	WVector m_pos;         // 0x8
	float m_alpha;         // 0x14, 0..255, set by CSky::Process
	bool m_hide;           // 0x18
	PADDING(0x19, 0x1C);
	int m_texIndex;        // 0x1c, random in [0, 3]
	bool m_flip;           // 0x20, random
	PADDING(0x21, 0x24);
};
CHECK_SIZE(sCloud, 0x24);
BEGIN_LAYOUT_CHECK(sCloud)
	CHECK_FIELD(sCloud, m_rotSpeed, 0x0);
	CHECK_FIELD(sCloud, m_size, 0x4);
	CHECK_FIELD(sCloud, m_pos, 0x8);
	CHECK_FIELD(sCloud, m_alpha, 0x14);
	CHECK_FIELD(sCloud, m_hide, 0x18);
	CHECK_FIELD(sCloud, m_texIndex, 0x1C);
	CHECK_FIELD(sCloud, m_flip, 0x20);
END_LAYOUT_CHECK

class CSky {
	LAYOUT_CHECK_FRIEND;
public:
	CSky(void);
	~CSky(void);
	void Clear(void);
	void ShowClouds(bool show);
	void ClearSky(void);
	void ChangeWeatherTex(unsigned char index);
	void ClearClouds(void);
	void RegisterCloud(char* name);
	void UpdateBright(void);
	void LoadSky(const char* name, float param);
	void GenerateStars(int count);
	void GenerateLensFlare(void);
	void GenerateMoons(void);
	void GenerateClouds(int count, float density);
	void Render(WView* view);
	void Process(float elapsed);

	// sky.h:76
	void SetColor(unsigned long color) { m_skyColor = color; }

protected:
	void RenderUpAndBottom(WView* view);
	void RenderFarCylinder(WView* view);
	void RenderNearCylinder(WView* view);
	void RenderFlatCloud(WView* view);
	void RenderBillboardCloud(WView* view);

private:
	void LoadTexByWeather(const char* name, const char* layer);
	int IsThereTexHandle(const char* name, int index);
	void SetSkyTex(int texhandle, unsigned char layer, unsigned char batch);
	float CalcScaleByView(WView* view) const;
	float CalcClipFarByView(WView* view) const;

	int m_farTex[5];               // 0x0
	int m_nearTex[5];              // 0x14
	int m_upTex[2];                // 0x28
	int m_dnTex[2];                // 0x30
	int m_loaded;                  // 0x38, set by LoadSky, cleared by Clear
	WxBatchState m_batch[4];       // 0x3c: far, near, up, down
	float m_cloudRadius;           // 0x1ac, spawn radius of a cloud
	float m_cloudFadeRange;        // 0x1b0
	float m_cloudHeight;           // 0x1b4
	int m_cloudTex;                // 0x1b8
	int m_cloudCount;              // 0x1bc
	sCloud m_clouds[50];           // 0x1c0
	int m_cloudGenCount;           // 0x8c8, GenerateClouds calls since the last clear
	float m_cloudInterval;         // 0x8cc, seconds between two clouds
	float m_cloudTimer;            // 0x8d0
	float m_rotateSpeed;           // 0x8d4, added to the rotation each Render
	unsigned long m_skyColor;      // 0x8d8
	float m_loadParam;             // 0x8dc, LoadSky's float argument
	int m_texColumns;              // 0x8e0, at least 1
	int m_texRows;                 // 0x8e4, at least 1
	PADDING(0x8E8, 0x9A8);         // members not transcribed yet
	WVector m_viewPos;             // 0x9a8, the camera position copied by Render
	CLensFlare* m_lensFlare;       // 0x9b4
	CFxBox* m_fxBox;               // 0x9b8
	CFxBox* m_pvsFxBox;            // 0x9bc
	float m_upAndBottomAngle;      // 0x9c0, y rotation of the up layer
	float m_nearCylinderAngle;     // 0x9c4, y rotation of the near cylinder
};
CHECK_SIZE(CSky, 0x9C8);
BEGIN_LAYOUT_CHECK(CSky)
	CHECK_FIELD(CSky, m_farTex, 0x0);
	CHECK_FIELD(CSky, m_nearTex, 0x14);
	CHECK_FIELD(CSky, m_upTex, 0x28);
	CHECK_FIELD(CSky, m_dnTex, 0x30);
	CHECK_FIELD(CSky, m_loaded, 0x38);
	CHECK_FIELD(CSky, m_batch, 0x3C);
	CHECK_FIELD(CSky, m_cloudRadius, 0x1AC);
	CHECK_FIELD(CSky, m_cloudFadeRange, 0x1B0);
	CHECK_FIELD(CSky, m_cloudHeight, 0x1B4);
	CHECK_FIELD(CSky, m_cloudTex, 0x1B8);
	CHECK_FIELD(CSky, m_cloudCount, 0x1BC);
	CHECK_FIELD(CSky, m_cloudGenCount, 0x8C8);
	CHECK_FIELD(CSky, m_cloudInterval, 0x8CC);
	CHECK_FIELD(CSky, m_cloudTimer, 0x8D0);
	CHECK_FIELD(CSky, m_rotateSpeed, 0x8D4);
	CHECK_FIELD(CSky, m_skyColor, 0x8D8);
	CHECK_FIELD(CSky, m_loadParam, 0x8DC);
	CHECK_FIELD(CSky, m_texColumns, 0x8E0);
	CHECK_FIELD(CSky, m_texRows, 0x8E4);
	CHECK_FIELD(CSky, m_clouds, 0x1C0);
	CHECK_FIELD(CSky, m_lensFlare, 0x9B4);
	CHECK_FIELD(CSky, m_fxBox, 0x9B8);
	CHECK_FIELD(CSky, m_pvsFxBox, 0x9BC);
	CHECK_FIELD(CSky, m_viewPos, 0x9A8);
	CHECK_FIELD(CSky, m_upAndBottomAngle, 0x9C0);
	CHECK_FIELD(CSky, m_nearCylinderAngle, 0x9C4);
END_LAYOUT_CHECK

#endif
