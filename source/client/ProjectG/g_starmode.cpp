// Original: d:\build\custom\temp\code\client\projectg\g_starmode.cpp
#include "NetworkSystem.h"
#include "G_StarMode.h"

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "TaskManager.h"
#include "Packet.h"
#include "actor.h"
#include "SharedDoc.h"
#include "parttidlist.h"

// The project's SDK has no <wininet.h>; the function and the two flags used
// here are declared locally.
extern "C" __declspec(dllimport) BOOL __stdcall InternetGetConnectedState(LPDWORD lpdwFlags, DWORD dwReserved);
#define INTERNET_CONNECTION_LAN 0x02
#define INTERNET_CONNECTION_CONFIGURED 0x40

// The first line of a character settings file, and the line written in its
// place when saving fails (adjacent .rdata at 0xA2980C and 0xA2982C, not
// pooled literals).
static const char s_settingsHeader[] = "SINGLE_PLAY_CHARACTER_SETTINGS\n";
static const char s_errorHeader[] = "ERROR!\n";

// g_starmode.cpp:39
CGStarMode::CGStarMode(void) {
	memset(m_parts, 0, sizeof(m_parts));

	m_pSecondBuffer = NULL;
	m_pFirstBuffer = NULL;
}

// g_starmode.cpp:47
CGStarMode::~CGStarMode(void) {
	if (m_pSecondBuffer) { delete[] m_pSecondBuffer; m_pSecondBuffer = NULL; }
	if (m_pFirstBuffer) { delete[] m_pFirstBuffer; m_pFirstBuffer = NULL; }
}

// g_starmode.cpp:92
void CGStarMode::SetPlayerData(void) {
	CPartTidList partTidList;
	char path[MAX_PATH]; // unused: the /GS cookie of the original comes from a char buffer left over from the compiled-out body

	// The rest of the original body (lines 100-156) was compiled out; only the clears remain.
	WSingleton<CSharedDoc>::Instance()->m_myGearList.clear();
	WSingleton<CSharedDoc>::Instance()->m_characterMap.clear();
	WSingleton<CSharedDoc>::Instance()->m_caddieMap.clear();
}

// g_starmode.cpp:183
void CGStarMode::GetAllItemFromItemManager(void) {
}

// g_starmode.cpp:344
void ErrorMessageToMyRoom(void *param) {
	CTaskManager::Instance()->PostMsg(NULL, "RealMyRoom", 0x23, (intptr_t)param, 0, 0, 0);
}

// g_starmode.cpp:516
bool CGStarMode::LoadFile(const char *fileName) {
	bool loaded = false;
	char line[0x2800] = "";
	char delims[] = "\t\r\n|";

	FILE *fp = fopen(fileName, "rt");
	if (fp != NULL) {
		if (fgets(line, sizeof(line), fp) != NULL) {
			if (strcmp(line, s_errorHeader) != 0) {
				if (strcmp(line, s_settingsHeader) == 0) {
					while (fgets(line, sizeof(line), fp) != NULL) {
						char *token = strtok(line, delims);
						if (token == NULL)
							goto close; // a short line: the file stays not loaded
						int entry = atoi(token);

						for (int part = 0; part < 10; part++) {
							token = strtok(NULL, delims);
							if (token == NULL)
								goto close;
							Parse(entry, part, token);
						}
					}
					loaded = true;
				}
			}
		}
close:
		fclose(fp);
	}
	return loaded;
}

// g_starmode.cpp:573
void CGStarMode::Parse(int entry, int part, char *text) {
	if (atoi(text) == 0) {
		m_parts[entry][part] = 0;
		return;
	}

	unsigned char digits[5];
	char digit[12];
	for (int i = 0; i < 5; i++) {
		lstrcpynA(digit, text + i, 2);
		digits[i] = (unsigned char)atoi(digit);
	}

	m_parts[entry][part] = 0x08000000 | (digits[0] << 18) | (digits[1] << 13) | (digits[2] << 11) | (digits[3] << 9) | digits[4];
}

// g_starmode.cpp:591
void CGStarMode::SetParts(int entry, unsigned long *parts) {
	unsigned long sum = 0;
	for (int i = 0; i < STARMODE_PART_COUNT; i++) {
		sum += m_parts[entry][i];
	}

	if (sum != 0) {
		memcpy(parts, m_parts[entry], sizeof(m_parts[entry]));
	}
}

// g_starmode.cpp:604
void CGStarMode::SkipServer(void) {
	WNetworkSystem *pNetwork = WNetworkSystem::Instance();

	if (pNetwork->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
		pNetwork->ForceShutDown(WNetworkSystem::NET_UNIT_DEFAULT);
	}
}

// g_starmode.cpp:630
void CGStarMode::Skip30vs(void) {
	WSendPacket packet(4);
	packet.Encode1(8);
	packet.Send(SEND_TO_DEFAULT);

	AfxGetTask()->GetActor("Lobby") << MsgObject(0, 1, 0, 0, 0, 0, 0);
}

// g_starmode.cpp:643
bool CGStarMode::IsInternetConnect(void) {
	DWORD flags;

	if (InternetGetConnectedState(&flags, 0)) {
		if (flags & INTERNET_CONNECTION_CONFIGURED)
			return true;
		else if (flags & INTERNET_CONNECTION_LAN)
			return true;
		else
			return false;
	}
	return false;
}
