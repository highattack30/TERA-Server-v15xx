#include "inventory.h"
#include "itemtemplate.h"
#include "passivitytemplate.h"
#include "dataservice.h"
#include "equipmentdatatemplate.h"
#include "player.h"
#include "Stream.h"
#include "connexion.h"

inventory::inventory()
{
	InitializeCriticalSection(&inv_lock);

	slotCount = equipedItemsCount = itemCount = 0;
	profileItemLevel = itemLevel = 0;
	gold = maxGold = 0;
}

inventory::~inventory()
{
	DeleteCriticalSection(&inv_lock);


	for (uint16 i = 0; i < slots.size(); i++)
	{
		slot_wipe(slots[i]);
	}
	slots.clear();
}

bool inventory::insert_or_stack_item(item_id id, uint32 stack_count)
{
	uint32 residual = stack_count; bool result = false;
	EnterCriticalSection(&inv_lock);

	for (size_t i = 20; i < slots.size(); i++)
		if (!slots[i].isEmpty && slots[i]._item &&
			slots[i]._item->item_t->id == id)
			if (!(residual = item_stack(slots[i]._item, stack_count, STACK_FRAGMENT)))
			{
				result = true;
				break;
			}


	if (residual)
		for (size_t i = 20; i < slots.size(); i++)
			if (slots[i].isEmpty && slot_insert(slots[i], id, residual))
			{
				itemCount++;
				result = true;
				break;
			}


	if (result)
		recalculate_levels();

	LeaveCriticalSection(&inv_lock);
	return result;
}

uint32 inventory::pull_item_stack(item_id id, uint32 stack_count)
{
	EnterCriticalSection(&inv_lock);
	for (size_t i = 20; i < slots.size(); i++)
	{
		if (!slots[i].isEmpty && slots[i]._item &&
			slots[i]._item->item_t->id == id && slots[i]._item->stackCount >= stack_count)
		{
			slots[i]._item->stackCount -= stack_count;
			if (slots[i]._item->stackCount <= 0)
				slot_wipe(slots[i]);

			itemCount--;
			recalculate_levels();

			LeaveCriticalSection(&inv_lock);
			return stack_count;
		}

	}

	LeaveCriticalSection(&inv_lock);
	return 0;
}

bool inventory::equipe_item(slot_id)
{
	EnterCriticalSection(&inv_lock);

	//todo

	LeaveCriticalSection(&inv_lock);

	return true;
}

bool inventory::unequipe_item(slot_id)
{
	EnterCriticalSection(&inv_lock);

	//todo
	LeaveCriticalSection(&inv_lock);

	return true;
}

void inventory::send(connection* c, byte show /*= 0*/)
{
	//todo
}

void inventory::send_item_levels(connection*)
{
	//todo
}

item * inventory::get_item_by_eid(item_eid eid, byte remove /*= 0*/, uint32 stack_count /*= 1*/)
{
	EnterCriticalSection(&inv_lock);

	for (size_t i = 0; i < slots.size(); i++)
	{
		if (slots[i].isEmpty == 0 && slots[i]._item->eid == eid)
			if (remove && !stack_count)
			{
				item *  toReturn = slots[i]._item;
				slots[i]._item = nullptr;
				slots[i].isEmpty = 1;
				itemCount--;

				LeaveCriticalSection(&inv_lock);
				return toReturn;
			}
			else
			{
				if (remove && (stack_count > 0 && stack_count <= slots[i]._item->stackCount))
				{
					item * out = new item;
					memset(out, 0, sizeof item);

					if (!item_copy(slots[i]._item, out))
					{
						delete out;
						out = nullptr;
						LeaveCriticalSection(&inv_lock);
						return out;
					}
					else
					{
						out->stackCount = stack_count;
						slots[i]._item->stackCount -= stack_count;
						if (slots[i]._item->stackCount <= 0)
							slot_clear(slots[i]);

						LeaveCriticalSection(&inv_lock);
						return out;
					}
				}
				else if (!remove)
				{
					LeaveCriticalSection(&inv_lock);
					return slots[i]._item;
				}
			}

	}

	LeaveCriticalSection(&inv_lock);
	return nullptr;
}

item * inventory::get_item_by_id(item_id id, byte remove /*= 0*/, uint32 stack_count /*= 1*/)
{
	EnterCriticalSection(&inv_lock);

	for (size_t i = 0; i < slots.size(); i++)
	{
		if (slots[i].isEmpty == 0 && slots[i]._item->item_t->id == id)
			if (remove && !stack_count)
			{
				item *  toReturn = slots[i]._item;
				slots[i]._item = nullptr;
				slots[i].isEmpty = 1;
				itemCount--;

				LeaveCriticalSection(&inv_lock);
				return toReturn;
			}
			else
			{
				if (remove && (stack_count > 0 && stack_count <= slots[i]._item->stackCount))
				{
					item * out = new item;
					memset(out, 0, sizeof item);

					if (!item_copy(slots[i]._item, out))
					{
						delete out;
						out = nullptr;
						LeaveCriticalSection(&inv_lock);
						return out;
					}
					else
					{
						out->stackCount = stack_count;
						slots[i]._item->stackCount -= stack_count;
						if (slots[i]._item->stackCount <= 0)
							slot_clear(slots[i]);

						LeaveCriticalSection(&inv_lock);
						return out;
					}
				}
				else if (!remove)
				{
					LeaveCriticalSection(&inv_lock);
					return slots[i]._item;
				}
			}

	}

	LeaveCriticalSection(&inv_lock);
	return nullptr;
}

inventory_slot& inventory::operator[](slot_id id)
{
	EnterCriticalSection(&inv_lock);
	if (id >= 0 && id < slots.size())
	{
		LeaveCriticalSection(&inv_lock);
		return slots[id];
	}

	LeaveCriticalSection(&inv_lock);

	return inventory_slot(0xffff);
}

void inventory::clear()
{
	EnterCriticalSection(&inv_lock);
	for (uint16 i = 0; i < slots.size(); i++)
	{
		slot_wipe(slots[i]);
	}

	equipedItemsCount = itemCount = 0;
	profileItemLevel = itemLevel = 0;
	gold = maxGold = 0;

	LeaveCriticalSection(&inv_lock);
}

bool inventory::is_full()
{
	EnterCriticalSection(&inv_lock);
	for (uint16 i = 0; i < slots.size(); i++)
	{
		if (slots[i].isEmpty)
		{
			LeaveCriticalSection(&inv_lock);
			return false;
		}
	}

	LeaveCriticalSection(&inv_lock);
	return true;
}

slot_id inventory::get_item_slot_id(item_eid eid, byte offseted /*= 1*/)
{
	
	if (eid <= 0)
		return 0xffff;
	EnterCriticalSection(&inv_lock);
	if (offseted)
	{
		for (uint16 i = 20; i < slots.size(); i++)
		{
			if (!slots[i].isEmpty && slots[i]._item->eid == eid)
			{
				LeaveCriticalSection(&inv_lock);
				return slots[i].id;
			}
		}
	}
	else
	{
		for (uint16 i = 0; i < slots.size(); i++)
		{
			if (!slots[i].isEmpty && slots[i]._item->eid == eid)
			{
				LeaveCriticalSection(&inv_lock);
				return slots[i].id;
			}
		}
	}

	LeaveCriticalSection(&inv_lock);
	return 0xffff;
}





item_id inventory::get_item(slot_id id)
{
	if (id < 0 || id >= slotCount)
		return 0;

	if (!slots[id].isEmpty && slots[id]._item && slots[id]._item->item_t)
		return slots[id]._item->item_t->id;
	return 0;
}

uint8 inventory::get_enchant_leve(slot_id id)
{
	if (id < 0 || id >= slotCount)
		return 0;

	if (!slots[id].isEmpty && slots[id]._item)
		return slots[id]._item->enchantLevel;
	return 0;
}

Stream * inventory::get_raw()
{
	Stream* out = new Stream();
	EnterCriticalSection(&inv_lock);
	out->WriteInt32(itemCount); //count;
	int count = 0;
	for (size_t i = 0; i < slots.size(); i++)
	{
		if (!slots[i].isEmpty)
		{
			count++;
			out->WriteInt32(slots[i].id);
			item * info = slots[i]._item;
			out->WriteInt32(info->item_t->id);

			out->WriteByte(info->hasCrystals);
			for (byte j = 0; j < info->hasCrystals; j++)
				out->WriteInt32(info->crystals[j]);

			out->WriteByte(info->itemLevel);
			out->WriteByte(info->isMasterworked);
			out->WriteFloat(info->masterworkRate);
			out->WriteByte(info->isAwakened);
			out->WriteByte(info->isEnigmatic);
			out->WriteByte(info->enchantLevel);
			out->WriteByte(info->isBinded);
			out->WriteInt32(info->binderEntityId);
			out->WriteByte(info->isCrafted);
			out->WriteInt32(info->crafterEntityId);
			out->WriteInt32(info->stackCount);

			short passCount = out->NextPos();
			short pCount = 0;
			for (size_t j = 0; j < info->passivities.size(); j++)
			{
				if (info->passivities[j])
				{
					out->WriteInt32(info->passivities[j]->id);
					pCount++;
				}
				else
					out->WriteInt32(0);
			}
			out->_pos = passCount;
			out->WriteInt16(pCount);
			out->SetEnd();


		}
	}

	out->SetFront();
	out->WriteInt32(count);
	out->SetEnd();

	LeaveCriticalSection(&inv_lock);
	return out;
}

void inventory::recalculate_levels()
{
	//todo
}

bool WINAPI item_copy(_In_ item* src, _In_ item* dest)
{
	return memcpy(dest, src, sizeof item) ? true : false;
}

uint32 WINAPI item_stack(item * i, uint32 stack_count, byte mode)
{
	uint32 out = 0;

	if ((stack_count + i->stackCount) > i->item_t->maxStack)
	{
		if (!mode)
		{
			out = stack_count - i->item_t->maxStack + i->stackCount;
			i->stackCount = i->item_t->maxStack;
		}
		else
			out = stack_count;
	}
	else
	{
		i->stackCount += stack_count;
	}

	return out;
}

bool WINAPI slot_insert(inventory_slot & j, item_id id, uint32 stack_count)
{
	if (!j.isEmpty)
		return false;

	if (inventory_build_item(j, id))
	{
		j._item->stackCount = stack_count;
		return true;
	}

	slot_clear(j);
	return false;
}

void WINAPI slot_wipe(inventory_slot &s)
{
	if (s._item)
	{
		delete s._item;
		s._item = NULL;
	}
	s.isEmpty = 1;
}

void WINAPI slot_clear(inventory_slot& s)
{
	s.isEmpty = 1;
	s._item = nullptr;
}



item * WINAPI inventory_create_item(item_id id)
{
	const item_template * t = data_service::data_resolve_item(id);
	if (!t)
		return NULL;

	item *i = new item;
	memset(i, 0, sizeof item);
	i->item_t = t;
	i->stackCount = 1;
	i->itemLevel = 1;

	if (t->category == twohand || t->category == axe || t->category == circle || t->category == bow || t->category == staff || t->category == rod || t->category == lance ||
		t->category == dual || t->category == blaster || t->category == gauntlet || t->category == shuriken ||

		t->category == bodyMail || t->category == handMail || t->category == feetMail ||
		t->category == bodyLeather || t->category == handLeather || t->category == feetLeather ||
		t->category == bodyRobe || t->category == handRobe || t->category == feetRobe)
	{
		switch (t->type)
		{

		case EQUIP_WEAPON:
		{



		}break;
		case EQUIP_ARMOR_BODY:
		{



		}break;
		case EQUIP_ARMOR_LEG:
			break;
		case EQUIP_ARMOR_ARM:
			break;
		}
	}
	return i;
}

bool WINAPI inventory_build_item(inventory_slot& slot, item_id id)
{
	if (id <= 0 || !slot.isEmpty)
		return false;

	if (!(slot._item = inventory_create_item(id)))
		return false;
	slot.isEmpty = 0;

	item * i = slot._item;
	const item_template * t = i->item_t;

	if (t->equipmentData && t->equipmentData->passivityG)
		i->passivities.push_back(t->equipmentData->passivityG);

	if (t->enchantEnable && t->enchant)
	{
		//EquipmentService::RollEnchantPassivities(item->_enchant, slot->_info->_passivities); //todo
	}

	return true;
}

bool WINAPI inventory_new(player * p)
{
	if (!p)
		return false;
	e_player_class pClass = p->pClass;

	inventory * inv = &p->_inventory;
	

	if (pClass == WARRIOR || pClass == ARCHER || pClass == SLAYER || pClass == REAPER)
	{
		if (inventory_build_item((*inv)[PROFILE_ARMOR], 15004)) inv->itemCount++;
		if (inventory_build_item((*inv)[PROFILE_GLOVES], 15005)) inv->itemCount++;
		if (inventory_build_item((*inv)[PROFILE_BOOTS], 15006)) inv->itemCount++;
	}
	else if (pClass == BERSERKER || pClass == LANCER || pClass == ENGINEER || pClass == FIGHTER)
	{
		if (inventory_build_item((*inv)[PROFILE_ARMOR], 15001)) inv->itemCount++;
		if (inventory_build_item((*inv)[PROFILE_GLOVES], 15002)) inv->itemCount++;
		if (inventory_build_item((*inv)[PROFILE_BOOTS], 15003)) inv->itemCount++;
	}
	else if (pClass == SORCERER || pClass == PRIEST || pClass == MYSTIC || pClass == ASSASSIN)
	{
		if (inventory_build_item((*inv)[PROFILE_ARMOR], 15007)) inv->itemCount++;
		if (inventory_build_item((*inv)[PROFILE_GLOVES], 15008)) inv->itemCount++;
		if (inventory_build_item((*inv)[PROFILE_BOOTS], 15009)) inv->itemCount++;
	}

	switch (pClass)
	{
	case WARRIOR:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 10001)) inv->itemCount++;
		break;
	case BERSERKER:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 10004)) inv->itemCount++;
		break;
	case FIGHTER:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 82005)) inv->itemCount++;
		break;
	case ENGINEER:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 55005)) inv->itemCount++;
		break;
	case PRIEST:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 10007)) inv->itemCount++;
		break;
	case MYSTIC:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 10008)) inv->itemCount++;
		break;
	case ASSASSIN:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 58171)) inv->itemCount++;
		break;
	case LANCER:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 10002)) inv->itemCount++;
		break;
	case SLAYER:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 10003)) inv->itemCount++;
		break;
	case SORCERER:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 10005)) inv->itemCount++;
		break;
	case ARCHER:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 10006)) inv->itemCount++;
		break;
	case REAPER:
		if (inventory_build_item((*inv)[PROFILE_WEAPON], 80396)) inv->itemCount++;
		break;
	}

	//insert additional items here TODO

	return true;
}

void WINAPI inventory_init(inventory& inv, uint16 slot_count)
{
	for (uint16 i = 0; i < 20; i++)
	{
		inv.slots.push_back(inventory_slot(i));
	}

	for (uint16 i = 0; i < slot_count; i++)
	{
		inv.slots.push_back(inventory_slot(i + 40));
	}

	inv.slotCount = slot_count + 20;
	return;
}


inventory_slot::inventory_slot(slot_id i)
{
	id = i;
	isEmpty = 1;
	_item = nullptr;
}
