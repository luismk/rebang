// Original: d:\build\custom\temp\code\client\projectg\sequencestatemachine.h
#ifndef PG_CLIENT_PROJECTG_SEQUENCESTATEMACHINE_H
#define PG_CLIENT_PROJECTG_SEQUENCESTATEMACHINE_H

#include "..\..\defines.h"

#include <list>
#include <string>

class CSequenceStateMachine;

// One state of a CSequenceStateMachine. The machine orders its units by
// m_order (operator<) and drives them through the private virtuals.
// The PDB spells the first virtual GetClassNameA: windows.h's GetClassName
// macro renamed it in the original.
class CStateUnit {
	LAYOUT_CHECK_FRIEND;
	friend class CSequenceStateMachine;
public:
	virtual char const* GetClassNameA(void) const = 0;
	virtual ~CStateUnit(void) {}

	bool operator<(CStateUnit const* pOther) const {
		return m_order < pOther->m_order;
	}

protected:
	CStateUnit(void) : m_order(0) {}

	void SetOwner(CSequenceStateMachine* pOwner) { m_pOwner = pOwner; }
	void OnPrintLog(char const* pszLog);

private:
	virtual void OnProcess(float fDelta) = 0;
public:
	virtual void OnRender(void) = 0;
private:
	virtual bool OnFinished(void) = 0;

protected:
	CSequenceStateMachine* m_pOwner; // 0x4
	int m_order;                     // 0x8
};
CHECK_SIZE(CStateUnit, 0xC);
BEGIN_LAYOUT_CHECK(CStateUnit)
	CHECK_FIELD(CStateUnit, m_pOwner, 0x4);
	CHECK_FIELD(CStateUnit, m_order, 0x8);
END_LAYOUT_CHECK

// Runs its CStateUnits one after the other, in m_order order. The machine
// owns the units: Clear deletes them.
class CSequenceStateMachine {
	LAYOUT_CHECK_FRIEND;
public:
	CSequenceStateMachine(void const* pUser, char const* pszName);
	~CSequenceStateMachine(void);

	void Clear(void);
	void Reset(void);
	CStateUnit* GetCurrentState(void);
	CStateUnit* GetPreviousState(void);
	CStateUnit* GetNextState(void);
	bool SkipCurrentState(void);
	bool Process(float fDelta);
	void Render(void);
	void Insert(int order, CStateUnit* pState);
	void BackInsert(CStateUnit* pState);

	void PrintLog(CStateUnit const* pState, char const* pszLog);

private:
	void PrintLog(char const* pszFormat, ...);

	std::list<CStateUnit*> m_list;            // 0x0
	std::list<CStateUnit*>::iterator m_current; // 0xC
	int m_lastOrder;                          // 0x10
	void const* m_pUser;                      // 0x14
	std::string m_name;                       // 0x18
	int m_unknown34;                          // 0x34
	int m_unknown38;                          // 0x38
};
CHECK_SIZE(CSequenceStateMachine, 0x3C);
BEGIN_LAYOUT_CHECK(CSequenceStateMachine)
	CHECK_FIELD(CSequenceStateMachine, m_list, 0x0);
	CHECK_FIELD(CSequenceStateMachine, m_current, 0xC);
	CHECK_FIELD(CSequenceStateMachine, m_lastOrder, 0x10);
	CHECK_FIELD(CSequenceStateMachine, m_pUser, 0x14);
	CHECK_FIELD(CSequenceStateMachine, m_name, 0x18);
	CHECK_FIELD(CSequenceStateMachine, m_unknown34, 0x34);
	CHECK_FIELD(CSequenceStateMachine, m_unknown38, 0x38);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SEQUENCESTATEMACHINE_H
