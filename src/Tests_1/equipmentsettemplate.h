#ifndef EQUIPMENTSETTEMPLATE_H
#define EQUIPMENTSETTEMPLATE_H

#include "typeDefs.h"

#include <vector>

struct item_template;
struct passivity_template;

struct equipment_set_effect
{
	uint32 countOfEquipment;
	std::vector<const  passivity_template  *> passivities;
};

struct equipment_set_item
{
	uint32 slotId;
	std::vector<uint32> *ids;
	std::vector<const item_template * > items;
};

typedef struct equipment_set_data
{
	int32 id;
	std::vector<equipment_set_effect> effects;
	std::vector<equipment_set_item > items;

}equipment_set , *lpequipment_set;

#endif
