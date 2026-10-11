// Original: d:\build\custom\temp\code\client\projectg\binstr.inl
// Inline members of cToken, cTokenV and cTokenS (binstr.h).

// binstr.inl:6
inline int cToken::GetPos(void) const {
	return m_Pos;
}

// binstr.inl:26
inline int cToken::GetLen(void) const {
	return m_Len;
}

// binstr.inl:40
inline bool cToken::GetToken(char **ppToken, const char *sep, int sepLen) {
	if (!GetResultBuf()) {
		if (ppToken) {
			*ppToken = 0;
		}
		return false;
	}
	if (ppToken) {
		*ppToken = m_pcResBuf;
	}
	return GetToken(m_pcResBuf, sep, sepLen);
}

// binstr.inl:60
inline bool cToken::GetTokenWithoutFail(char *const pToken, const char *sep, int sepLen) {
	bool ret = false;
	*pToken = 0;
	while (m_Pos < m_Len) {
		ret = GetToken(pToken, sep, sepLen);
		if (*pToken != 0) {
			break;
		}
	}
	return ret;
}

// binstr.inl:78
inline bool cToken::GetTokenWithoutFail(char **ppToken, const char *sep, int sepLen) {
	if (!GetResultBuf()) {
		if (ppToken) {
			*ppToken = 0;
		}
		return false;
	}
	if (ppToken) {
		*ppToken = m_pcResBuf;
	}
	return GetTokenWithoutFail(m_pcResBuf, sep, sepLen);
}

// binstr.inl:98
inline bool cToken::GetTokenFullSep(char **ppToken, const char *sep, int sepLen) {
	if (!GetResultBuf()) {
		if (ppToken) {
			*ppToken = 0;
		}
		return false;
	}
	if (ppToken) {
		*ppToken = m_pcResBuf;
	}
	return GetTokenFullSep(m_pcResBuf, sep, sepLen);
}

// binstr.inl:118
inline const char *cTokenV::GetBuf(void) const {
	return m_pcBuf;
}

// binstr.inl:125
inline const char *cTokenS::GetBuf(void) const {
	return m_pcBuf;
}

// binstr.inl:135
inline int PosB(const char *str, int strLen, const char *sub, int subLen) {
	for (int i = 0; i < strLen - subLen + 1; i++) {
		bool bFind = true;
		for (int j = 0; j < subLen; j++) {
			if (str[i + j] != sub[j]) {
				bFind = false;
				break;
			}
		}
		if (bFind) {
			return i;
		}
		if (str[i] & 0x80) {
			i++;
		}
	}
	return -1;
}
