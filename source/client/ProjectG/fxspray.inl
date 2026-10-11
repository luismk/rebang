// Original: d:\build\custom\temp\code\client\projectg\fxspray.inl
// Inline members of CFxSpray, included at the end of fxspray.h.

// fxspray.inl:51
// Where a particle's position is measured from: the spray when the particles
// follow it, the particle's own position otherwise.
inline WVector CFxSpray::GetBgPos(const CFxParticle* particle) {
	return m_param.m_bSurvivesSequence ? GetBgPos() : particle->m_pos;
}

// fxspray.inl:57
inline WMatrix CFxSpray::GetBgRot(const CFxParticle* particle) {
	return m_param.m_bSurvivesSequence ? m_matrix : particle->m_rot;
}

// fxspray.inl:63
inline void CFxSpray::SetActive(bool bActive) {
	m_bActive = bActive;
}

// fxspray.inl:84
inline WVector& CFxSpray::InitialMinVel(void) {
	return m_param.m_initialMinVel;
}

// fxspray.inl:91
inline WVector& CFxSpray::InitialMaxVel(void) {
	return m_param.m_initialMaxVel;
}

// fxspray.inl:107
// The product of the rotations of the attachment matrices, up to the first
// missing one.
inline void CFxSpray::GetBoneMatrix(WMatrix* pMatrix) {
	pMatrix->Reset();

	for (int i = 0; i < 4; i++) {

		if (m_pMatrix[i] == 0)
			break;

		WMatrix rot;
		m_pMatrix[i]->GetRotMatrix(&rot);
		*pMatrix = *pMatrix * rot;
	}
}
