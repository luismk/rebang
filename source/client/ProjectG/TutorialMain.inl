// Original: d:\build\custom\temp\code\client\projectg\tutorialmain.inl
// Grade (tutorial category) helpers of the tutorial lobby. Inline; the
// original also holds out-of-line copies of them in TutorialMain's object.
#ifndef PG_CLIENT_PROJECTG_TUTORIALMAIN_INL
#define PG_CLIENT_PROJECTG_TUTORIALMAIN_INL

// tutorialmain.inl:38 (one past the last m_Tutorials index of a grade)
inline unsigned char GetAccumulateLimit(unsigned char grade) {
	unsigned char limit = 0;
	switch (grade) {
	case 0:
		limit = 8;
		break;
	case 1:
		limit = 14;
		break;
	case 2:
		limit = 0;
		break;
	}
	return limit;
}

// tutorialmain.inl:56 (the first m_Tutorials index of a grade)
inline unsigned char GetGradeBegin_Grade(unsigned char grade) {
	unsigned char begin = 0;
	switch (grade) {
	case 0:
		begin = 0;
		break;
	case 1:
		begin = 8;
		break;
	case 2:
		begin = 0;
		break;
	}
	return begin;
}

#endif // PG_CLIENT_PROJECTG_TUTORIALMAIN_INL
