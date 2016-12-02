#include "player.h"
#include "connexion.h"


player::player() : creature(CT_PLAYER)
{
}








bool WINAPI player_calculate_model(player * p)
{
	if (!p)
		return false;

	p->model = 10000;
	p->model += (p->pRace * 2 + 1 + p->pGender) * 100;
	p->model += p->pClass + 1;

	return true;
}
