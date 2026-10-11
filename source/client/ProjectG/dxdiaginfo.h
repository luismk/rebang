// Original: d:\build\custom\temp\code\client\projectg\dxdiaginfo.h
#ifndef PG_CLIENT_PROJECTG_DXDIAGINFO_H
#define PG_CLIENT_PROJECTG_DXDIAGINFO_H

#include "..\..\defines.h"

// System information gathered through dxdiag (Init clears the three text
// buffers and the gathered flag). The size assumes nothing follows
// m_isGathered (no padding: the class has only char members).
class CDxDiagInfo {
public:
	CDxDiagInfo();  // dxdiaginfo.h:277
	~CDxDiagInfo(); // dxdiaginfo.h:278

	void Init(void);
	void StartGathering(void);
	void GatherInfo(void);

	const char* GetOSName(void);          // dxdiaginfo.h:285
	const char* GetCPUInfo(void);         // dxdiaginfo.h:286
	const char* GetGraphicCardInfo(void);
	bool IsGathered(void);                // dxdiaginfo.h:289

	char m_osName[0x100];          // 0x000
	char m_cpuInfo[0x100];         // 0x100
	char m_graphicCardInfo[0x100]; // 0x200
	bool m_isGathered;             // 0x300
};
CHECK_FIELD(CDxDiagInfo, m_osName, 0x000);
CHECK_FIELD(CDxDiagInfo, m_cpuInfo, 0x100);
CHECK_FIELD(CDxDiagInfo, m_graphicCardInfo, 0x200);
CHECK_FIELD(CDxDiagInfo, m_isGathered, 0x300);
CHECK_SIZE(CDxDiagInfo, 0x301);

#endif // PG_CLIENT_PROJECTG_DXDIAGINFO_H
