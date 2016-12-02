#ifndef ENCHANTDATATEMPLATE_H
#define ENCHANTDATATEMPLATE_H

#include <vector>

#include "typeDefs.h"
#include "equipmentEnums.h"

struct passivity_category_template;



typedef struct enchant_effect
{
	uint16 step;
	 passivity_category *  passivitiesCategory;
}enchant_effect , *lpenchant_effect;

typedef struct basic_stat_enchant
{
	float rate;
	short enchantStep;
	e_basic_stat_enchant_kind kind;
}basic_stat_enchant, *lpbasic_stat_enchant;



typedef struct enchant_set
{
	int32 id;

	std::vector<enchant_effect> effects;
	std::vector<basic_stat_enchant > stats;

} enchant_set ,*lpenchant_set;

#endif
