#ifndef EQUIPMENTDATATEMPLATE_H
#define EQUIPMENTDATATEMPLATE_H

#include "typeDefs.h"

struct passivity_template;

struct equipment_data
{
	uint32
		minAttack,
		maxAttack,
		defense,
		impact,
		balance,
		countOfSlot,
		id;

	const passivity_template * passivityG;

};

#endif
