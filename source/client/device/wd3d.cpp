#include <string>
#include "wd3d.h"
#include <cmath>
#include <wutil.h>
#include <algorithm>
WDirect3D::WDirect3D()
{
	m_mainThreadId = 0;
	m_chVertex = 0;
	m_dwOffset = 0;
	m_bufPolySwNum = m_bufPolyHwNum = m_bufSortNum = 0;
	m_texCount1 = 128;
	m_texCount2 = 1;
	m_useHiQualityTex = false;
	for (unsigned i = 0; i < 2048; i++)
		m_texList[i] = TS_VOID;
	m_MaxTextureBlendStages = 0;
	ApplyGamma(1.3f);
	m_fog = false;
	m_fogColor = 0;
	m_fogEnd = m_fogStart = 0;
	m_xiVbCount = m_xiIbCount = 1;
	memset(m_xahVbs, 0, sizeof(m_xahVbs));
	memset(m_xahIbs, 0, sizeof(m_xahIbs));
	m_xahVbs[0] = m_xahIbs[0] = 1;
}

WDirect3D::~WDirect3D()
{
	this->WDirect3D::Release();
}

void WDirect3D::Init()
{
	this->m_chVertex = new char[0x100000];
	this->m_devState = W_VDEVSTATE_NORMAL;
	this->m_buf4sort.resize(0x1200);
	this->m_bufPolySw.resize(0x1000);
	this->m_bufPolyHw.resize(0x200);
}

void WDirect3D::Release()
{
	for (unsigned int i = 0; i < 0x800; i++)
	{
		if (this->m_texList[i])
		{
			this->DestroyTexture(i);
			this->m_texList[i] = TS_VOID;
		}
	}
	for (unsigned int i = 1; i < 0x400; i++)
	{
		if (this->m_xahVbs[i])
		{
			this->xReleaseVertexBuffer(i);
			this->m_xahVbs[i] = 0;
		}
	}
	for (int i = 1; i < 0x100; i++)
	{
		if (this->m_xahIbs[i])
		{
			this->xReleaseIndexBuffer(i);
			this->m_xahIbs[i] = 0;
		}
	}
	if (this->m_chVertex)
	{
		delete[] this->m_chVertex;
		this->m_chVertex = 0;
	}
}

void* WDirect3D::ModifyVertices(WTVertex** vl, int n,
	unsigned long dwVertexTypeDesc, bool store)
{
	unsigned long dwOffset = this->m_dwOffset;
	if ((int)dwOffset >= 0xCCCCC)
	{
		this->Flush(0);
		dwOffset = 0;
	}

	char* result = &this->m_chVertex[dwOffset];
	switch (dwVertexTypeDesc)
	{
	case 0x00000000:
		for (int i = 0; i < n; dwOffset += 24, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 16);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i]->lv, 8);
		}
		break;
	case 0x00000040:
		for (int i = 0; i < n; dwOffset += 28, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 20);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i]->lv, 8);
		}
		break;
	case 0x00000042:
		for (int i = 0; i < n; dwOffset += 16, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i]->diffuse, 4);
		}
		break;
	case 0x00000044:
		for (int i = 0; i < n; dwOffset += 20, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 20);
		}
		break;
	case 0x00000102:
		for (int i = 0; i < n; dwOffset += 20, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i]->tu, 8);
		}
		break;
	case 0x00000104:
		for (int i = 0; i < n; dwOffset += 24, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 16);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i]->tu, 8);
		}
		break;
	case 0x00000142:
		for (int i = 0; i < n; dwOffset += 24, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i]->diffuse, 4);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i]->tu, 8);
		}
		break;
	case 0x00000144:
		for (int i = 0; i < n; dwOffset += 28, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 20);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i]->tu, 8);
		}
		break;
	case 0x00000202:
		for (int i = 0; i < n; dwOffset += 28, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i]->tu, 16);
		}
		break;
	case 0x00000204:
		for (int i = 0; i < n; dwOffset += 32, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 16);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i]->tu, 16);
		}
		break;
	case 0x00000242:
		for (int i = 0; i < n; dwOffset += 32, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i]->diffuse, 4);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i]->tu, 16);
		}
		break;
	case 0x00000244:
		for (int i = 0; i < n; dwOffset += 36, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 20);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i]->tu, 16);
		}
		break;
	case 0x80000080:
		for (int i = 0; i < n; dwOffset += 28, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 16);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i]->lv, 8);
			this->m_chVertex[dwOffset + 19] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x800000C2:
		for (int i = 0; i < n; dwOffset += 20, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i]->diffuse, 4);
		}
		break;
	case 0x800000C4:
		for (int i = 0; i < n; dwOffset += 24, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 20);
			this->m_chVertex[dwOffset + 23] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x80000182:
		for (int i = 0; i < n; dwOffset += 24, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 12);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i]->tu, 8);
		}
		break;
	case 0x80000184:
		for (int i = 0; i < n; dwOffset += 28, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 16);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i]->tu, 8);
			this->m_chVertex[dwOffset + 19] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x800001C2:
		for (int i = 0; i < n; dwOffset += 28, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i]->diffuse, 4);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i]->tu, 8);
		}
		break;
	case 0x800001C4:
		for (int i = 0; i < n; dwOffset += 32, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 20);
			memcpy(&this->m_chVertex[dwOffset + 24], &vl[i]->tu, 8);
			this->m_chVertex[dwOffset + 23] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x80000282:
		for (int i = 0; i < n; dwOffset += 32, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 12);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i]->tu, 16);
		}
		break;
	case 0x80000284:
		for (int i = 0; i < n; dwOffset += 36, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 16);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i]->tu, 16);
			this->m_chVertex[dwOffset + 19] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x800002C2:
		for (int i = 0; i < n; dwOffset += 36, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i]->diffuse, 4);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i]->tu, 16);
			this->m_chVertex[dwOffset + 19] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x800002C4:
		for (int i = 0; i < n; dwOffset += 40, i++)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i]->x, 20);
			memcpy(&this->m_chVertex[dwOffset + 24], &vl[i]->tu, 16);
			this->m_chVertex[dwOffset + 23] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	default:
		break;
	}

	if (store)
	{
		this->m_dwOffset = dwOffset;
	}

	return result;
}

void* WDirect3D::ModifyVertices(WTVertex* vl, int n,
	unsigned long dwVertexTypeDesc, bool store)
{
	unsigned long dwOffset = this->m_dwOffset;
	if ((int)dwOffset >= 0xCCCCC)
	{
		this->Flush(0);
		dwOffset = 0;
	}

	char* result = &this->m_chVertex[dwOffset];
	switch (dwVertexTypeDesc)
	{
	case 0x00000000:
		for (int i = 0; i < n; i++, dwOffset += 24)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 16);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i].lv, 8);
		}
		break;
	case 0x00000040:
		for (int i = 0; i < n; i++, dwOffset += 28)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 20);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i].lv, 8);
		}
		break;
	case 0x00000042:
		for (int i = 0; i < n; i++, dwOffset += 16)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i].diffuse, 4);
		}
		break;
	case 0x00000044:
		for (int i = 0; i < n; i++, dwOffset += 20)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 20);
		}
		break;
	case 0x00000102:
		for (int i = 0; i < n; i++, dwOffset += 20)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i].tu, 8);
		}
		break;
	case 0x00000104:
		for (int i = 0; i < n; i++, dwOffset += 24)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 16);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i].tu, 8);
		}
		break;
	case 0x00000142:
		for (int i = 0; i < n; i++, dwOffset += 24)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i].diffuse, 4);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i].tu, 8);
		}
		break;
	case 0x00000144:
		for (int i = 0; i < n; i++, dwOffset += 28)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 20);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i].tu, 8);
		}
		break;
	case 0x00000202:
		for (int i = 0; i < n; i++, dwOffset += 28)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i].tu, 16);
		}
		break;
	case 0x00000204:
		for (int i = 0; i < n; i++, dwOffset += 32)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 16);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i].tu, 16);
		}
		break;
	case 0x00000242:
		for (int i = 0; i < n; i++, dwOffset += 32)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i].diffuse, 4);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i].tu, 16);
		}
		break;
	case 0x00000244:
		for (int i = 0; i < n; i++, dwOffset += 36)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 20);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i].tu, 16);
		}
		break;
	case 0x80000080:
		for (int i = 0; i < n; i++, dwOffset += 28)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 16);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i].lv, 8);
			this->m_chVertex[dwOffset + 19] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x800000C2:
		for (int i = 0; i < n; i++, dwOffset += 20)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i].diffuse, 4);
		}
		break;
	case 0x800000C4:
		for (int i = 0; i < n; i++, dwOffset += 24)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 20);
			this->m_chVertex[dwOffset + 23] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x80000182:
		for (int i = 0; i < n; i++, dwOffset += 24)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 12);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i].tu, 8);
		}
		break;
	case 0x80000184:
		for (int i = 0; i < n; i++, dwOffset += 28)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 16);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i].tu, 8);
			this->m_chVertex[dwOffset + 19] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x800001C2:
		for (int i = 0; i < n; i++, dwOffset += 28)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i].diffuse, 4);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i].tu, 8);
		}
		break;
	case 0x800001C4:
		for (int i = 0; i < n; i++, dwOffset += 32)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 20);
			memcpy(&this->m_chVertex[dwOffset + 24], &vl[i].tu, 8);
			this->m_chVertex[dwOffset + 23] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x80000282:
		for (int i = 0; i < n; i++, dwOffset += 32)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 12);
			memcpy(&this->m_chVertex[dwOffset + 16], &vl[i].tu, 16);
		}
		break;
	case 0x80000284:
		for (int i = 0; i < n; i++, dwOffset += 36)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 16);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i].tu, 16);
			this->m_chVertex[dwOffset + 19] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	case 0x800002C2:
		for (int i = 0; i < n; i++, dwOffset += 36)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 12);
			memcpy(&this->m_chVertex[dwOffset + 12], &vl[i].diffuse, 4);
			memcpy(&this->m_chVertex[dwOffset + 20], &vl[i].tu, 16);
		}
		break;
	case 0x800002C4:
		for (int i = 0; i < n; i++, dwOffset += 40)
		{
			memcpy(&this->m_chVertex[dwOffset + 0], &vl[i].x, 20);
			memcpy(&this->m_chVertex[dwOffset + 24], &vl[i].tu, 16);
			this->m_chVertex[dwOffset + 23] =
				this->CalcFog(*(float*)&m_chVertex[dwOffset + 8]);
		}
		break;
	default:
		break;
	}

	if (store)
	{
		this->m_dwOffset = dwOffset;
	}

	return result;
}

void WDirect3D::ApplyGamma(float gamma)
{
	for (int i = 0; i < 256; i++)
	{
		this->m_iRefTable[i] = Between<int>(0,
			static_cast<int>(
				pow(static_cast<double>(i) * 0.00390625, 1.0 / gamma) * 256.0),
			255);
	}
}

bool WDirect3D::SetFogEnable(bool enable)
{
	bool old = m_fog;
	m_fog = enable;
	return old;
}

void WDirect3D::SetFogState(float fogStart, float fogEnd, unsigned long color)
{
	this->m_fogStart = fogStart;
	this->m_fogEnd = fogEnd;
	this->m_fogColor = color & 0xFFFFFF;
}

unsigned char WDirect3D::CalcFog(float depth)
{
	if (this->m_clip_near_scale == 0.0f)
	{
		this->m_clip_near_scale = 0.1f;
	}
	float x = this->m_clip_scale_z - depth;
	if (x == 0.0f)
	{
		x = 0.0000001f;
	}
	float d = this->m_clip_near_scale / static_cast<float>(x);
	if (d > this->m_fogEnd)
	{
		return 0;
	}
	if (d <= this->m_fogStart)
		return 255;
	{
		return static_cast<unsigned char>(
			static_cast<int>((this->m_fogEnd - d) * 255.0f /
				(this->m_fogEnd - this->m_fogStart)));
	}
	return -1;
}

float WDirect3D::CalcDepth(WTVertex** p, int num)
{
	float n = 0.0f;
	for (int i = 0; i < num; i++)
	{
		n += p[i]->vz;
	}
	return static_cast<float>(n / static_cast<float>(num));
}

void WDirect3D::DrawPolygonFan(WTVertex** p, int iType, int iNum, int iType2,
	unsigned long dwVertexTypeDesc)
{
	WTVertex** vertexList;
	void* lpvVertices;
	WTVertex* t[64];
	float pa;
	D3DPRIMITIVETYPE dptPrimitiveType;

	if (this->m_devState == W_VDEVSTATE_LOST)
	{
		return;
	}

	if (iType & 0x4000000)
	{
		if (iNum == 2)
		{
			dptPrimitiveType = D3DPT_LINESTRIP;
			vertexList = p;
		}
		else
		{
			int i;
			for (i = 0; i < iNum; i++)
				t[i] = p[i];
			t[i] = p[0];
			vertexList = t;
			++iNum;
			dptPrimitiveType = D3DPT_LINESTRIP;
		}
	}
	else
	{
		dptPrimitiveType = D3DPT_TRIANGLEFAN;
		vertexList = p;
	}
	dwVertexTypeDesc |=
		((iType & 0x3F800) ? 0x200 : ((iType & 0x7FF) ? 0x100 : 0)) |
		(~((unsigned long)iType >> 24) & 0x40);

	if (iType & 0x8000000)
	{
		if (this->m_fog)
		{
			dwVertexTypeDesc |= 0x80000080;
		}
		else
		{
			iType &= ~0x8000000;
		}
	}
	if (iType & 0x400000)
	{
		if ((iType & 0x300000) == 0x300000 || (iType & 0x300000) == 0x100000)
		{
			pa = 0.0;
		}
		else
		{
			pa = this->CalcDepth(vertexList, iNum);
		}
		this->Buffering(iType, iType2, iNum, dwVertexTypeDesc,
			this->ModifyVertices(vertexList, iNum, dwVertexTypeDesc, true),
			dptPrimitiveType, pa);
	}
	else if (iType & 0x3F800 && this->m_MaxTextureBlendStages == 1)
	{
		if (!(dwVertexTypeDesc & 0x80000000))
		{
			if (iType & 0x7FF)
			{
				if (dwVertexTypeDesc & 0x40)
				{
					this->DrawPrimitive(iType & 0xFFF807FF, iNum, 324u,
						this->ModifyVertices(vertexList, iNum, 0x144, false),
						dptPrimitiveType, iType2);
				}
				else
				{
					this->DrawPrimitive(iType & 0xFFF807FF, iNum, 260u,
						this->ModifyVertices(vertexList, iNum, 0x104u, false),
						dptPrimitiveType, iType2);
				}
				if (iType & 0x40000)
				{
					if (dwVertexTypeDesc & 0x40)
					{
						this->Buffering((iType >> 11 & 0x7F) | 0x20900000, 0,
							iNum, 0x104u,
							this->ModifyVertices(vertexList, iNum, 0, true),
							dptPrimitiveType, 0.0);
					}
					else
					{
						this->Buffering((iType >> 11 & 0x7F) | 0x60900000, 0,
							iNum, 0x144u,
							this->ModifyVertices(vertexList, iNum, 0x40u, true),
							dptPrimitiveType, 0.0);
					}
				}
				else
				{
					this->Buffering((iType >> 11 & 0x7F) | 0x61100000, 0, iNum,
						0x104u, this->ModifyVertices(vertexList, iNum, 0, true),
						dptPrimitiveType, 0.0);
				}
			}
			else
			{
				if (dwVertexTypeDesc & 0x40)
				{
					this->DrawPrimitive(iType >> 11 & 0x7F, iNum, 260u,
						this->ModifyVertices(vertexList, iNum, 0, true),
						dptPrimitiveType, iType2);
				}
				else
				{
					this->DrawPrimitive((iType >> 11 & 0x7F) | 0x40000000, iNum,
						0x144u,
						this->ModifyVertices(vertexList, iNum, 0x40u, true),
						dptPrimitiveType, iType2);
				}
			}
		}
		else
		{
			if (iType & 0x3FFFF)
			{
				this->DrawPrimitive(static_cast<int>(iType & 0xFFFC07FF), iNum,
					388u,
					this->ModifyVertices(vertexList, iNum, 0x80000184, false),
					dptPrimitiveType, iType2);
			}
			lpvVertices =
				this->ModifyVertices(vertexList, iNum, 0x80000080, true);
			int i;
			for (i = 0; i < iNum; i++)
			{
				if (((unsigned char*)lpvVertices)[i * 28 + 19] != 0xFF)
				{
					this->Buffering((iType >> 11 & 0x7F) | 0x69100000, 0, iNum,
						0x184, lpvVertices, dptPrimitiveType, 0.0);
					break;
				}
			}
		}
	}
	else
	{
		this->DrawPrimitive(iType, iNum, dwVertexTypeDesc,
			this->ModifyVertices(vertexList, iNum, dwVertexTypeDesc, false),
			dptPrimitiveType, iType2);
	}
}

void WDirect3D::DrawIndexedTriangles(WTVertex* p, int pNum,
	unsigned short* fList, int fNum, int iType, int iType2)
{
	if (m_devState != W_VDEVSTATE_LOST)
	{
		unsigned long dwVertexTypeDesc =
			((iType & 0x3F800) ? 0x200 : ((iType & 0x7FF) ? 0x100 : 0)) |
			(~((unsigned long)iType >> 24) & 0x40) | 2;
		if (iType & 0x8000000)
		{
			if (m_fog)
				dwVertexTypeDesc |= 0x80000080;
			else
				iType &= ~0x8000000;
		}
		if (iType & 0x400000)
		{
			for (int i = 0; i < fNum;)
			{
				WTVertex* t[3];
				t[0] = &p[fList[i++]];
				t[1] = &p[fList[i++]];
				t[2] = &p[fList[i++]];
				float depth;
				if ((iType & 0x300000) == 0x300000 ||
					(iType & 0x300000) == 0x100000)
					depth = 0.0f;
				else
					depth = CalcDepth(t, 3);
				Buffering(iType, iType2, 3, dwVertexTypeDesc,
					ModifyVertices(t, 3, dwVertexTypeDesc, true),
					D3DPT_TRIANGLEFAN, depth);
			}
		}
		else if (!(iType & 0x4000000))
		{
			DrawPrimitiveIndexed(iType, dwVertexTypeDesc,
				ModifyVertices(p, pNum, dwVertexTypeDesc, false), pNum, fList,
				fNum, iType2);
		}
		else
		{
			for (int i = 0; i < fNum;)
			{
				WTVertex* t[4];
				t[0] = &p[fList[i++]];
				t[1] = &p[fList[i++]];
				t[2] = &p[fList[i++]];
				t[3] = t[0];
				DrawPrimitive(iType, 4, dwVertexTypeDesc,
					ModifyVertices(t, 4, dwVertexTypeDesc, false),
					D3DPT_LINESTRIP, iType2);
			}
		}
	}
}

void WDirect3D::DrawBuffered(const w_poly_buffer_sw& bufSw)
{
	this->ApplyCustomRenderState(bufSw.customRenderState);
	this->DrawPrimitive(bufSw.type, bufSw.num, bufSw.dwVertexTypeDesc,
		bufSw.lpvVertices, bufSw.dptPrimitiveType, bufSw.type2);
}

void WDirect3D::DrawBuffered(const w_poly_buffer_hw& bufHw)
{
	this->ApplyCustomRenderState(bufHw.customRenderState);
	this->xDrawIndexedPrimitive(bufHw.VState, bufHw.BState);
}

int __cdecl WDirect3D::ComparePolyDepth(const void* elem1, const void* elem2)
{
	const w_poly_buffer* fElem1 =
		*static_cast<const w_poly_buffer* const*>(elem1);
	const w_poly_buffer* fElem2 =
		*static_cast<const w_poly_buffer* const*>(elem2);

	if (fElem1->depth > fElem2->depth)
	{
		return -1;
	}

	if (fElem1->depth < fElem2->depth)
	{
		return 1;
	}

	return fElem1 >= fElem2 ? 1 : -1;
}

void WDirect3D::Flush(unsigned long flag)
{
	this->FlushRenderPrimitive();
	ClearVertices();
	if (this->m_devState != W_VDEVSTATE_LOST && this->m_bufSortNum > 0)
	{
		this->BeginUsingCustomRenderState();
		this->FlushEqual();
		this->FlushRenderPrimitive();
		qsort(&this->m_buf4sort[0], this->m_bufSortNum, 4, ComparePolyDepth);
		if (flag & 1)
		{
			this->FlushMultiPass(flag);
		}
		else
		{
			this->FlushOnePass(flag);
		}
		this->FlushRenderPrimitive();
		this->FlushAlways();
		this->FlushRenderPrimitive();
		this->EndUsingCustomRenderState();
	}
	this->ClearCustomRenderStateSnapShotList();

	if (m_bufPolySwNum == m_bufPolySw.size())
	{
		m_bufPolySw.clear();
		m_bufPolySw.resize((int)(m_bufPolySwNum * 1.5f));
	}
	if (m_bufPolyHwNum == m_bufPolyHw.size())
	{
		m_bufPolyHw.clear();
		m_bufPolyHw.resize(m_bufPolyHwNum * 1.5);
	}
	if (m_bufPolySw.size() + m_bufPolyHw.size() != m_buf4sort.size())
	{
		m_buf4sort.clear();
		m_buf4sort.resize(m_bufPolySw.size() + m_bufPolyHw.size());
	}
	this->m_bufSortNum = 0;
	this->m_bufPolyHwNum = 0;
	this->m_bufPolySwNum = 0;
}

void WDirect3D::FlushMultiPass(unsigned long flag)
{
	bool bLastAlphaTest = this->SetRenderState4Flushing(0);

	int i;
	for (i = this->m_bufSortNum - 1; i >= 0; --i)
	{
		if (!m_buf4sort[i]->bHw)
		{
			w_poly_buffer_sw* swPolyBuf = (w_poly_buffer_sw*)m_buf4sort[i];
			if (bLastAlphaTest)
			{
				swPolyBuf->type |= 0x80000000;
			}
			else
			{
				swPolyBuf->type &= ~0x80000000;
			}
			if (!(swPolyBuf->type & 0x01800000))
			{
				this->ApplyCustomRenderState(swPolyBuf->customRenderState);
				this->DrawPrimitive(swPolyBuf->type, swPolyBuf->num,
					swPolyBuf->dwVertexTypeDesc, swPolyBuf->lpvVertices,
					swPolyBuf->dptPrimitiveType, swPolyBuf->type2);
			}
		}
		else
		{
			w_poly_buffer_hw* hwPolyBuf = (w_poly_buffer_hw*)m_buf4sort[i];
			if (bLastAlphaTest)
			{
				hwPolyBuf->BState.xiFlag0 |= 0x80000000;
			}
			else
			{
				hwPolyBuf->BState.xiFlag0 &= ~0x80000000;
			}
			if (!(hwPolyBuf->BState.xiFlag0 & 0x01800000))
			{
				this->ApplyCustomRenderState(hwPolyBuf->customRenderState);
				this->xDrawIndexedPrimitive(hwPolyBuf->VState,
					hwPolyBuf->BState);
			}
		}
	}

	unsigned long lastRenderState = 0;
	this->ApplyCustomRenderState(0);
	this->FlushRenderPrimitive();
	this->SetRenderState4Flushing(1);

	int beginIdx, endIdx, increment, typeMask, typeToAdd;
	if (flag & 2)
	{
		beginIdx = this->m_bufSortNum - 1;
		typeMask = 0x20000000;
		endIdx = -1;
		typeToAdd = 0;
		increment = -1;
	}
	else
	{
		typeMask = 0;
		typeToAdd = 0x20000000;
		beginIdx = 0;
		endIdx = this->m_bufSortNum;
		increment = 1;
	}

	for (i = beginIdx; i != endIdx; i += increment)
	{
		if (!m_buf4sort[i]->bHw)
		{
			w_poly_buffer_sw* swPolyBuffer = (w_poly_buffer_sw*)m_buf4sort[i];
			if ((swPolyBuffer->type & 0x1800000) != lastRenderState)
			{
				if ((swPolyBuffer->type & 0x1800000) == 0)
				{
					this->SetRenderState4Flushing(1);
				}
				else
				{
					this->SetRenderState4Flushing(2);
				}
				lastRenderState = swPolyBuffer->type & 0x1800000;
			}
			swPolyBuffer->type = typeToAdd | (swPolyBuffer->type & ~typeMask);
			this->ApplyCustomRenderState(swPolyBuffer->customRenderState);
			this->DrawPrimitive(swPolyBuffer->type, swPolyBuffer->num,
				swPolyBuffer->dwVertexTypeDesc, swPolyBuffer->lpvVertices,
				swPolyBuffer->dptPrimitiveType, swPolyBuffer->type2);
		}
		else
		{
			w_poly_buffer_hw* hwPolyBuffer = (w_poly_buffer_hw*)m_buf4sort[i];
			if ((hwPolyBuffer->BState.xiFlag0 & 0x1800000) != lastRenderState)
			{
				if ((hwPolyBuffer->BState.xiFlag0 & 0x1800000) == 0)
				{
					this->SetRenderState4Flushing(1);
				}
				else
				{
					this->SetRenderState4Flushing(2);
				}
				lastRenderState = hwPolyBuffer->BState.xiFlag0 & 0x1800000;
			}
			hwPolyBuffer->BState.xiFlag0 =
				typeToAdd | (hwPolyBuffer->BState.xiFlag0 & ~typeMask);
			this->ApplyCustomRenderState(hwPolyBuffer->customRenderState);
			this->xDrawIndexedPrimitive(hwPolyBuffer->VState,
				hwPolyBuffer->BState);
		}
	}

	this->ApplyCustomRenderState(0);
	this->FlushRenderPrimitive();
	this->SetRenderState4Flushing(3);
}

void WDirect3D::FlushOnePass(unsigned long flag)
{
	int beginIdx, dir, n;
	int typeMask, typeToAdd;

	if (flag & 2)
	{
		beginIdx = this->m_bufSortNum - 1;
		typeMask = 0xA0000000;
		n = -1;
		typeToAdd = 0;
		dir = -1;
	}
	else
	{
		typeMask = 0x80000000;
		typeToAdd = 0x20000000;
		beginIdx = 0;
		n = this->m_bufSortNum;
		dir = 1;
	}

	for (int i = beginIdx; i != n; i += dir)
	{
		if (!m_buf4sort[i]->bHw)
		{
			w_poly_buffer_sw* swPolyBuffer = (w_poly_buffer_sw*)m_buf4sort[i];
			swPolyBuffer->type = typeToAdd | (swPolyBuffer->type & ~typeMask);
			DrawBuffered(*swPolyBuffer);
		}
		else
		{
			w_poly_buffer_hw* hwPolyBuffer = (w_poly_buffer_hw*)m_buf4sort[i];
			hwPolyBuffer->BState.xiFlag0 =
				typeToAdd | (hwPolyBuffer->BState.xiFlag0 & ~typeMask);
			DrawBuffered(*hwPolyBuffer);
		}
	}

	this->ApplyCustomRenderState(0);
	this->FlushRenderPrimitive();
}

void WDirect3D::FlushEqual()
{
	int i;
	for (i = 0; i < this->m_bufPolySwNum; i++)
	{
		w_poly_buffer_sw& swPolyBuf = this->m_bufPolySw[i];
		if ((swPolyBuf.type & 0x300000) == 0x100000)
		{
			this->ApplyCustomRenderState(swPolyBuf.customRenderState);
			this->DrawPrimitive(swPolyBuf.type, swPolyBuf.num,
				swPolyBuf.dwVertexTypeDesc, swPolyBuf.lpvVertices,
				swPolyBuf.dptPrimitiveType, swPolyBuf.type2);
		}
	}
	for (i = 0; i < this->m_bufPolyHwNum; i++)
	{
		w_poly_buffer_hw& hwPolyBuf = this->m_bufPolyHw[i];
		if ((hwPolyBuf.BState.xiFlag0 & 0x300000) == 0x100000)
		{
			this->ApplyCustomRenderState(hwPolyBuf.customRenderState);
			this->xDrawIndexedPrimitive(hwPolyBuf.VState, hwPolyBuf.BState);
		}
	}
	this->ApplyCustomRenderState(0);
}

void WDirect3D::FlushAlways()
{
	for (int i = 0; i < this->m_bufPolySwNum; i++)
	{
		w_poly_buffer_sw& swPolyBuf = this->m_bufPolySw[i];
		if ((swPolyBuf.type & 0x300000) == 0x300000)
		{
			this->ApplyCustomRenderState(swPolyBuf.customRenderState);
			this->DrawPrimitive(swPolyBuf.type, swPolyBuf.num,
				swPolyBuf.dwVertexTypeDesc, swPolyBuf.lpvVertices,
				swPolyBuf.dptPrimitiveType, swPolyBuf.type2);
		}
	}

	for (int i = 0; i < this->m_bufPolyHwNum; i++)
	{
		w_poly_buffer_hw& hwPolyBuf = this->m_bufPolyHw[i];
		if ((hwPolyBuf.BState.xiFlag0 & 0x300000) == 0x300000)
		{
			this->ApplyCustomRenderState(hwPolyBuf.customRenderState);
			this->xDrawIndexedPrimitive(hwPolyBuf.VState, hwPolyBuf.BState);
		}
	}

	this->ApplyCustomRenderState(0);
}

void WDirect3D::Buffering(int type, int type2, int num,
	unsigned long dwVertexTypeDesc, void* p, D3DPRIMITIVETYPE dptPrimitiveType,
	float depth)
{
	w_poly_buffer_sw* swPolyBuf = &this->m_bufPolySw[this->m_bufPolySwNum++];
	type2 &= ~0x1000;
	swPolyBuf->type2 = type2;
	swPolyBuf->num = num;
	swPolyBuf->bHw = false;
	swPolyBuf->type = type;
	swPolyBuf->dwVertexTypeDesc = dwVertexTypeDesc;
	swPolyBuf->lpvVertices = p;
	swPolyBuf->dptPrimitiveType = dptPrimitiveType;
	swPolyBuf->customRenderState = this->SnapShotCustomRenderState();

	if ((type & 0x300000) != 0x300000 && (type & 0x300000) != 0x100000)
	{
		swPolyBuf->depth = depth;
		this->m_buf4sort[this->m_bufSortNum++] = swPolyBuf;
	}

	int bufCapacity = this->m_bufPolySw.size();
	if (this->m_bufPolySwNum == bufCapacity)
	{
		this->Flush(1);
	}
}

void WDirect3D::Buffering(const WxViewState& viewState,
	const WxBatchState& batchState)
{
	w_poly_buffer_hw& hwPolyBuf = this->m_bufPolyHw[this->m_bufPolyHwNum++];
	hwPolyBuf.bHw = true;
	memcpy(&hwPolyBuf.VState, &viewState, sizeof hwPolyBuf.VState);
	memcpy(&hwPolyBuf.BState, &batchState, sizeof hwPolyBuf.BState);
	hwPolyBuf.customRenderState = this->SnapShotCustomRenderState();

	if ((batchState.xiFlag0 & 0x300000) != 0x300000 &&
		(batchState.xiFlag0 & 0x300000) != 0x100000)
	{
		hwPolyBuf.depth = batchState.xfDepth;
		this->m_buf4sort[this->m_bufSortNum++] = &hwPolyBuf;
	}

	if (this->m_bufPolyHwNum == static_cast<int>(this->m_bufPolyHw.size()))
	{
		this->Flush(1);
	}
}

int WDirect3D::GetTextureNum(int stage)
{
	if (stage)
	{
		for (int i = 0; this->m_texList[this->m_texCount2] && i < 127; ++i)
		{
			this->m_texCount2 =
				this->m_texCount2 >= 127 ? 1 : this->m_texCount2 + 1;
		}
		this->m_texList[this->m_texCount2] = TS_CREATED;
		return this->m_texCount2;
	}
	for (int i = 0; this->m_texList[this->m_texCount1] && i < 1920; ++i)
	{
		this->m_texCount1 =
			this->m_texCount1 >= 2047 ? 128 : this->m_texCount1 + 1;
	}
	this->m_texList[this->m_texCount1] = TS_CREATED;
	return this->m_texCount1;
}

int WDirect3D::UploadCompressedTexture(void* pSrc, size_t srcSize, int type)
{
	int result = this->UploadCompressedTextureSurface(pSrc, srcSize, type);
	this->m_texList[result] = TS_UPDATED;
	return result;
}

int WDirect3D::CreateTexture(LPBITMAPINFO src, int type)
{
	int texNum = GetTextureNum((static_cast<unsigned int>(type) >> 29) & 1);
	if (!(type & 0x1000))
	{
		CreateTextureSurface(src->bmiHeader.biWidth, src->bmiHeader.biHeight,
			((type & 0x40000000) | 0x20000000U) >> 29, type & 0x1e000, texNum,
			src->bmiHeader.biBitCount >= 32
				? 2
				: (static_cast<unsigned int>(type) >> 11) & 1,
			m_useHiQualityTex || (type & 0x80000000) ? 24 : 16);
		m_texList[texNum] = TS_UPDATED;
	}
	return texNum;
}

void WDirect3D::UpdateTexture(int texHandle, LPBITMAPINFO src, void* data,
	unsigned long type)
{
	if (this->m_texList[texHandle] == 1)
	{
		this->CreateTextureSurface(src->bmiHeader.biWidth,
			src->bmiHeader.biHeight, 1, 0, texHandle, 0, 16);
		this->m_texList[texHandle] = TS_UPDATED;
	}
	this->UpdateTextureSurface(texHandle, src, data, type);
}

void WDirect3D::DestroyTexture(int texHandle)
{
	this->m_texList[texHandle] = TS_VOID;
}

int WDirect3D::xCreateVertexBuffer(int numVertices, unsigned long dwFvf,
	unsigned long dwUsage)
{
	int hVb = this->xGetVbHandle();
	this->xCreateVertexBuffer(hVb, numVertices, dwFvf, dwUsage);
	return hVb;
}

int WDirect3D::xCreateIndexBuffer(int numIndices, unsigned long dwUsage)
{
	int hIb = this->xGetIbHandle();
	this->xCreateIndexBuffer(hIb, numIndices, dwUsage);
	return hIb;
}

int WDirect3D::xGetVbHandle()
{
	for (unsigned int i = 1; this->m_xahVbs[this->m_xiVbCount] && i < 0x3ff;
		++i)
	{
		this->m_xiVbCount =
			static_cast<unsigned int>(this->m_xiVbCount) >= 0x400
			? 1
			: this->m_xiVbCount + 1;
	}
	this->m_xahVbs[this->m_xiVbCount] = 1;
	return this->m_xiVbCount;
}

int WDirect3D::xGetIbHandle()
{
	for (int i = 1; this->m_xahIbs[this->m_xiIbCount] && i < 0xff; ++i)
	{
		this->m_xiIbCount =
			this->m_xiIbCount >= 0x100 ? 1 : this->m_xiIbCount + 1;
	}
	this->m_xahIbs[this->m_xiIbCount] = 1;
	return this->m_xiIbCount;
}

void WDirect3D::xReleaseVertexBuffer(int hVb)
{
	this->m_xahVbs[hVb] = 0;
}

void WDirect3D::xReleaseIndexBuffer(int hIb)
{
	this->m_xahIbs[hIb] = 0;
}

void WDirect3D::xDrawIndexedTriangles(const WxViewState& viewState,
	const WxBatchState& batchState)
{
	if (this->m_devState == W_VDEVSTATE_LOST)
	{
		return;
	}
	if (batchState.xiFlag0 & 0x400000)
	{
		this->Buffering(viewState, batchState);
	}
	else
	{
		this->xDrawIndexedPrimitive(viewState, batchState);
	}
}
