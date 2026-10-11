// Original: d:\build\custom\temp\code\client\projectg\golfball.inl
// Inline members of CGolfBall, included at the end of golfball.h.

// golfball.inl:8
GOLFRULE_GOLFBALL_INLINE const CGolfBall::sExpected* CGolfBall::PreData(int index) const {
	if (index < 0 || index > m_preDataCount) {
		return 0;
	}

	if (index < 1500) {
		return &m_preData[index];
	}
	return &m_pPreDataOverflow[index - 1500];
}

// golfball.inl:80 (out-of-line copy in SharedDoc's object)
inline CGolfBall::~CGolfBall(void) {
	if (m_pPreDataOverflow) {
		delete[] m_pPreDataOverflow;
		m_pPreDataOverflow = 0;
	}
}
