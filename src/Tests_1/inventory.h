#ifndef INVENTORY_H
#define INVENTORY_H

#include "typeDefs.h"
#include "socket.h"

#include <vector>



class connection;
class player;
class Stream;

struct item_template;
struct passivity_template;

struct item
{
	item_eid eid;
	
	uint32
		stackCount,
		crystals[4],
		binderEntityId,
		crafterEntityId;


	byte
		hasCrystals,
		isBinded,
		enchantLevel,
		isMasterworked,
		isEnigmatic,
		isAwakened,
		itemLevel,
		isCrafted;

	float
		masterworkRate;
	std::vector<const passivity_template *> passivities;


	const item_template* item_t;
};

struct inventory_slot
{
	inventory_slot(slot_id);
	slot_id
		id;
	byte
		isEmpty;
	item * _item;
};

class inventory
{
public:
	inventory();
	~inventory();


	bool insert_or_stack_item(item_id, uint32 stack_count);
	uint32 pull_item_stack(item_id, uint32 stack_count);
	
	bool equipe_item(slot_id);
	bool unequipe_item(slot_id);

	void send(connection*, byte show = 0);
	void send_item_levels(connection*);

	item * get_item_by_eid(item_eid, byte remove = 0, uint32 stack_count = 1);
	item * get_item_by_id(item_id, byte remove = 0, uint32 stack_count = 1);

	//bool operator <<(item_id);
	//bool operator <<(item*);

	inventory_slot& operator[](slot_id);

	void clear();

	bool is_full();
	slot_id get_item_slot_id(item_eid, byte offseted = 1);

	item_id get_item(slot_id id);
	uint8  get_enchant_leve(slot_id id);

	Stream * get_raw();

	uint16
		slotCount,
		equipedItemsCount,
		itemCount;

	uint32
		profileItemLevel,
		itemLevel;

	uint64
		gold,
		maxGold;

	std::vector<inventory_slot> slots;
private:
	void recalculate_levels();
	CRITICAL_SECTION inv_lock;

};

bool WINAPI item_copy(_In_ item* src, _In_ item* dest);
uint32 WINAPI item_stack(item * i, uint32 stack_count, byte mode);

bool WINAPI slot_insert(inventory_slot &, item_id, uint32 stack_count);
void WINAPI slot_wipe(inventory_slot &);
void WINAPI slot_clear(inventory_slot&);


item*WINAPI inventory_create_item(item_id);
bool WINAPI inventory_build_item(inventory_slot& slot, item_id);
bool WINAPI inventory_new(player * p);
void WINAPI inventory_init(inventory&, uint16 slot_count);
#endif



