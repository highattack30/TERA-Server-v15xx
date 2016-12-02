#ifndef CUSTOMIZINGITEMTEMPLATE_H
#define CUSTOMIZINGITEMTEMPLATE_H

#include "typeDefs.h"

#include <vector>

struct passivity_template;

typedef struct cusotmizing_item
{
	int32 id;
	uint32 takeSlot;
	float destroyProbOnDeath;
	std::vector<const passivity_template *> passivities;

}cusotmizing_item, *lpcusotmizing_item;

#endif
