#ifndef SPAWN_H
#define SPAWN_H

#include "typeDefs.h"

#include "linkList.h"

class player; class npc;


class spawn
{
public:
	spawn();
	virtual ~spawn();
	virtual void release();

	virtual bool inside(player *);
	virtual bool inside(npc*);

	virtual bool add(player *);
	virtual bool add(npc*);

	virtual bool remove(player*);
	virtual bool remove(npc*);

	link_list<npc *> * npcList[2];			   //linked-list
	link_list<player *>* playerList[2];		   //linked-list

protected:
	//need mutex?
};

#endif 


