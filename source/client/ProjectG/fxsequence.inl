// Original: d:\build\custom\temp\code\client\projectg\fxsequence.inl
// Inline members of CFxSequence, included at the end of fxsequence.h.
// SetDelay (fxsequence.inl:63) is still defined out of line in guideline.cpp.
//
// SetActive needs CFxSpray complete, which fxspray.h only gives after
// FxBox.h: FxBox.inl includes this file a second time, after fxspray.h, with
// PG_FXSEQUENCE_INL_WITH_SPRAY defined, and only that pass defines it.

#ifndef PG_CLIENT_PROJECTG_FXSEQUENCE_INL
#define PG_CLIENT_PROJECTG_FXSEQUENCE_INL

// fxsequence.inl:26
inline WVector& CFxSequence::Pos(void) {
	return m_pos;
}

// fxsequence.inl:40
inline std::map<CFxSpray*, CFxSpray*>& CFxSequence::GetSprayList(void) {
	return m_sprays;
}

#endif // PG_CLIENT_PROJECTG_FXSEQUENCE_INL

#if defined(PG_FXSEQUENCE_INL_WITH_SPRAY) && !defined(PG_CLIENT_PROJECTG_FXSEQUENCE_INL_SPRAY)
#define PG_CLIENT_PROJECTG_FXSEQUENCE_INL_SPRAY

// fxsequence.inl:7
// Turns the sequence on or off and, with bChildren, its sprays.
inline void CFxSequence::SetActive(bool bActive, bool bChildren) {
	if (m_bActive != bActive) {
		m_bActive = bActive;

		if (bChildren) {
			for (std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.begin(); it != m_sprays.end(); ++it) {
				it->second->SetActive(bActive);
			}
		}
	}
}

#endif // PG_CLIENT_PROJECTG_FXSEQUENCE_INL_SPRAY
