#ifndef ABNORMALITYTEMPLATE_H
#define ABNORMALITYTEMPLATE_H

#include <vector>

#include "typeDefs.h"

typedef  struct abnormality_effect_template
{
	float
		value;

	int32 
		type,
		tickInterval,
		overlayEffectId,
		isEnemyCheck,
		appearEffectId,
		attackEffectId,
		damageEffectId,
		dissappearEffectId,
		method;
}abnormality_effect , *lpabnormality_effect;


typedef struct abnormality_template
{
	std::vector<abnormality_effect> 
		effects;

	int32 
		id,
		kind,
		level,
		priority,
		property,
		time,
		bySkillCategory;

	std::string
		toolTip,
		name;

	bool
		notCareDeth,
		infinity,
		isBuff,
		isShow,
		isHideOnRefresh;
}abnormality_template , *lpabnormality_template;

#endif
