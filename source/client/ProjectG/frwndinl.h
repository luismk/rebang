#pragma once

#include "frwnd.h"

inline void FrWnd::SetFixed(bool fixed)
{
	m_dwStyle.Turn(FWS_FIXED, fixed);
}

inline float FrWnd::GetAlpha() const
{
	return m_wndAlpha;
}

inline void FrWnd::SetAlpha(float alpha)
{
	m_wndAlpha = alpha;
}

inline FrScrollBar* FrWnd::GetScrollBar()
{
	return m_pScrBar;
}
