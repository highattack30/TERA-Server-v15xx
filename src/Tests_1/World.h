#ifndef WORLD_H
#define WORLD_H

#include "linkList.h"
#include "typeDefs.h"

class area; class player;
class world
{
	link_list<area *> g_areas;

};


bool w_enterWorld(world*, player *);
bool w_exitWorld(world*, player *);

#endif
