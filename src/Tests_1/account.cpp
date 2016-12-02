#include "account.h"
#include "connexion.h"
#include "player.h"
#include "Stream.h"
#include "dataservice.h"
#include "config.h"

#include <string>


bool account_perform_login(connection * c, sql::Connection * conn, byte ticket[33], byte username_cstr[SC_PLAYER_NAME_MAX_LENGTH])
{
	if (!c || !conn || !username_cstr)
		return false;

	sql::PreparedStatement *p = conn->prepareStatement("SELECT * FROM accounts WHERE username=? AND password=?");
	if (!p)
		return false;

	p->setString(1, (const char*)username_cstr);
	p->setString(2, (const char*)ticket);

	sql::ResultSet * rs;
	try
	{
		rs = p->executeQuery();
	}
	catch (sql::SQLException & e)
	{
		printf("::SQL-EX::LN[%d] FN[%s] EX[%s]\n", __LINE__, __FUNCTION__, e.what());

		delete p;
		return false;
	}
	if (!rs)
	{
		delete p;
		return false;
	}

	if (rs->next())
	{
		try
		{
			c->_account.isGm = rs->getBoolean("isGm");
			c->_account.lastOnlineUTC = rs->getInt64("lastOnlineUtc");
			c->_account.remainingPlayTime = rs->getInt64("remainingPlayTime");
			c->_account.id = rs->getInt("id");
			c->_account.coins = rs->getInt("coins");
		}
		catch (sql::SQLException & e)
		{
			printf("::SQL-EX::LN[%d] FN[%s] EX[%s]\n", __LINE__, __FUNCTION__, e.what());

			delete rs;
			delete p;
			return false;
		}

		memcpy(c->_account.username, username_cstr, 32);
		memcpy(c->_account.password, ticket, 33);

		delete rs;
		rs = NULL;

		p->close();
		delete p;
		p = NULL;


		p = conn->prepareStatement("SELECT * FROM players WHERE username=?");
		p->setString(1, (const char*)username_cstr);

		sql::ResultSet * rs;
		try
		{
			rs = p->executeQuery();
		}
		catch (sql::SQLException & e)
		{
			printf("::SQL-EX::LN[%d] FN[%s] EX[%s]\n", __LINE__, __FUNCTION__, e.what());

			delete p;
			return false;
		}
		delete p;

		//load players
		for (uint16 i = 0; i < SC_PLAYER_MAX_CHARACTER_COUNT; i++)
		{
			if (rs && rs->next())
			{
				player * newPlayer = new player();


				newPlayer->lastOnlineUTC = rs->getInt64("lastOnlineUTC");
				newPlayer->banTimeUTC = rs->getInt64("banTimeUTC");
				newPlayer->creationTimeUTC = rs->getInt64("creationTimeUTC");

				std::string name = rs->getString("name").c_str();
				size_t nameLen = strlen(name.c_str());
				if (nameLen > SC_PLAYER_NAME_MAX_LENGTH)
					nameLen = SC_PLAYER_NAME_MAX_LENGTH;

				if (!memcpy(newPlayer->name, name.c_str(), nameLen + 1))
				{
					delete newPlayer;
					continue;
				}


				newPlayer->exp = rs->getInt64("exp");
				newPlayer->restedExp = rs->getInt64("restedExp");

				newPlayer->pClass = (e_player_class)rs->getInt("class");
				newPlayer->pRace = (e_player_race)rs->getInt("race");
				newPlayer->pGender = (e_player_gender)rs->getInt("gender");

				player_calculate_model(newPlayer);

				newPlayer->level = (uint32)rs->getInt("level");

				newPlayer->position.coords[0] = (float)rs->getDouble("x");
				newPlayer->position.coords[1] = (float)rs->getDouble("y");
				newPlayer->position.coords[2] = (float)rs->getDouble("z");
				newPlayer->position.heading = (int16)rs->getInt("h");

				newPlayer->position.areaId = (uint32)rs->getInt("areaId");
				newPlayer->position.worldMapGuardId = (uint32)rs->getInt(21);
				newPlayer->position.worldMapWorldId = (uint32)rs->getInt(22);
				newPlayer->position.worldMapSectionId = (uint32)rs->getInt(23);

				//visited sections

				std::istream * details = NULL;

				details = rs->getBlob("details1");
				if (details)
				{
					details->read((char*)newPlayer->details1, SC_PLAYER_DETAILS_1_BUFFER_SIZE);

					delete details;
					details = NULL;
				}

				details = rs->getBlob("details2");
				if (details)
				{
					details->read((char*)newPlayer->details2, SC_PLAYER_DETAILS_2_BUFFER_SIZE);

					delete details;
					details = NULL;
				}

				details = rs->getBlob("details3");
				if (details)
				{
					details->read((char*)newPlayer->details3, SC_PLAYER_DETAILS_3_BUFFER_SIZE);

					delete details;
					details = NULL;
				}

#pragma region inventory
				p = conn->prepareStatement("SELECT * FROM player_inventory WHERE username=? AND name=?");
				p->setString(1, c->_account.username);
				p->setString(2, newPlayer->name);
				sql::ResultSet *rs2 = NULL;
				try
				{
					rs2 = p->executeQuery();
				}
				catch (sql::SQLException & e)
				{
					printf("::SQL-EX::LN[%d] FN[%s] EX[%s]\n", __LINE__, __FUNCTION__, e.what());

					p->close();
					delete p;
					return false;
				}

				if (rs2 && rs2->next())
				{
					inventory_init(newPlayer->_inventory, (uint16)rs2->getInt(4));
					std::istream * inventory_blob = rs2->getBlob(3);
					if (inventory_blob)
					{
						Stream items = Stream(inventory_blob);

						uint32 itemsCount = items.ReadInt32();
						newPlayer->_inventory.itemCount = itemsCount;
						if (itemsCount <= 72 + 20)
						{
							for (uint32 j = 0; j < itemsCount; j++)
							{
								slot_id s_id = items.ReadInt32();
								item_id i_id = items.ReadInt32();

								const item_template * i = data_service::data_resolve_item(i_id);
								if (!i)
									continue;

								newPlayer->_inventory[s_id]._item = new item;
								newPlayer->_inventory[s_id].isEmpty = 0;
								newPlayer->_inventory[s_id]._item->item_t = i;

								newPlayer->_inventory[s_id]._item->hasCrystals = items.ReadByte();
								for (byte j = 0; j < newPlayer->_inventory[s_id]._item->hasCrystals; j++)
									newPlayer->_inventory[s_id]._item->crystals[j] = items.ReadInt32();

								newPlayer->_inventory[s_id]._item->itemLevel = items.ReadByte();
								newPlayer->_inventory[s_id]._item->isMasterworked = items.ReadByte();
								newPlayer->_inventory[s_id]._item->masterworkRate = items.ReadFloat();
								newPlayer->_inventory[s_id]._item->isAwakened = items.ReadByte();
								newPlayer->_inventory[s_id]._item->isEnigmatic = items.ReadByte();
								newPlayer->_inventory[s_id]._item->enchantLevel = items.ReadByte();
								newPlayer->_inventory[s_id]._item->isBinded = items.ReadByte();
								newPlayer->_inventory[s_id]._item->binderEntityId = items.ReadInt32();
								newPlayer->_inventory[s_id]._item->isCrafted = items.ReadByte();
								newPlayer->_inventory[s_id]._item->crafterEntityId = items.ReadInt32();
								newPlayer->_inventory[s_id]._item->stackCount = items.ReadInt32();

								uint16 passivitiesCount = items.ReadInt16();
								for (uint16 g = 0; g < passivitiesCount; g++)
								{
									const passivity_template  * ps = data_service::data_resolve_passivity(items.ReadInt32());
									if (ps)
										newPlayer->_inventory[s_id]._item->passivities.push_back(ps);
								}

							}
						}
						items.Clear();
						delete inventory_blob;
					}

					delete rs2;
				}
				else
				{
					inventory_init(newPlayer->_inventory, config::player.start_inventory_slot_count);
				}
				delete p;
#pragma endregion

				//bank
				//skills
				//settings

				c->_players[i] = newPlayer;
			}
			else
				break;
		}

	}
	else
	{
		delete rs;
		delete p;
		return false;
	}

	return true;
}

bool account_upadate(connection * c, sql::Connection * con)
{

	return false;
}

