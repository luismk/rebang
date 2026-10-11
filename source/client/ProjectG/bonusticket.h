// Original: d:\build\custom\temp\code\client\projectg\bonusticket.h
#ifndef PG_CLIENT_PROJECTG_BONUSTICKET_H
#define PG_CLIENT_PROJECTG_BONUSTICKET_H

#include <map>
#include <string>
#include <vector>

#include "..\..\defines.h"

// Singleton holding, per name, a shuffled table of 100 bonus tickets that
// is drawn in order (the ticket values are table indices).
class CBonusTicket {
	LAYOUT_CHECK_FRIEND;
public:
	struct sTicketTable {
		unsigned char m_tickets[100];   // 0x00, ticket values, shuffled
		int m_index;                    // 0x64, next ticket to draw

		// bonusticket.h:37
		sTicketTable() : m_index(0) { memset(m_tickets, 0, sizeof(m_tickets)); }

		// bonusticket.h:39
		int GetTicket() {
			int ticket = m_tickets[m_index++];
			if (m_index >= 100) {
				m_index = 0;
			}
			return ticket;
		}
	};

	static CBonusTicket& Instance();

	void SetTicketFromServer(std::string name, unsigned char* pTickets, unsigned int size);
	void __cdecl SetTicketFromClient(std::string name, int count, ...);
	void DeleteTable(std::string name);
	int GetRandomTicket(std::string name);

private:
	CBonusTicket();
	~CBonusTicket();

	void Release();
	void SetRandomTicket(sTicketTable& table, std::vector<int>& counts);

	std::map<std::string, sTicketTable*> m_tables;   // 0x00
};

CHECK_SIZE(CBonusTicket::sTicketTable, 0x68);
CHECK_SIZE(CBonusTicket, 0xc);
BEGIN_LAYOUT_CHECK(CBonusTicket)
	CHECK_FIELD(CBonusTicket, m_tables, 0x0);
	CHECK_FIELD(CBonusTicket::sTicketTable, m_index, 0x64);
END_LAYOUT_CHECK

#endif
