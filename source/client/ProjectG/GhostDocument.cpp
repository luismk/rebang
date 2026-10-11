// Original: d:\build\custom\temp\code\client\projectg\ghostdocument.cpp

#include <windows.h>

#include "ghostdocument.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iterator>

#include "GolfDoc.h"
#include "actor.h"
#include "club.h"
#include "SharedDoc.h"
#include "ghosttypes.h"
#include "westpak.h"
#include "../shared/globalgamedefine.h"
#include "type_convert.h"

cFile *__cdecl GetCFile(const char *fileName, int a, int b);
void __cdecl CloseCFile(cFile *file);

namespace {

// ghostdocument.cpp:27
// Copies the bits of a value into a variable of another type of the same
// size (float <-> int), without numeric conversion.
template <typename T, typename U>
void py_data_cast(T &src, U &dst) {
	dst = reinterpret_cast<const U &>(src);
}

// The callers are not decompiled yet; these instantiate the two conversions
// the original object emits.
template void py_data_cast<float, int>(float &src, int &dst);
template void py_data_cast<const int, float>(const int &src, float &dst);

// Function object: converts a legacy card into its ghost-file form.
struct card_converter {
	card_converter(void) {}
	GHOST_CARD_DATA operator()(const sSCardAvilityPeriodInfo &card) const;
};

// ghostdocument.cpp:34
GHOST_CARD_DATA card_converter::operator()(const sSCardAvilityPeriodInfo &card) const {
	GHOST_CARD_DATA ghostCard = { 0 };
	type_convert(&card, &ghostCard);

	return ghostCard;
}

// ghostdocument.cpp:46
const char *GetCardTypeString(int type) {
	if (type == 0) return "CARD_CHARACTER";
	if (type == 1) return "CARD_CADDIE";
	if (type == 2) return "CARD_SPECIAL";
	if (type == 3) return "CARD_PACK";
	if (type == 4) return "CARD_BOX";

	return "NOT DEFINED";
}

// ghostdocument.cpp:61
const char *GetCardAbilityString(int ability) {
	if (ability == -1) return "SPECIAL_AVILITY_NONE";
	if (ability == 0) return "SPECIAL_EXP_ADD";
	if (ability == 1) return "SPECIAL_PANG_RATE_UP_TIME";
	if (ability == 2) return "SPECIAL_EXP_REAE_UP_TIME";
	if (ability == 3) return "SPECIAL_PANG_ADD";
	if (ability == 4) return "SPECIAL_POWER_UP_TIME";
	if (ability == 5) return "SPECIAL_CONTROL_UP_TIME";
	if (ability == 6) return "SPECIAL_ACCCURACY_UP_TIME";
	if (ability == 7) return "SPECIAL_SPIN_UP_TIME";
	if (ability == 8) return "SPECIAL_CURVE_UP_TIME";
	if (ability == 9) return "SPECIAL_START_COMBOGAUGE_UP_TIME";
	if (ability == 10) return "SPECIAL_SLOT_UP_TIME";
	if (ability == 11) return "SPECIAL_PANGYAZONE_UP";
	if (ability == 12) return "SPECIAL_CLEARBONUS_UP_SEPIAWIND";
	if (ability == 13) return "SPECIAL_CLEARBONUS_UP_WINDHILL";
	if (ability == 14) return "SPECIAL_CLEARBONUS_UP_PINKWIND";
	if (ability == 15) return "SPECIAL_CLEARBONUS_UP_BLUEMOON";
	if (ability == 16) return "SPECIAL_RANDOMPANG_ADD";
	if (ability == 17) return "SPECIAL_TREASUREPOINT_UP";
	if (ability == 18) return "SPECIAL_RAINFALL_PROBABILITY_UP";

	return "NOT DEFINED";
}

// Function object for std::for_each: writes one GHOST_CARD_DATA to a text
// file.
struct print_ghost_card_data {
	explicit print_ghost_card_data(FILE *fp);
	void operator()(const GHOST_CARD_DATA &card) const;

	FILE *m_fp; // 0x0
};
CHECK_SIZE(print_ghost_card_data, 0x4);
CHECK_FIELD(print_ghost_card_data, m_fp, 0x0);

// ghostdocument.cpp:89
const char *GetActionString(GHOST::eGHOSTACTMSG act) {
	if (act == GHOST::CHANGE_WIND) return "GHOST::CHANGE_WIND";
	if (act == GHOST::CHANGE_WEATHER) return "GHOST::CHANGE_WEATHER";
	if (act == GHOST::CHANGE_DIRECTION) return "GHOST::CHANGE_DIRECTION";
	if (act == GHOST::CHANGE_CLUB) return "GHOST::CHANGE_CLUB";
	if (act == GHOST::USE_ITEM) return "GHOST::USE_ITEM";
	if (act == GHOST::BALL_DROP) return "GHOST::BALL_DROP";
	if (act == GHOST::TIME_OUT) return "GHOST::TIME_OUT";
	if (act == GHOST::GAME_TIME_OVER) return "GHOST::GAME_TIME_OVER";
	if (act == GHOST::SAVE_SHOT_DATA) return "GHOST::SAVE_SHOT_DATA";

	return "NOT DEFINED";
}

// ghostdocument.cpp:107
print_ghost_card_data::print_ghost_card_data(FILE *fp) : m_fp(fp) {}

// ghostdocument.cpp:109
void print_ghost_card_data::operator()(const GHOST_CARD_DATA &card) const {
	if (card.partsTypeId) {
		fprintf(m_fp, "\tATTACHED : %d(%s), PARTS_ITEMID : %d\n", card.partsTypeId,
			ItemManager()->GetItemName(card.partsTypeId), card.partsId);
	}

	fprintf(m_fp, "\tCARD TYPEID : %d(%s)\n", card.typeId, ItemManager()->GetItemName(card.typeId));
	fprintf(m_fp, "\tCARD ITEMID : %d\n", card.id);
	fprintf(m_fp, "\tCARD TYPE : %d(%s)\n", card.kind, GetCardTypeString(card.kind));
	fprintf(m_fp, "\tCARD ABILITY : %d(%s)\n", card.effect, GetCardAbilityString(card.effect));
	fprintf(m_fp, "\tABILITY VALUE : %d\n", card.effectValue);
	fprintf(m_fp, "\tCARD SLOTNUM : %d\n", card.slot);

	fprintf(m_fp, "\n");
}

} // namespace

// ghostdocument.cpp:137
CGhostHoleData::CGhostHoleData(void) : m_holeNum(0) {
	m_pRecordingShot = new GHOST_SHOT_DATA;
	memset(m_pRecordingShot, 0, sizeof(GHOST_SHOT_DATA));
}

// ghostdocument.cpp:143
CGhostHoleData::~CGhostHoleData(void) {
	std::for_each(m_shotDataList.begin(), m_shotDataList.end(), Delete_Object());
	m_shotDataList.clear();

	if (m_pRecordingShot) { delete m_pRecordingShot; m_pRecordingShot = NULL; }
}

// ghostdocument.cpp:151
void CGhostHoleData::RecordAct(int type, int arg, int const *pParams, int count) {
	GHOST_ACTION &action = m_pRecordingShot->m_actions[type];

	action.m_changed = true; action.m_arg = (unsigned char)arg;
	memset(action.m_param, 0xFF, sizeof(action.m_param));

	if (count != 0 && pParams != NULL) {
		memcpy(action.m_param, pParams, sizeof(int) * (count < 1 ? 1 : (count > 5 ? 5 : count)));
	}
}

// ghostdocument.cpp:165
void CGhostHoleData::RecordDummyShot(unsigned int shotNum) {
	m_pRecordingShot->m_type = 1; m_pRecordingShot->m_shotNum = shotNum;
	m_pRecordingShot->m_holeNum = GOLFDOC()->m_currentHole;

	GHOST_SHOT_DATA *pShotData = new GHOST_SHOT_DATA;
	*pShotData = *m_pRecordingShot;
	m_shotDataList.push_back(pShotData);

	memset(m_pRecordingShot, 0, sizeof(GHOST_SHOT_DATA));
}

// ghostdocument.cpp:178
unsigned int CGhostHoleData::RollBackShotData(unsigned int count) {
	unsigned int remain = count;

	memset(m_pRecordingShot, 0, sizeof(GHOST_SHOT_DATA));

	while (!m_shotDataList.empty() && remain != 0) {
		delete m_shotDataList.back();
		--remain;

		m_shotDataList.pop_back();
	}

	return count - remain;
}

// ghostdocument.cpp:248
void CGhostHoleData::AddShotData(GHOST_SHOT_DATA const *pShotData) {
	GHOST_SHOT_DATA *pCopy = new GHOST_SHOT_DATA;
	*pCopy = *pShotData;
	m_shotDataList.push_back(pCopy);
}

// ghostdocument.cpp:264
CGhostDocument::CGhostDocument(void) : m_shotCount(0), m_bInvalidate(0) {
	m_pUserData = new GHOST_USER_DATA;
	memset(m_pUserData, 0, sizeof(GHOST_USER_DATA));

	m_pGameData = new GHOST_GAME_DATA;
	memset(m_pGameData, 0, sizeof(GHOST_GAME_DATA));

	m_pHoleData = new CGhostHoleData[18];
	for (int i = 0; i < 18; i++)
		m_pHoleData[i].SetHoleNum(i + 1);
}

// ghostdocument.cpp:277
CGhostDocument::~CGhostDocument(void) {
	if (m_pUserData) { delete m_pUserData; m_pUserData = NULL; }
	if (m_pGameData) { delete m_pGameData; m_pGameData = NULL; }
	if (m_pHoleData) { delete[] m_pHoleData; m_pHoleData = NULL; }
}

// ghostdocument.cpp:298
void CGhostDocument::RecordUserData(void) {
	const sUserInfo &userInfo = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[MyIndex()];

	memset(m_pUserData, 0, sizeof(GHOST_USER_DATA));
	type_convert(&userInfo, m_pUserData);

	// Only the period cards still attached to an equipped part are recorded.
	const std::list<sSCardAvilityPeriodInfo> &periodList = WSingleton<CSharedDoc>::Instance()->m_cardPeriodList[MyIndex()];
	std::list<sSCardAvilityPeriodInfo> cards;
	std::copy(periodList.begin(), periodList.end(), std::back_inserter(cards));

	std::list<sSCardAvilityPeriodInfo>::iterator it = cards.begin();
	while (it != cards.end()) {
		if (it->kind == 0 || it->kind == 1) {
			bool attached = false;
			for (int i = 0; i < 24; i++) {
				if (it->partsTypeId == userInfo.m_characterInfo.partTypeIds[i] && it->partsId == userInfo.m_characterInfo.partIds[i]) {
					attached = true;
					break;
				}
			}

			if (!attached) {
				it = cards.erase(it);
				continue;
			}
		}

		++it;
	}

	if (cards.size() != 0) {
		m_cardList.reserve(cards.size());
		std::transform(cards.begin(), cards.end(), std::back_inserter(m_cardList), card_converter());
	}
}

// ghostdocument.cpp:359
void CGhostDocument::RecordHoleData(const MsgObject &msg) {
	CGhostHoleData *pHoleData = &m_pHoleData[GOLFDOC()->m_currentHole - 1];

	switch (msg.m_msg) {
	case GHOST::CHANGE_WEATHER:
		pHoleData->RecordAct(GHOST::CHANGE_WEATHER, 0, &msg.m_param1, 1);
		break;
	case GHOST::CHANGE_WIND:
		pHoleData->RecordAct(GHOST::CHANGE_WIND, 0, &msg.m_param1, 4);
		break;
	case GHOST::CHANGE_DIRECTION:
		pHoleData->RecordAct(GHOST::CHANGE_DIRECTION, 1, &msg.m_param1, 1);
		break;
	case GHOST::BALL_DROP: {
		WVector *pPos = (WVector *)msg.m_param1;
		int params[3];
		py_data_cast(pPos->x, params[0]);
		py_data_cast(pPos->y, params[1]);
		py_data_cast(pPos->z, params[2]);
		pHoleData->RecordAct(GHOST::BALL_DROP, 1, params, 3);
		pHoleData->RecordDummyShot(++m_shotCount);
		break;
	}
	case GHOST::USE_ITEM:
		pHoleData->RecordAct(GHOST::USE_ITEM, 2, &msg.m_param1, 2);
		// Using this item takes the last shot back.
		if (msg.m_param1 == 0x1800000E) {
			pHoleData->RollBackShotData(1);
			m_shotCount--;
		}
		break;
	case 6: // the power shot action (no GHOST:: name known)
		pHoleData->RecordAct(6, 0, &msg.m_param1, 1);
		break;
	case GHOST::SAVE_SHOT_DATA: {
		const sShotProtocol *pProtocol = (const sShotProtocol *)msg.m_param1;
		pHoleData->RecordShotData(pProtocol, ++m_shotCount);
		break;
	}
	case GHOST::TIME_OUT:
		pHoleData->RecordAct(GHOST::TIME_OUT, 0, NULL, 0);
		pHoleData->RecordDummyShot(++m_shotCount);
		break;
	}
}

// ghostdocument.cpp:437
void CGhostDocument::RecordException(const char *message) {
	if (message) {
		m_exceptionList.push_back(message);
	}
}

// ghostdocument.cpp:575
bool CGhostDocument::WriteTextFile(FILE *fp, CGhostHoleData const *pHoleData) const {
	std::list<GHOST_SHOT_DATA *> const &shotDataList = pHoleData->GetShotDataList();
	std::list<GHOST_SHOT_DATA *>::const_iterator it = shotDataList.begin();
	while (it != shotDataList.end()) {
		WriteTextFile(fp, *it);
		++it;
	}

	return true;
}

// ghostdocument.cpp:587
bool CGhostDocument::WriteTextFile(FILE *fp, GHOST_SHOT_DATA const *pShotData) const {
	fprintf(fp, "HOLE INDEX     : %d\n", pShotData->m_holeNum);
	fprintf(fp, "SHOT INDEX     : %d\n", pShotData->m_shotNum);
	fprintf(fp, "DUMMY SHOT     : %s\n", pShotData->m_type ? "TRUE" : "FALSE");

	const GHOST_SHOT_PROTOCOL &protocol = pShotData->m_protocol;
	fprintf(fp, "MAX CLICK      : %.02f\n", protocol.power);
	fprintf(fp, "MIN CLICK      : %.02f\n", protocol.accuracy);
	fprintf(fp, "CURVE, SPIN    : %.02f, %.02f\n", protocol.lieSlope, protocol.lieAngle);
	fprintf(fp, "ACCURACY       : %d\n", protocol.accuracyGrade);
	fprintf(fp, "SPECIAL        : %d\n", protocol.powerFlags);
	fprintf(fp, "FRAME          : %d\n", protocol.frame);
	fprintf(fp, "SHOT ANGLE     : %.02f\n", protocol.angle);
	fprintf(fp, "SHOT TIME      : %d\n", protocol.shotElapsed);
	fprintf(fp, "CENTER         : %.02f\n", protocol.barStartPosition);
	fprintf(fp, "CLUB INDEX     : %d(%s)\n", protocol.club, GolfClub().GetClubString((CClub::eClub)protocol.club));
	fprintf(fp, "RANDOM POWER   : %.02f\n", protocol.randomPower);
	// The original loads GHOST_SHOT_PROTOCOL 0x30 as a float (ghosttypes.h
	// declares it unsigned long `spot`); the conversion stands in until the
	// header is fixed.
	fprintf(fp, "RANDOM ANGLE   : %.02f\n", (float)protocol.spot);
	fprintf(fp, "CAMERA ANGLE   : %.02f\n", pShotData->m_cameraAngle);
	fprintf(fp, "\n");

	for (int i = 0; i < 9; i++) {
		const GHOST_ACTION &action = pShotData->m_actions[i];

		if (action.m_changed) {
			fprintf(fp, "\tACTION : [%s] ", GetActionString((GHOST::eGHOSTACTMSG)i));

			for (int j = 0; j < 5; j++) {
				if (action.m_param[j].m_dw != 0xFFFFFFFF) {
					if (action.m_arg == 1) {
						fprintf(fp, "%.02f ", action.m_param[j].m_f);
					} else {
						fprintf(fp, "%d ", action.m_param[j].m_dw);
					}
				}
			}

			fprintf(fp, "\n");
		}
	}

	fprintf(fp, "\n");
	fprintf(fp, "\t\tBALL POSITION : %.02f, %.02f, %.02f\n", pShotData->m_ballPos[0], pShotData->m_ballPos[1], pShotData->m_ballPos[2]);
	fprintf(fp, "\t\tWIND DIR : %.02f\n", pShotData->m_windDirection);
	fprintf(fp, "\t\tWIND PWR : %.02f\n", pShotData->m_windPower);
	fprintf(fp, "\t\tGLOBAL WIND : %.02f, %.02f, %.02f\n", pShotData->m_globalWind[0], pShotData->m_globalWind[1], pShotData->m_globalWind[2]);
	fprintf(fp, "---------------------------------------------------------------------------------\n");

	return true;
}

// ghostdocument.cpp:647
std::string CGhostDocument::MakeTextFileName(void) const {
	char fileName[MAX_PATH] = { 0 };
	sprintf(fileName, "ghost_save_data(%s).txt", MyNick());

	return fileName;
}

// ghostdocument.cpp:655
CGhostHoleData const *CGhostDocument::GetHoleData(unsigned char holeNum) const {
	CGhostHoleData const *pHoleData = NULL;
	if (m_pHoleData) {
		pHoleData = &m_pHoleData[holeNum - 1];
	}

	return pHoleData;
}

// A growable byte buffer: encode() appends at m_length, decode() reads from
// m_position.
class memory_buffer {
	LAYOUT_CHECK_FRIEND;
public:
	explicit memory_buffer(unsigned int capacity);
	memory_buffer(char *buffer, unsigned int length);
	~memory_buffer(void);

	unsigned int reserve(unsigned int capacity);
	void encode(void const *data, unsigned int size);
	void decode(void *data, unsigned int size);
	void const *get_buffer(void) const;
	unsigned int length(void) const;

private:
	char *m_buffer;          // 0x0
	unsigned int m_length;   // 0x4
	unsigned int m_position; // 0x8
	unsigned int m_capacity; // 0xC
};
CHECK_SIZE(memory_buffer, 0x10);
BEGIN_LAYOUT_CHECK(memory_buffer)
	CHECK_FIELD(memory_buffer, m_buffer, 0x0);
	CHECK_FIELD(memory_buffer, m_length, 0x4);
	CHECK_FIELD(memory_buffer, m_position, 0x8);
	CHECK_FIELD(memory_buffer, m_capacity, 0xC);
END_LAYOUT_CHECK

// ghostdocument.cpp:669
memory_buffer::memory_buffer(unsigned int capacity) : m_buffer(0), m_length(0), m_position(0) {
	m_buffer = (char *)calloc(m_capacity = capacity, 1);
}

// ghostdocument.cpp:674
memory_buffer::memory_buffer(char *buffer, unsigned int length)
	: m_buffer(0), m_length(0), m_position(0) {
	m_buffer = (char *)calloc(m_capacity = length, 1);
	encode(buffer, length);
}

// ghostdocument.cpp:681
memory_buffer::~memory_buffer(void) {
	if (m_buffer) free(m_buffer);
}

// ghostdocument.cpp:686
unsigned int memory_buffer::reserve(unsigned int capacity) {
	m_capacity = capacity;
	if (m_buffer == 0) {
		m_buffer = (char *)calloc(capacity, 1);
	} else {
		m_buffer = (char *)realloc(m_buffer, capacity);
	}

	return m_capacity;
}

// ghostdocument.cpp:698
void memory_buffer::encode(void const *data, unsigned int size) {
	if (m_length + size > m_capacity) {
		for (m_capacity = 1; m_capacity < m_length + size; m_capacity <<= 1) {
		}
		m_buffer = (char *)(m_buffer == 0 ? calloc(m_capacity, 1) : realloc(m_buffer, m_capacity));
	}

	memcpy(m_buffer + m_length, data, size);
	m_length += size;
}

// ghostdocument.cpp:708
void memory_buffer::decode(void *data, unsigned int size) {
	memcpy(data, m_buffer + m_position, size);
	m_position += size;
}

// ghostdocument.cpp:714
void const *memory_buffer::get_buffer(void) const { return m_buffer; }

// ghostdocument.cpp:715
unsigned int memory_buffer::length(void) const { return m_length; }

// ghostdocument.cpp:835
bool CGhostDocument::Import(const std::string &fileName) {
	cFile *pFile = GetCFile(fileName.c_str(), -1, 0);
	if (pFile == NULL)
		return false;

	unsigned int size = pFile->Length();
	unsigned char *pData = new unsigned char[size];
	pFile->Read(pData, size);
	CloseCFile(pFile);

	bool result = Import(pData, size);
	delete[] pData;

	return result;
}

// ghostdocument.cpp:940
bool CGhostDocument::WriteExceptionLogFile(void) const {
	return true;
}
