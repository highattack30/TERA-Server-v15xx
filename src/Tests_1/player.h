#ifndef PLAYER_H
#define PLAYER_H

#include "serverEnums.h"
#include "typeDefs.h"
#include "spawn.h"
#include "playerEnums.h"
#include "typeDefs.h"
#include "inventory.h"

#include "creature.h"

class connection; 

class player : public creature
{
public:
	player();

	char name[SC_PLAYER_NAME_MAX_LENGTH];

	ulong
		model,

		settingsSize;

	uint64
		exp,
		restedExp,
		maxExp,

		banTimeUTC,
		creationTimeUTC,
		lastOnlineUTC;


	byte
		details1[SC_PLAYER_DETAILS_1_BUFFER_SIZE],
		details2[SC_PLAYER_DETAILS_2_BUFFER_SIZE],
		details3[SC_PLAYER_DETAILS_3_BUFFER_SIZE];

	spawn _spawn;
	inventory _inventory;

	e_player_class		pClass;
	e_player_race		pRace;
	e_player_gender		pGender;
};

bool WINAPI player_calculate_model(player * p);


#endif
