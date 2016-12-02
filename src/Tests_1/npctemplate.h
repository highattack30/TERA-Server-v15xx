#ifndef NPCTEMPLATE_H
#define NPCTEMPLATE_H

#include "typeDefs.h"
#include "NpcEnum.h"

#include <string>
#include <vector>

struct npc_name_plate
{
	int
		namePlateHeight,
		deathNamePlateHeight;
};

struct npc_stat
{
	int
		level,
		maxHp;
};

struct npc_alt_animation
{
	e_npc_alt_anim_type type;
	std::string animName;
};


typedef struct npc_template
{
	byte
		isIgnoreSelect,
		pushedByNpc,
		isSpirit,
		villager,
		unionElite,
		isObjectNpc,
		invincible,
		showShorttermTarget,
		showAggroTarget,
		isFreeNamed,
		elite;


	int
		id,
		huntingZoneId,
		spawnScriptId,
		despawnScriptId,
		basicActionId,
		parentId,
		resourceSize;

	float 
		scale;


	npc_name_plate 
		namePlate;
	npc_stat 
		stats;
	std::vector<npc_alt_animation> 
		altAnimations;

	e_mob_size						size;
	e_npc_race						race;
	e_npc_gender					gender;
	e_npc_resource_type				resourceType;


}npc_template , *lpnpc_template;

#endif
