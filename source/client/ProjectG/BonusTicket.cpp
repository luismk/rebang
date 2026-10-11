// Original: d:\build\custom\temp\code\client\projectg\bonusticket.cpp
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "BonusTicket.h"

// bonusticket.cpp:6
CBonusTicket& CBonusTicket::Instance() {
	static CBonusTicket instance;

	return instance;
}

// bonusticket.cpp:12
CBonusTicket::CBonusTicket() {
}

// bonusticket.cpp:17
CBonusTicket::~CBonusTicket() {
	Release();
}

// bonusticket.cpp:23
void CBonusTicket::Release() {
	std::map<std::string, sTicketTable*>::iterator it = m_tables.begin();
	while (it != m_tables.end()) {
		if (it->second != NULL) {
			delete it->second;
			it->second = NULL;
			m_tables.erase(it++);
		} else {
			++it;
		}
	}
	m_tables.clear();
}

// bonusticket.cpp:40
void CBonusTicket::DeleteTable(std::string name) {
	std::map<std::string, sTicketTable*>::iterator it = m_tables.find(name);
	if (it != m_tables.end()) {
		if (it->second != NULL) {
			delete it->second;
			it->second = NULL;
			m_tables.erase(it);
		}
	}
}

// bonusticket.cpp:54
void CBonusTicket::SetTicketFromServer(std::string name, unsigned char* pTickets, unsigned int size) {
	if (size == 100) {
		DeleteTable(name);
		sTicketTable* pTable = new sTicketTable;
		memcpy(pTable->m_tickets, pTickets, 100);
		m_tables[name] = pTable;
	}
}

// bonusticket.cpp:78
void __cdecl CBonusTicket::SetTicketFromClient(std::string name, int count, ...) {
	DeleteTable(name);

	std::vector<int> counts;

	va_list args;
	va_start(args, count);
	counts.reserve(count);

	int total = 0;
	for (int i = 0; i < count; ++i) {
		int n = va_arg(args, int);
		total += n;
		counts.push_back(n);
	}
	va_end(args);

	if (total > 100) {
		counts.clear();
	}

	sTicketTable* pTable = new sTicketTable;
	SetRandomTicket(*pTable, counts);
	m_tables[name] = pTable;
}

// bonusticket.cpp:122
void CBonusTicket::SetRandomTicket(sTicketTable& table, std::vector<int>& counts) {
	int pos = 0;
	int ticket = 0;
	for (std::vector<int>::iterator it = counts.begin(); it != counts.end(); ++it) {
		int n = *it;
		for (int j = 0; j < n; ++j) {
			table.m_tickets[pos++] = (unsigned char)ticket;
		}
		++ticket;
	}

	for (int i = 0; i < 100; ++i) {
		int r = rand() % 100;
		std::swap(table.m_tickets[i], table.m_tickets[r]);
	}
}

// bonusticket.cpp:148
int CBonusTicket::GetRandomTicket(std::string name) {
	std::map<std::string, sTicketTable*>::iterator it = m_tables.find(name);
	if (it != m_tables.end()) {
		sTicketTable* pTable = it->second;
		return pTable->GetTicket();
	}
	return 0;
}
