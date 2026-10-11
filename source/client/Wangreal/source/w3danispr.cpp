// Original: d:\build\custom\temp\code\client\wangreal\source\w3danispr.cpp
#include <math.h>
#include <stdio.h>

#include "../include/w3danispr.h"
#include "../include/wresrcmng.h"
#include "../include/wview.h"

// The four corners as (1 = w1/h1, 2 = w2/h2) pairs, in drawing order.
static int corner[4][2] = { { 1, 2 }, { 2, 2 }, { 2, 1 }, { 1, 1 } };

// Texture coordinates of the four corners, in texels of a 256 grid.
static int uv[4][2] = { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 } };

// w3danispr.cpp:19
W3dAniSpr::W3dAniSpr(void) : m_TextureList(8, 0), m_SpriteList(8, 0) {
	for (int i = 0; i < 4; i++) {
		vl[i] = &vtx[i];
		vl[i]->tv = (uv[i][0] * 255.0f + 0.5f) / 256.0f;
		vl[i]->tu = (uv[i][1] * 255.0f + 0.5f) / 256.0f;

		vl[i]->diffuse = 0xffffffff;
	}
	vl[4] = NULL;
	m_nTotalSprite = 0;
}

// w3danispr.cpp:36
W3dAniSpr::~W3dAniSpr(void) {
	AllDelSprite();

	int hTexture;
	while ((hTexture = m_TextureList.Start()) != 0) {
		GetResrcManager()->Release(hTexture);
		m_TextureList -= hTexture;
	}
}

// w3danispr.cpp:53
void W3dAniSpr::SetPos(const WVector& vec) {
	pos = vec;
}

// w3danispr.cpp:58
void W3dAniSpr::Render(WView* view, int type, w_spr_align align, int nSprNum) {
	_WSPRITE* pSpr = FindSprite(nSprNum);
	if (!pSpr) return;

	vtx[0].tu = pSpr->fStartV;
	vtx[0].tv = pSpr->fStartU;
	vtx[1].tu = pSpr->fEndV;
	vtx[1].tv = pSpr->fStartU;
	vtx[2].tu = pSpr->fEndV;
	vtx[2].tv = pSpr->fEndU;
	vtx[3].tu = pSpr->fStartV;
	vtx[3].tv = pSpr->fEndU;

	switch (align) {
	case CAMERA_XY_ALIGN:








		vl[0]->SetPosition(pos + view->camera.xa * w1 + view->camera.ya * h2);
		vl[1]->SetPosition(pos + view->camera.xa * w2 + view->camera.ya * h2);
		vl[2]->SetPosition(pos + view->camera.xa * w2 + view->camera.ya * h1);
		vl[3]->SetPosition(pos + view->camera.xa * w1 + view->camera.ya * h1);

		break;
	case CAMERA_X_ALIGN: {


		WVector ya = WVector::UNIT_POS_Y;






		vl[0]->SetPosition(pos + view->camera.xa * w1 + ya * h2);
		vl[1]->SetPosition(pos + view->camera.xa * w2 + ya * h2);
		vl[2]->SetPosition(pos + view->camera.xa * w2 + ya * h1);
		vl[3]->SetPosition(pos + view->camera.xa * w1 + ya * h1);

		break;
	}
	case X_ALIGN:








		vl[0]->SetPosition(pos + WVector(w1, h2, 0.0f));
		vl[1]->SetPosition(pos + WVector(w2, h2, 0.0f));
		vl[2]->SetPosition(pos + WVector(w2, h1, 0.0f));
		vl[3]->SetPosition(pos + WVector(w1, h1, 0.0f));

		break;
	case Y_ALIGN:







		vl[0]->SetPosition(pos + WVector(0.0f, h2, w1));
		vl[1]->SetPosition(pos + WVector(0.0f, h2, w2));
		vl[2]->SetPosition(pos + WVector(0.0f, h1, w2));
		vl[3]->SetPosition(pos + WVector(0.0f, h1, w1));

		break;
	case Z_ALIGN:







		vl[0]->SetPosition(pos + WVector(w1, 0.0f, h2));
		vl[1]->SetPosition(pos + WVector(w2, 0.0f, h2));
		vl[2]->SetPosition(pos + WVector(w2, 0.0f, h1));
		vl[3]->SetPosition(pos + WVector(w1, 0.0f, h1));
	}







	if (type & 0x4000000) {





		SetColor(0xffffffff); view->DrawPolygonFan(vl, type, 0x400, false);







	} else view->DrawPolygonFan(vl, (pSpr->hTexture & 0x7ff) | type, 0x400, false);



}

// w3danispr.cpp:168
void W3dAniSpr::Render(WView* view, float angle, int type, int nSprNum) {




	_WSPRITE* pSprite = FindSprite(nSprNum);
	if (!pSprite) return;

	vtx[0].tu = pSprite->fStartV; vtx[0].tv = pSprite->fStartU; vtx[1].tu = pSprite->fEndV; vtx[1].tv = pSprite->fStartU; vtx[2].tu = pSprite->fEndV; vtx[2].tv = pSprite->fEndU; vtx[3].tu = pSprite->fStartV; vtx[3].tv = pSprite->fEndU;










	for (int i = 0; i < 4; i++) {
		float x, y;
		Rotate(corner[i][0] == 1 ? w1 : w2, corner[i][1] == 1 ? h1 : h2, x, y, angle);



		vl[i]->SetPosition(view->camera.xa * x + pos + view->camera.ya * y);
	}



	if (type & 0x4000000) {





		SetColor(0xffffffff); view->DrawPolygonFan(vl, type, 0x400, false);







	} else view->DrawPolygonFan(vl, (pSprite->hTexture & 0x7ff) | type, 0x400, false);



}

// w3danispr.cpp:216
void W3dAniSpr::Render(WView* view, const WVector& angle, int type, int nSprNum) {



	_WSPRITE* pSprite = FindSprite(nSprNum);
	if (!pSprite) return;

	vtx[0].tu = pSprite->fStartV;
	vtx[0].tv = pSprite->fStartU;
	vtx[1].tu = pSprite->fEndV;
	vtx[1].tv = pSprite->fStartU;
	vtx[2].tu = pSprite->fEndV;
	vtx[2].tv = pSprite->fEndU;
	vtx[3].tu = pSprite->fStartV;
	vtx[3].tv = pSprite->fEndU;

	for (int i = 0; i < 4; i++) {

		WVector p = WVector(corner[i][0] == 1 ? w1 : w2, corner[i][1] == 1 ? h1 : h2, 0.0f) * RotMat(angle);



		vl[i]->SetPosition(p + pos);
	}



	if (type & 0x4000000) {





		SetColor(0xffffffff); view->DrawPolygonFan(vl, type, 0x400, false);







	} else view->DrawPolygonFan(vl, (pSprite->hTexture & 0x7ff) | type, 0x400, false);



}

// w3danispr.cpp:263
void W3dAniSpr::Render(WView* view, const WMatrix& rot, int type, int nSprNum) {



	_WSPRITE* pSprite = FindSprite(nSprNum);
	if (!pSprite) return;

	vtx[0].tu = pSprite->fStartV;
	vtx[0].tv = pSprite->fStartU;
	vtx[1].tu = pSprite->fEndV;
	vtx[1].tv = pSprite->fStartU;
	vtx[2].tu = pSprite->fEndV;
	vtx[2].tv = pSprite->fEndU;
	vtx[3].tu = pSprite->fStartV;
	vtx[3].tv = pSprite->fEndU;

	for (int i = 0; i < 4; i++) {

		WVector p = WVector(corner[i][0] == 1 ? w1 : w2, corner[i][1] == 1 ? h1 : h2, 0.0f) * rot;



		vtx[i].SetPosition(p + pos);
	}









	if (type & 0x4000000) {







		SetColor(0xffffffff); view->DrawPolygonFan(vl, type, 0x400, false);
	} else view->DrawPolygonFan(vl, (pSprite->hTexture & 0x7ff) | type, 0x400, false);



}

// w3danispr.cpp:310
void W3dAniSpr::Rotate(float x, float y, float& tx, float& ty, float angle) {
	float cos = cosf(angle), sin = sinf(angle);
	tx = cos * x - sin * y;
	ty = sin * x + cos * y;
}

// w3danispr.cpp:317
void W3dAniSpr::SetRect(float width, float height, float pivot_x, float pivot_y) {
	w1 = -pivot_x;
	w2 = width - pivot_x;
	h1 = -pivot_y;
	h2 = height - pivot_y;
}

// w3danispr.cpp:325
void W3dAniSpr::SetColor(const int color) {
	for (int i = 0; i < 4; i++) vl[i]->diffuse = color;
}

// w3danispr.cpp:331
int W3dAniSpr::LoadSprite(const char* filename, int type) {
	int hTexture = GetResrcManager()->LoadTexture(filename, type, 0, 0);
	char buf[260];













	_WSPRITE* pSprite = new _WSPRITE; pSprite->hTexture = hTexture; pSprite->fStartU = 0.0f; pSprite->fStartV = 0.0f; pSprite->fEndU = 1.0f; pSprite->fEndV = 1.0f; AddSprite(pSprite); m_TextureList += hTexture;

	if (hTexture) {


		sprintf(buf, "W3dAniSpr:%s", filename);
		SetLeakHint(buf);
	}


	return hTexture == 0;
}

// w3danispr.cpp:359
int W3dAniSpr::LoadSpritesInOneTexture(const char* filename, int type, float fSprSizeX, float fSprSizeY) {
	int handle = GetResrcManager()->LoadTexture(filename, type, 0, 0);
	float fWidth = (float)GetResrcManager()->GetTextureWidth(handle);
	float fHeight = (float)GetResrcManager()->GetTextureHeight(handle);
	int nx = (int)(fWidth / fSprSizeX);
	int ny = (int)(fHeight / fSprSizeY);
	float fUnitX = 1.0f / (fWidth / fSprSizeX);
	float fUnitY = 1.0f / (fHeight / fSprSizeY);
	float fx = 0.0f;
	float fy = 0.0f;
	for (int y = 0; y < ny; ++y) {
		for (int x = 0; x < nx; ++x) {
			_WSPRITE* pSprite = new _WSPRITE;
			pSprite->hTexture = handle;
			pSprite->fStartU = fy;
			pSprite->fStartV = fx;
			pSprite->fEndU = fy + fUnitY;
			pSprite->fEndV = fx + fUnitX;
			AddSprite(pSprite);
			fx += fUnitX;
		}
		fx = 0.0f;
		fy += fUnitY;
	}
	m_TextureList += handle;
	return 0;
}

// w3danispr.cpp:403
int W3dAniSpr::LoadTexture(int handle, float fSprSizeX, float fSprSizeY) {
	float fWidth = (float)GetResrcManager()->GetTextureWidth(handle);
	float fHeight = (float)GetResrcManager()->GetTextureHeight(handle);
	int nx = (int)(fWidth / fSprSizeX);
	int ny = (int)(fHeight / fSprSizeY);
	float fUnitX = 1.0f / (fWidth / fSprSizeX);
	float fUnitY = 1.0f / (fHeight / fSprSizeY);
	float fx = 0.0f;
	float fy = 0.0f;
	for (int y = 0; y < ny; ++y) {
		for (int x = 0; x < nx; ++x) {
			_WSPRITE* pSprite = new _WSPRITE;
			pSprite->hTexture = handle;
			pSprite->fStartU = fy;
			pSprite->fStartV = fx;
			pSprite->fEndU = fy + fUnitY;
			pSprite->fEndV = fx + fUnitX;
			AddSprite(pSprite);
			fx += fUnitX;
		}
		fx = 0.0f;
		fy += fUnitY;
	}
	return 1;
}

// w3danispr.cpp:440
void W3dAniSpr::AddSprite(_WSPRITE* pSprite) {
	m_SpriteList += pSprite;
	m_nTotalSprite++;
}

// w3danispr.cpp:446
void W3dAniSpr::DelSprite(_WSPRITE* pSprite) {
	if (pSprite) {

		m_SpriteList -= pSprite;
		delete pSprite;
		m_nTotalSprite--;
	}
}

// w3danispr.cpp:456
void W3dAniSpr::AllDelSprite(void) {
	_WSPRITE* pSprite;

	while ((pSprite = m_SpriteList.Start()) != NULL) {

		DelSprite(pSprite);
	}
	m_SpriteList.Reset();
}

// w3danispr.cpp:467
_WSPRITE* W3dAniSpr::FindSprite(int nSprNum) {
	_WSPRITE* pSprite = m_SpriteList.Start();

	for (int i = 0; i < nSprNum && pSprite; i++)
		pSprite = m_SpriteList.Next();




	return pSprite;
}
