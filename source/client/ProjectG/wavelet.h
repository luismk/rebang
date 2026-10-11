// Original: d:\build\custom\temp\code\client\projectg\wavelet.h
#ifndef PG_CLIENT_PROJECTG_WAVELET_H
#define PG_CLIENT_PROJECTG_WAVELET_H

#include <vector>
#include "..\..\defines.h"
#include "wmath.h"
#include "..\..\padding.h"

struct WTVertex;

class CWavelet {
	LAYOUT_CHECK_FRIEND;
public:
	struct sWavelet {
		WVector pos[2];   // 0x0
		WVector dir;      // 0x18
		float length[2];  // 0x24
	};

	// One expanding ring of the wavelet (Process animates it).
	struct sLayer {
		float m_offset;   // 0x0, distance travelled (+= m_speed * elapsed)
		float m_delay;    // 0x4, time left before the ring restarts (state 2)
		float m_alpha;    // 0x8, 0..255
		float m_time;     // 0xc, time since the ring started
		float m_speed;    // 0x10, slowed by CWavelet::m_deceleration
		int m_state;      // 0x14, 0 fading in, 1 fading out, 2 waiting
	};

	CWavelet(void);
	virtual ~CWavelet(void);
	virtual void Process(float elapsed);
	virtual void Render(void);

	void SetWavelet(const std::vector<WVector>& points, const char* attribute, int layer);

protected:
	virtual void ReadAttribute(const char* attribute);
	virtual void SetLayer(int count);

	static WTVertex vtr[4];           // the quad Render draws each ring with
	static WTVertex* vtx[5];          // the quad's vertices in fan order, NULL-terminated

	std::vector<sWavelet> m_wavelets; // 0x4
	int m_texture;                    // 0x14
	float m_range;                    // 0x18, distance a ring travels (16 by default)
	float m_freq;                     // 0x1C, sWave::m_rate of the attribute's wave
	float m_deceleration;             // 0x20, m_range / (4t - t*t/2) for the fade-in time t
	float m_fadeInTime;               // 0x24, alpha rises 255 per this time
	float m_fadeOutTime;              // 0x28, alpha falls 255 per this time
	float m_restartDelay;             // 0x2C, delay given to the next ring when one fades out
	std::vector<sLayer> m_layers;     // 0x30
};
CHECK_SIZE(CWavelet::sLayer, 0x18);
CHECK_FIELD(CWavelet::sLayer, m_offset, 0x0);
CHECK_FIELD(CWavelet::sLayer, m_delay, 0x4);
CHECK_FIELD(CWavelet::sLayer, m_alpha, 0x8);
CHECK_FIELD(CWavelet::sLayer, m_time, 0xc);
CHECK_FIELD(CWavelet::sLayer, m_speed, 0x10);
CHECK_FIELD(CWavelet::sLayer, m_state, 0x14);
CHECK_SIZE(CWavelet::sWavelet, 0x2C);
CHECK_FIELD(CWavelet::sWavelet, pos, 0x0);
CHECK_FIELD(CWavelet::sWavelet, dir, 0x18);
CHECK_FIELD(CWavelet::sWavelet, length, 0x24);
BEGIN_LAYOUT_CHECK(CWavelet)
	CHECK_FIELD(CWavelet, m_wavelets, 0x4);
	CHECK_FIELD(CWavelet, m_texture, 0x14);
	CHECK_FIELD(CWavelet, m_range, 0x18);
	CHECK_FIELD(CWavelet, m_freq, 0x1C);
	CHECK_FIELD(CWavelet, m_deceleration, 0x20);
	CHECK_FIELD(CWavelet, m_fadeInTime, 0x24);
	CHECK_FIELD(CWavelet, m_fadeOutTime, 0x28);
	CHECK_FIELD(CWavelet, m_restartDelay, 0x2C);
	CHECK_FIELD(CWavelet, m_layers, 0x30);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_WAVELET_H
