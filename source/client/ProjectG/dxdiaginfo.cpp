// Original: d:\build\custom\temp\code\client\projectg\dxdiaginfo.cpp
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "DxDiagInfo.h"
#include "tinyxml.h"

// dxdiaginfo.cpp:13
void CDxDiagInfo::Init(void) {
	memset(m_osName, 0, sizeof(m_osName));
	memset(m_cpuInfo, 0, sizeof(m_cpuInfo));
	memset(m_graphicCardInfo, 0, sizeof(m_graphicCardInfo));
	m_isGathered = false;
}

// dxdiaginfo.cpp:21
void CDxDiagInfo::StartGathering(void) {
	CreateDirectoryA("GameGuard", NULL);
	char command[0x100];
	sprintf(command, "cmd /c \"dxdiag /whql:off /x %s\"", "./gameguard/npsys.des");
	WinExec(command, SW_HIDE);
}

// dxdiaginfo.cpp:48
void CDxDiagInfo::GatherInfo(void) {
	if (m_isGathered) {
		return;
	}

	TiXmlDocument doc;

	if (doc.LoadFile("./gameguard/npsys.des")) {
		m_isGathered = true;
		TiXmlNode* dxDiag = doc.FirstChild("DxDiag");
		if (dxDiag) {
			TiXmlNode* systemInformation = dxDiag->FirstChild("SystemInformation");
			if (systemInformation) {
				TiXmlNode* processor = systemInformation->FirstChild("Processor");
				if (processor && processor->FirstChild()) {
					strcpy(m_cpuInfo, processor->FirstChild()->Value());
				}

				TiXmlNode* operatingSystem = systemInformation->FirstChild("OperatingSystem");
				if (operatingSystem && operatingSystem->FirstChild()) {
					strcpy(m_osName, operatingSystem->FirstChild()->Value());
				}
			}

			TiXmlNode* displayDevices = dxDiag->FirstChild("DisplayDevices");
			if (displayDevices) {
				TiXmlNode* displayDevice = displayDevices->FirstChild("DisplayDevice");
				if (displayDevice) {
					TiXmlNode* cardName = displayDevice->FirstChild("CardName");
					if (cardName && cardName->FirstChild()) {
						strcpy(m_graphicCardInfo, cardName->FirstChild()->Value());
					}
				}
			}
		}
		DeleteFileA("./gameguard/npsys.des");
	}
}
