#ifndef CREATUREENUMS_H
#define CREATUREENUMS_H

enum e_creature_type
{
	CT_NPC,
	CT_PLAYER,
	CT_GM
};

enum e_creature_reaction : int32
{
	Friendly = 1

};

enum e_creature_action_state :int8
{
	STATE_NONCOMBAT = 0,
	STATE_COMBAT = 1,
	STATE_MOUNT = 3,
};

enum e_creature_status : uint8
{
	Dead = 0,
	Alive = 1,
	Ghost = 3 //admin only
};

enum e_creature_relations : uint8
{
	R_NONE,

	R_Ally,
	R_Enemy,
	R_Pvp,
	R_Npc,
	R_PartyMember,
	R_GuildMember,
};


#endif