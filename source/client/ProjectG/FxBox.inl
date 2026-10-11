// Original: d:\build\custom\temp\code\client\projectg\fxbox.inl
// Inline members of CFxBox, included at the end of FxBox.h.

#include <math.h>
#include <stdlib.h>

#include "fxspray.h"

// CFxSpray is complete here: the second pass of fxsequence.inl (SetActive).
#define PG_FXSEQUENCE_INL_WITH_SPRAY
#include "fxsequence.inl"

// fxbox.inl:9
inline std::map<CFxSequence*, CFxSequence*>& CFxBox::GetSequenceList(void) {
	return m_sequences;
}

// fxbox.inl:15
inline std::map<CFxSpray*, CFxSpray*>& CFxBox::GetSprayList(void) {
	return m_sprays;
}

// fxbox.inl:23
inline CFxSpray* CFxBox::GetSpray(int index) {
	if (index < 0) {
		return 0;
	}
	for (std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.begin(); it != m_sprays.end(); ++it, --index) {
		if (index == 0) {
			return it->second;
		}
	}
	return 0;
}

// fxbox.inl:39
inline bool CFxBox::FindSequence(CFxSequence* pSequence) {
	return m_sequences.find(pSequence) != m_sequences.end();
}

// fxbox.inl:45
inline bool CFxBox::FindSpray(CFxSpray* pSpray) {
	return m_sprays.find(pSpray) != m_sprays.end();
}

// fxbox.inl:53
inline bool CFxBox::GetOutput(WVector& pos, unsigned char& alpha) {
	for (std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.begin(); it != m_sprays.end(); ++it) {
		CFxSpray* pSpray = it->second;

		WVector delta = pos - pSpray->m_outputPos;
		float scale = 1.0f / (delta.Magnitude() + 1.0f);
		pos += pSpray->m_outputDir * scale;
		int sum = pSpray->m_outputAlpha + alpha;
		alpha = sum > 0xFF ? 0xFF : sum;
	}
	return m_sprays.size() != 0;
}

// fxbox.inl:70
// A random number in [min, max], in 20000 steps.
inline float CFxBox::Random(float min, float max) {
	return (max - min) * (rand() % 20001) * 0.00005f + min;
}

// fxbox.inl:77
// A random point of the box [min, max] (z drawn first).
inline WVector CFxBox::Random(const WVector& min, const WVector& max) {
	return WVector(Random(min.x, max.x), Random(min.y, max.y), Random(min.z, max.z));
}

// fxbox.inl:84
// A random point around center: a random direction within the cone of
// angle about +y (a uniform cap of the unit sphere), at a random distance
// in [minDist, maxDist].
inline WVector CFxBox::Random(const WVector& center, float minDist, float maxDist, float angle) {
	float cosHalf = cosf(angle * 0.5f);
	float capHeight = 1.0f - cosHalf;
	float x, z, y, len;
	do {
		x = (rand() % 20001) * 0.0001f - 1.0f;
		z = (rand() % 20001) * 0.0001f - 1.0f;
		y = (rand() % 20001) * capHeight * 0.00005f + cosHalf;

		len = sqrtf(z * z + x * x);
	} while (fabsf(len) < 0.00001f);

	float scale = sqrtf(1.0f - y * y) / len;
	x *= scale;
	z *= scale;

	float dist = Random(minDist, maxDist);
	return WVector(x * dist + center.x, y * dist + center.y, z * dist + center.z);
}

// fxbox.inl:115
inline bool CFxBox::CheckGroundCollision(const WVector& from, const WVector& to, float& height, float radius) {
	if (!(to.y - height < 0.00001f && to.y - height > -0.00001f)) {
		if (to.y < height) {
			if ((from.y - height < 0.00001f && from.y - height > -0.00001f) || from.y < height) {
				float top = radius + to.y;
				return (top - height < 0.00001f && top - height > -0.00001f) || top > height;
			}
		} else {
			if ((from.y - height < 0.00001f && from.y - height > -0.00001f) || from.y > height) {
				float bottom = to.y - radius;
				return (bottom - height < 0.00001f && bottom - height > -0.00001f) || bottom < height;
			}
		}
	}
	return true;
}

// fxbox.inl:139
inline void CFxBox::OnSequenceCreated(CFxSequence* pSequence) {
}

// fxbox.inl:145
inline void CFxBox::OnSequenceDeleted(CFxSequence* pSequence) {
}

// fxbox.inl:151
inline void CFxBox::OnSprayCreated(CFxSpray* pSpray) {
}

// fxbox.inl:157
inline void CFxBox::OnSprayDeleted(CFxSpray* pSpray) {
}
