#pragma once

#include <string>
#include <vector>
#include <map>

class CBonusTicket
{
public:
	static CBonusTicket& Instance();

private:
	CBonusTicket();
	~CBonusTicket();

public:
	int GetRandomTicket(std::string name);
	void DeleteTable(std::string name);
	void SetTicketFromServer(std::string name, unsigned char* ticket,
		unsigned int size);
	void __cdecl SetTicketFromClient(std::string name, int num, ...);

	struct sTicketTable;

private:
	void Release();
	void SetRandomTicket(sTicketTable& table, std::vector<int>& rate);

	std::map<std::string, sTicketTable*> m_ticketTable;
};
