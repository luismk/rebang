// Stage 3: definitions whose declarations exist in the recovered headers but
// whose original defining translation unit was not present in the source set.
#include "stdafx.h"
#include "../../shared/globalgamedefine.h"

sUserPosition::sUserPosition(void)
{
	m_roomNumber = 0xFFFF;
	m_value2 = 0xFFFFFFFF;
	m_userValue = 0xFFFFFFFF;
	m_channelId = 0xFF;
	memset(m_channelName, 0, sizeof(m_channelName));
}
