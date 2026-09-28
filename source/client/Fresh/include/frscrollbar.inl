#pragma once
#include "frscrollbar.h"

// HACK: workaround for unsolved emission order issue
inline void FrScrollBar::SetGuideVisible(bool visible)
{
	m_showGuide = visible;
}
