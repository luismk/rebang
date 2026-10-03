#pragma once

#include <vector>

extern WResourceManager* g_resrcmng;

class CSea
{
public:
	CSea();
	~CSea();

	struct sLayer
	{
		WVector2D speed;

		unsigned long color;
		WVector2D* pUV;
		WTVertex** ppVertex;
		WTVertex* pVertex;
		int texHandle;
		sLayer()
		{
			pUV = NULL;
			ppVertex = NULL;
			pVertex = NULL;
			texHandle = 0;
		}

		~sLayer()
		{
			if (pUV)
			{
				delete[] pUV;
				pUV = NULL;
			}
			if (ppVertex)
			{
				delete[] ppVertex;
				ppVertex = NULL;
			}
			if (pVertex)
			{
				delete[] pVertex;
				pVertex = NULL;
			}
			if (g_resrcmng && texHandle)
			{
				g_resrcmng->Release(texHandle);
				texHandle = 0;
			}
		}
	};

	void SetArea(const std::vector<WVector>& area);
	void LoadTexture();
	void Process(float delta);
	void Render();

protected:
	void Init();
	void AddLayer(float speedU, float speedV, float scale, unsigned char alpha);

	Waabb m_area;
	WVector* m_pPoint;
	int m_pointNum;
	sLayer* m_pLayer;
	int m_layerNum;
	unsigned char m_reserved[0x34];
	float m_scale;
};
