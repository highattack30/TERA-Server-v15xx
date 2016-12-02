#include "spawn.h"



spawn::spawn()
{
	playerList[0] = playerList[1] = nullptr;
	npcList[0] = npcList[1] = nullptr;
}

spawn::~spawn()
{
	release();
}


void spawn::release()
{
	while (npcList[0])
	{
		link_list<npc *>* temp = npcList[0];
		npcList[0] = temp->next;
		delete temp;
	}
	npcList[0] = npcList[1] = nullptr;

	while (playerList[0])
	{
		link_list<player *>* temp = playerList[0];
		playerList[0] = temp->next;
		delete temp;
	}
	playerList[0] = playerList[1] = nullptr;
}

bool spawn::inside(player * p)
{
	link_list<player *> * temp = playerList[0];
	while (temp)
	{
		if (temp->_val == p)
			return true;
		temp = temp->next;
	}

	return false;
}

bool spawn::inside(npc * n)
{
	link_list<npc *> * temp = npcList[0];
	while (temp)
	{
		if (temp->_val == n)
			return true;

		temp = temp->next;
	}

	return false;
}

bool spawn::add(player *)
{
	return false;
}

bool spawn::add(npc *)
{
	return false;
}

bool spawn::remove(player * p)
{
	link_list<player *> * temp = playerList[0];
	while (temp && temp->_val != p)
		temp = temp->next;

	if (temp)
	{
		temp->remove();
		delete temp;

		return true;
	}
	
	return false;
}

bool spawn::remove(npc * n)
{
	link_list<npc *> * temp = npcList[0];
	while (temp && temp->_val != n)
		temp = temp->next;

	if (temp)
	{
		temp->remove();
		delete temp;

		return true;
	}

	return false;
}

