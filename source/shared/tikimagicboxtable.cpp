// Original: d:\build\custom\temp\code\shared\tikimagicboxtable.cpp
#include "TikiMagicBoxTable.h"

#include <stdlib.h>
#include <string.h>
#include <deque>
#include <new>

#include "classdefine.h"
#include "../client/ProjectG/SharedDoc.h"

// tikimagicboxtable.cpp:20
void CTikiMagicBoxDoc::InsertOutput(const IFF_STRUCT::sTikiOutputTable& table)
{
	IFF_STRUCT::sTikiOutputTable *output = new IFF_STRUCT::sTikiOutputTable;
	if (output == NULL)
		return;

	memset(output, 0, sizeof(IFF_STRUCT::sTikiOutputTable)); *output = table;

	OutputGroupMap::iterator group = m_outputGroups.find(output->m_name);

	if (group == m_outputGroups.end()) {
		OutputTableMap *outputs = new OutputTableMap;
		if (outputs != NULL)
			m_outputGroups.insert(std::make_pair(output->m_name, outputs));

		group = m_outputGroups.find(output->m_name);
	}

	if (group != m_outputGroups.end()) {
		OutputTableMap *outputs = group->second;

		if (outputs != NULL)
			outputs->insert(OutputTableMap::value_type(output->m_typeId, output));
	}
}

// tikimagicboxtable.cpp:55
void CTikiMagicBoxDoc::InsertPointTable(const IFF_STRUCT::sTikiPointTable& table)
{
	IFF_STRUCT::sTikiPointTable *point = new IFF_STRUCT::sTikiPointTable;
	if (point == NULL)
		return;

	memset(point, 0, sizeof(IFF_STRUCT::sTikiPointTable)); *point = table;

	m_pointTables.insert(PointTableMap::value_type(point->m_typeId, point));
}

// tikimagicboxtable.cpp:66
void CTikiMagicBoxDoc::InsertSpecialRecipe(const IFF_STRUCT::sTikiSpecialRecipe& recipe)
{
	IFF_STRUCT::sTikiSpecialRecipe *special = new IFF_STRUCT::sTikiSpecialRecipe;
	if (special == NULL)
		return;

	memset(special, 0, sizeof(IFF_STRUCT::sTikiSpecialRecipe)); *special = recipe;

	m_specialRecipes.insert(SpecialRecipeMap::value_type(special->m_typeId, special));
}

// tikimagicboxtable.cpp:77
bool CTikiMagicBoxDoc::GetOutput(int count, const sTikiMagicBoxMtr* const mtrs, sTikiMagicBoxMtr& output)
{
	if (count == 0 || mtrs == NULL)
		return false;

	char name[32] = "\0";

	memset(&output, 0, sizeof(sTikiMagicBoxMtr));

	bool found = false;
	SpecialRecipeMap::iterator recipe = m_specialRecipes.begin(), recipeEnd = m_specialRecipes.end();

	for (; recipe != recipeEnd; ++recipe) {
		IFF_STRUCT::sTikiSpecialRecipe *special = recipe->second;

		if (special == NULL)
			continue;

		if (count != special->m_count)
			continue;

		// [0] the mix's type ids, [1] the recipe's
		unsigned long *typeIds[2] = { NULL, NULL };

		for (int i = 0; i < 2; i++)
			typeIds[i] = new unsigned long[count];

		for (int i = 0; i < count; i++) {
			typeIds[0][i] = mtrs[i].m_typeId;
			typeIds[1][i] = special->m_typeIds[i];
		}

		for (int i = 0; i < count; i++) {
			for (int j = i; j < count; j++) {
				for (int k = 0; k < 2; k++) {
					if (typeIds[k][i] > typeIds[k][j]) {
						unsigned long typeId = typeIds[k][i];
						typeIds[k][i] = typeIds[k][j];
						typeIds[k][j] = typeId;
					}
				}
			}
		}

		std::deque<unsigned long> mix;
		std::deque<unsigned long> recipeIds;

		for (int i = 0; i < count; i++) {
			mix.push_back(typeIds[0][i]);
			recipeIds.push_back(typeIds[1][i]);
		}

		for (int i = 0; i < 2; i++) {
			delete[] typeIds[i];
			typeIds[i] = NULL;
		}

		for (int i = 0; i < count; i++) {
			if (mix.back() == recipeIds.back()) {
				mix.pop_back();
				recipeIds.pop_back();
			}
		}

		if (mix.empty() && recipeIds.empty()) {
			strcpy(name, special->m_name);
			found = true;
			break;
		}
	}

	if (!found) {
		int point = 0;

		for (int i = 0; i < count; i++) {
			switch (mtrs[i].m_typeId >> 26) {
			case 2: {
				IFF_STRUCT::sPart *part = ItemManager()->FindPart(mtrs[i].m_typeId);
				if (part != NULL)
					point += part->m_tikiPoint * mtrs[i].m_count;
				break;
			}
			case 6: {
				IFF_STRUCT::sItem *item = ItemManager()->FindItem(mtrs[i].m_typeId);
				if (item != NULL)
					point += item->m_tikiPoint * mtrs[i].m_count;
				break;
			}
			}
		}

		PointTableMap::iterator table = m_pointTables.begin(), tableEnd = m_pointTables.end();

		for (; table != tableEnd; ++table) {
			IFF_STRUCT::sTikiPointTable *pointTable = table->second;

			if (pointTable != NULL) {
				if (pointTable->m_minPoint <= point && pointTable->m_maxPoint >= point) {
					// classdefine.h lacks sTikiPointTable::m_name (char[] at 0x5):
					// strcpy(name, pointTable->m_name);
					break;
				}
			}
		}
	}

	if (strlen(name) != 0) {
		OutputGroupMap::iterator group = m_outputGroups.find(name);

		if (group != m_outputGroups.end()) {
			OutputTableMap *outputs = group->second;

			if (outputs != NULL) {
				unsigned int index = rand() % outputs->size();
				IFF_STRUCT::sTikiOutputTable *table = (*outputs)[index];

				if (table != NULL) {
					output.m_typeId = table->m_itemTypeId;
					output.m_count = table->m_itemCount;
					output.m_itemId = 0;
				}
			}
		}
	}

	return false;
}

// tikimagicboxtable.cpp:246
void CTikiMagicBoxDoc::CleanUp(void)
{
	OutputGroupMap::iterator group = m_outputGroups.begin(), groupEnd = m_outputGroups.end();

	for (; group != groupEnd; ++group) {
		OutputTableMap *outputs = group->second;
		if (outputs != NULL) {
			OutputTableMap::iterator output = outputs->begin(), outputEnd = outputs->end();

			for (; output != outputEnd; ++output) {
				IFF_STRUCT::sTikiOutputTable *table = output->second;
				if (table != NULL) {
					delete table;
					output->second = NULL;
				}
			}
			outputs->clear();

			delete outputs;
			group->second = NULL;
		}
	}
	m_outputGroups.clear();

	PointTableMap::iterator point = m_pointTables.begin(), pointEnd = m_pointTables.end();

	for (; point != pointEnd; ++point) {
		IFF_STRUCT::sTikiPointTable *table = point->second;
		if (table != NULL) {
			delete table;
			point->second = NULL;
		}
	}
	m_pointTables.clear();

	SpecialRecipeMap::iterator recipe = m_specialRecipes.begin(), recipeEnd = m_specialRecipes.end();

	for (; recipe != recipeEnd; ++recipe) {
		IFF_STRUCT::sTikiSpecialRecipe *table = recipe->second;
		if (table != NULL) {
			delete table;
			recipe->second = NULL;
		}
	}
	m_specialRecipes.clear();
}
