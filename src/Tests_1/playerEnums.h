#ifndef PLAYERENUMS_H
#define PLAYERENUMS_H

#include "typeDefs.h"

enum e_player_class : uint16
{
	WARRIOR = 0,
	LANCER = 1,
	SLAYER = 2,
	BERSERKER = 3,
	SORCERER = 4,
	ARCHER = 5,
	PRIEST = 6,
	MYSTIC = 7,
	REAPER = 8,
	ENGINEER = 9,//gunner
	FIGHTER = 10, //brawler
	ASSASSIN = 11 //ninja
};

enum e_player_gender :uint16
{
	MALE = 0,
	FEMALE = 1
};

enum e_player_race : uint16
{
	HUMAN = 0,
	HIGHELF = 1,
	AMAN = 2,
	CASTANIC = 3,
	ELIN = 4,
	POPORI = 5,
	BARAKA = 6,

};

enum e_mob_size : uint16
{
	Small = 1,
	Medium = 2,
	Large = 3,
	All = 4,
	TPlayer = 5
};

enum e_player_move_type : uint16
{
	P_MOVE_NORMAL = 0,
	P_JUMP_START = 1,
	P_JUMP_END = 2
};


#endif // 
