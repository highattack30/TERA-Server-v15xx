#ifndef CREATURE_H
#define CREATURE_H

#include "typeDefs.h"
#include "creatureEnums.h"
#include "worldposition.h"

class creature
{
public:
	creature(e_creature_type type);
	virtual ~creature();

	uint32 level;
	world_position position;

protected:
	e_creature_type type;
};



#endif