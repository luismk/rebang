// Original: d:\build\custom\temp\code\client\projectg\pangfbi.cpp
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "PangFBI.h"
#include "GenericHTTPClient.h"

// pangfbi.cpp:15
CPangFBI::CPangFBI(void) {
}

// pangfbi.cpp:20
CPangFBI::~CPangFBI(void) {
}

// pangfbi.cpp:26
void CPangFBI::ProcessReport(const char *type, const char *data) {
	if (strcmpi("crime_log", type) == 0) {
		int reportType = 0;

		char id[128] = {0};
		char fileName[776];                 // size fixed by the frame layout (0x308)
		sscanf(data, "%s %d %s", id, &reportType, fileName);

		FILE *fp = fopen(fileName, "rb");

		if (fp == NULL) {
			return;
		}
		fclose(fp);

		GenericHTTPClient *client = new GenericHTTPClient;

		client->InitilizePostArguments();

		client->AddPostArguments("id", id, FALSE);
		client->AddPostArguments("type", reportType);
		client->AddPostArguments("file", fileName, TRUE);

		if (client->Request("http://qa.reports.pangya.gametree.co.kr:50000/post/CrimeReport_KR.asp", 3, "MERONG(0.9/;p)")) {
			client->QueryHTTPResponse();
		}
		delete client;
		DeleteFile(fileName);
	} else if (strcmpi("app_log", type) == 0) {
		int code = 0;
		char userId[128] = {0};
		char packetVer[64];
		char clientVer[64];
		char comment[128];
		sscanf(data, "%d %s %s %s %s", &code, userId, packetVer, clientVer, comment);

		GenericHTTPClient *client = new GenericHTTPClient;
		client->InitilizePostArguments();

		client->AddPostArguments("code", code);
		client->AddPostArguments("userid", userId, FALSE);
		client->AddPostArguments("packet_ver", packetVer, FALSE);
		client->AddPostArguments("client_ver", clientVer, FALSE);
		client->AddPostArguments("comment", comment, FALSE);

		if (client->Request("http://qa.reports.pangya.gametree.co.kr:50000/post/GameBugReport_KR.asp", 3, "MERONG(0.9/;p)")) {
			client->QueryHTTPResponse();
		}
		delete client;
	}
}
