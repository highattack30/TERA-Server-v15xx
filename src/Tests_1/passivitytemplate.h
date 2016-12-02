#ifndef PASSIVITYTEMEPLATE_H
#define PASSIVITYTEMEPLATE_H

#include "typeDefs.h"
#include "playerEnums.h"

typedef struct passivity_template
{
	e_mob_size mobSize;

	int32
		conditionValue,
		conditionCategory,
		abnormalityKind,
		abnormalityCategory,
		condition,
		tickInterval,
		method,
		kind,
		id,
		prevPassivityId,
		passivityChangeId,
		passivityChangeTime,
		type;

	float
		value,
		probability;

	byte
		judgmentOnce,
		balancedByTargetCount;

}passivity_template, *lppassivity_template;

#endif