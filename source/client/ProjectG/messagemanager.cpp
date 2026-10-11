// Original: d:\build\custom\temp\code\client\projectg\messagemanager.cpp
#include "MessageManager.h"
#include "TaskManager.h"
#include "actor.h"

namespace _systemmsg {

// messagemanager.cpp:17
CMessageManager::CMessageManager(void) {

}

// messagemanager.cpp:22
CMessageManager::~CMessageManager(void) {

}

// messagemanager.cpp:27
bool CMessageManager::InsertMessage(eMsgIdentifier id, const char* text) {
	MessageMap::const_iterator it = m_messages.find(id);
	if (it != m_messages.end()) {

		return false;
	}


	m_messages.insert(MessageMap::value_type(id, text));

	return true;
}

// messagemanager.cpp:40
bool CMessageManager::InsertMessageGroup(sMsgInfo* pInfo, int count) {

	int total = pInfo[count].id;
	for (int i = 0; i < total; i++) {

		MessageMap::const_iterator it = m_messages.find(pInfo[i].id);
		if (it != m_messages.end()) {

			return false;
		}


		m_messages.insert(std::make_pair(pInfo[i].id, pInfo[i].text));
	}


	return true;
}

// messagemanager.cpp:58
const char* CMessageManager::GetMsg(eMsgIdentifier id) {
	MessageMap::const_iterator it = m_messages.find(id);
	if (it == m_messages.end()) {

		return 0;
	}


	return it->second;
}

// messagemanager.cpp:69
void CMessageManager::NotifyNormalMessage(eMsgIdentifier id) {
	const char* text = GetMsg(id);

	if (!text) {

		text = "\xC1\xF6\xC1\xA4\xB5\xC7\xC1\xF6 \xBE\xCA\xC0\xBA \xB8\xDE\xBD\xC3\xC1\xF6 \xC4\xDA\xB5\xE5\xC0\xD4\xB4\xCF\xB4\xD9.";
	}


	CTaskManager* pTaskManager = CTaskManager::Instance();
	(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)text, 0, 0, 0, 0));
}

// messagemanager.cpp:81
void CMessageManager::NotifyNormalMessage(const char* text) {
	if (!text) {
		return;
	}





	CTaskManager* pTaskManager = CTaskManager::Instance();
	(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)text, 0, 0, 0, 0));
}

} // namespace _systemmsg
