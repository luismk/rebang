// Original: d:\build\custom\temp\code\shared\tikimagicboxtable.h
#ifndef PG_SHARED_TIKIMAGICBOXTABLE_H
#define PG_SHARED_TIKIMAGICBOXTABLE_H

#include <map>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"

// The Tiki magic box IFF records (IFF_STRUCT, classdefine.h).
namespace IFF_STRUCT {
struct sTikiOutputTable;
struct sTikiPointTable;
struct sTikiSpecialRecipe;
}

// One material of a mix (and the mix's output): GetOutput walks the
// caller's array with a 0xC stride, sorts the type ids as unsigned and
// fills the output with the item and its count.
struct sTikiMagicBoxMtr {
	unsigned long m_typeId;             // 0x0
	unsigned long m_itemId;             // 0x4, the inventory item; 0 on the output (a new item)
	int m_count;                        // 0x8, how many (times the item's or part's m_tikiPoint)
};
CHECK_SIZE(sTikiMagicBoxMtr, 0xC);
CHECK_FIELD(sTikiMagicBoxMtr, m_typeId, 0x0);
CHECK_FIELD(sTikiMagicBoxMtr, m_itemId, 0x4);
CHECK_FIELD(sTikiMagicBoxMtr, m_count, 0x8);

// The magic box tables (shared with the server): the outputs grouped by
// name, the point table and the special recipes, each record keyed by its
// first dword.
class CTikiMagicBoxDoc : public WSingleton<CTikiMagicBoxDoc> {
	LAYOUT_CHECK_FRIEND;

public:
	typedef std::map<unsigned int, IFF_STRUCT::sTikiOutputTable*> OutputTableMap;
	typedef std::map<std::string, OutputTableMap*> OutputGroupMap;
	typedef std::map<unsigned int, IFF_STRUCT::sTikiPointTable*> PointTableMap;
	typedef std::map<unsigned int, IFF_STRUCT::sTikiSpecialRecipe*> SpecialRecipeMap;

	// tikimagicboxtable.h:37
	CTikiMagicBoxDoc(void) {
		CleanUp();
	}
	// tikimagicboxtable.h:38
	virtual ~CTikiMagicBoxDoc(void) {
		CleanUp();
	}

	void InsertOutput(const IFF_STRUCT::sTikiOutputTable& table);
	void InsertPointTable(const IFF_STRUCT::sTikiPointTable& table);
	void InsertSpecialRecipe(const IFF_STRUCT::sTikiSpecialRecipe& recipe);
	bool GetOutput(int count, const sTikiMagicBoxMtr* const mtrs, sTikiMagicBoxMtr& output);
	void CleanUp(void);

private:
	OutputGroupMap m_outputGroups;          // 0x28
	PointTableMap m_pointTables;            // 0x34
	SpecialRecipeMap m_specialRecipes;      // 0x40
};
CHECK_SIZE(CTikiMagicBoxDoc, 0x4c);
BEGIN_LAYOUT_CHECK(CTikiMagicBoxDoc)
	CHECK_FIELD(CTikiMagicBoxDoc, m_outputGroups, 0x28);
	CHECK_FIELD(CTikiMagicBoxDoc, m_pointTables, 0x34);
	CHECK_FIELD(CTikiMagicBoxDoc, m_specialRecipes, 0x40);
END_LAYOUT_CHECK

#endif // PG_SHARED_TIKIMAGICBOXTABLE_H
