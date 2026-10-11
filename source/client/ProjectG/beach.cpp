#include "minatl.h"
#include "projectg.h"
#include "golfdoc.h"
#include "quadtree.h"
#include "beach.h"

extern LightSet g_lightset;

void CBeach::ReadAttribute(const char* name)
{
	if (!name)
		return;

	int index;
	if (strchr(name, '_') && sscanf(strchr(name, '_') + 1, "%d", &index))
	{
		m_texture = g_resrcmng->LoadTexture(
			GOLFDOC()->m_waveInfo.GetTexture(index), 0, 0, 0);
		m_range = GOLFDOC()->m_waveInfo.GetWidth(index);
		m_freq = GOLFDOC()->m_waveInfo.GetFreq(index);
	}
	else
	{
		m_texture = g_resrcmng->LoadTexture("[water.dds", 0, 0, 0);
		m_range = 76.8f;
		m_freq = 7.5f;
	}

	m_fadeInTime = m_freq * (4.0f / 15.0f);
	m_fadeOutTime = m_freq * (8.0f / 15.0f);
	m_restartDelay = m_freq * 0.2f;
	m_deceleration =
		m_range / (4.0f * m_fadeInTime - m_fadeInTime * m_fadeInTime * 0.5f);
}

void CBeach::Process(float dt)
{
	for (unsigned char i = 0; i < m_layers.size(); i++)
	{
		m_layers[i].m_time += dt;

		switch (m_layers[i].m_state)
		{
		case 0:
			m_layers[i].m_alpha += dt * (255.0f / m_fadeInTime);
			m_layers[i].m_speed -= dt * m_deceleration;
			m_layers[i].m_offset += dt * m_layers[i].m_speed;
			if (m_layers[i].m_alpha > 255.0f)
			{
				m_layers[i].m_alpha = 255.0f;
				m_layers[i].m_state = 1;
				m_layers[(i + 1) % m_layers.size()].m_delay = m_restartDelay;
			}
			break;

		case 1:
			m_layers[i].m_alpha -= dt * (255.0f / m_fadeOutTime);
			m_layers[i].m_speed -= dt * m_deceleration;
			m_layers[i].m_offset += dt * m_layers[i].m_speed;
			if (m_layers[i].m_alpha < 0.0f)
				m_layers[i].m_state = 2;
			break;

		case 2:
			if (m_layers[i].m_delay > 0.0f)
			{
				m_layers[i].m_delay -= dt;
				if (m_layers[i].m_delay < 0.0f)
				{
					m_layers[i].m_state = 0;
					m_layers[i].m_alpha = 0.0f;
					m_layers[i].m_time = 0.0f;
					m_layers[i].m_offset = 0.0f;
					m_layers[i].m_speed = m_deceleration * 4.0f;
				}
			}
			break;
		}
	}
}

void CBeach::Render()
{
	float maxTime = 0.0f;
	unsigned char start = 0;

	for (unsigned char i = 0; i < m_layers.size(); i++)
	{
		if (maxTime < m_layers[i].m_time)
		{
			start = i;
			maxTime = m_layers[i].m_time;
		}
	}

	BYTE g = 255 - (BYTE)(((255 - (g_lightset.ambient2 >> 8)) & 0xff) * 0.8f);
	BYTE r = 255 - (BYTE)(((255 - (g_lightset.ambient2 >> 16)) & 0xff) * 0.8f);
	BYTE b = 255 - (BYTE)(((255 - g_lightset.ambient2) & 0xff) * 0.8f);
	unsigned long color = (r << 16) | (g << 8) | b;

	for (unsigned char j = 0; j < m_layers.size(); j++)
	{
		if (m_layers[(j + start) % m_layers.size()].m_state == 2)
			continue;

		for (unsigned char k = 0; k < m_wavelets.size() - 1; k++)
		{
			unsigned char n;
			int m;
			for (n = 0, m = 0; n < 4; n++, m++)
			{
				unsigned char w = k + ((n & 2) ? 1 : 0);
				bool bEdge = (w == 0 || w == m_wavelets.size() - 1);
				bool bFar = (n & 1) != 0;
				float scale = bFar ? 0.1f : (bEdge ? 0.5f : 1.0f);

				WVector pos = m_wavelets[w].pos[m & 1] -
					scale * m_layers[(j + start) % m_layers.size()].m_offset *
						m_wavelets[w].dir;
				WVector ground;
				ground = GetPVS().GetGroundPoint(pos, true, NULL, true);
				ground.y += bFar ? 1.5f : 1.0f;

				vtr[n].SetPosition(ground);
				vtr[n].tu = m_wavelets[w].length[m & 1];
				if (bEdge)
					vtr[n].diffuse = color | 0xff000000;
				else
					vtr[n].diffuse = color |
						((int)m_layers[(j + start) % m_layers.size()].m_alpha
							<< 24);
			}

			g_view->DrawPolygonFan(vtx, (m_texture & 0x7ff) | 0x28400000,
				0x400, false);
		}
	}
}
