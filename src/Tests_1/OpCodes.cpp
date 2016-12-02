#include "OpCodes.h"
#include "config.h"

#include "stringUtils.h"
#include "connexion.h"
#include "account.h"
#include "servertime.h"
#include "Stream.h"
#include "server.h"
#include "itemEnums.h"
#include "servertime.h"

#include <fstream>
#include <sstream>

bool opcode_init()
{
	//--------------------------------------------------------------INIT
	opcode_add(C_CHECK_VERSION, op_check_version);
	opcode_add(C_LOGIN_ARBITER, op_login_arbiter);
	opcode_add(C_SET_VISIBLE_RANGE, op_set_visible_range);
	opcode_add(C_GET_USER_LIST, op_get_user_list, false);
	opcode_add(C_HARDWARE_INFO, op_hardware_info);
	opcode_add(C_REQUEST_VIP_SYSTEM_INFO, op_request_vip_system_info, false);
	opcode_add(C_GET_USER_GUILD_LOGO, op_get_user_guild_logo);


	//--------------------------------------------------------------LOBBY
	opcode_add(C_CAN_CREATE_USER, op_can_create_player, false);
	opcode_add(C_CREATE_USER, op_create_player);
	opcode_add(C_CHANGE_USER_LOBBY_SLOT_ID, op_change_user_slot_id);
	opcode_add(C_CHECK_USERNAME, op_check_username);
	opcode_add(C_STR_EVALUATE_LIST, op_str_evaluate_string);
	opcode_add(C_SELECT_USER, op_select_player);
	opcode_add(C_DELETE_USER, op_delete_player);
	//opcode_add(C_CANCEL_DELETE_USER, op_cancel_delete_player);


	return true;
}

void opcode_release()
{
	for (uint32 i = 0; i < opCodeCount; i++)
	{
		if (opCodesArray[i])
		{
			delete opCodesArray[i];
			opCodesArray[i] = nullptr;
		}
	}
	opCodeCount = 0;
}

bool opcode_add(e_opcode v, op_function f, bool hasBody)
{
	for (uint32 i = 0; i < OPCODE_MAX; i++)
	{
		if (!opCodesArray[i])
		{
			opCodesArray[i] = new opcode_data;
			opCodesArray[i]->opCodeValue = v;
			opCodesArray[i]->opFunction = f;
			opCodesArray[i]->hasBody = hasBody;
			opCodeCount++;
			return true;
		}
	}

	return false;
}

op_function opcode_resolve(e_opcode v, bool* hasBody)
{
	/*uint32 start = 0, mid, end = opCodeCount - 1;

	while (start <= end)
	{
		mid = (start + end) / 2;
		if (v == opCodesArray[mid]->opCodeValue)
			return opCodesArray[mid]->opFunction;
		else if (v < opCodesArray[mid]->opCodeValue)
			end = mid - 1;
		else
			start = mid + 1;
	}*/

	for (uint32 i = 0; i < opCodeCount; i++)
	{
		if (opCodesArray[i]->opCodeValue == v)
		{
			if (hasBody)
				*hasBody = opCodesArray[i]->hasBody;
			return opCodesArray[i]->opFunction;
		}
	}

	return nullptr;
}


//--------------------------------------------------------------INIT
bool WINAPI op_check_version(connection * c, void* argv[])
{
	return  true;
}

bool WINAPI op_login_arbiter(connection * c, void* argv[])
{
	Stream * stream = &c->_recvBuffer.data;

	uint16 nameOffset = stream->ReadInt16();
	uint16 ticketOffset = stream->ReadInt16();
	uint16 ticketSize = stream->ReadInt16();

	int32 unk1 = stream->ReadInt32();
	int8 unk2 = stream->ReadByte();
	int32 unk3 = stream->ReadInt32();
	int32 patchVersion = stream->ReadInt32();

	byte ticket[33];
	stream->_pos = ticketOffset - 4;
	stream->Read(ticket, 32);
	ticket[32] = '\0';

	byte username[SC_PLAYER_NAME_MAX_LENGTH];
	stream->_pos = nameOffset - 4;
	stream->ReadASCIIStringTo(username, SC_PLAYER_NAME_MAX_LENGTH);

	bool result = account_perform_login(c, (sql::Connection*)argv[0], ticket, username);
	if (!result)
	{
		printf("AUTH BAD\n");
		server_connexion_remove(c->_id);
		return false;
	}

	Stream data = Stream();
	data.Resize(5);
	data.WriteInt16(5);
	data.WriteInt16(S_CHECK_VERSION);
	data.WriteByte(1);
	result = connexion_send(c, &data);
	if (!result)
		return false;

	data.Clear();
	data.Resize(5);
	data.WriteInt16(5);
	data.WriteInt16(S_LOADING_SCREEN_CONTROL_INFO);
	data.WriteByte(0);
	result = connexion_send(c, &data);
	if (!result)
		return false;

	data.Clear();
	data.Resize(12);
	data.WriteInt16(12);
	data.WriteInt16(S_REMAIN_PLAY_TIME);
	data.WriteInt32(6);
	data.WriteInt32(0);
	result = connexion_send(c, &data);
	if (!result)
		return false;

	data.Clear();
	data.Resize(23);
	data.WriteInt16(23);
	data.WriteInt16(S_LOGIN_ARBITER);
	data.WriteInt16(1);
	data.WriteInt16(0); //unk?
	data.WriteInt32(0);
	data.WriteInt16(0);
	data.WriteInt32(6);
	data.WriteInt32(0);
	data.WriteByte(0);
	result = connexion_send(c, &data);
	if (!result)
		return false;

	data.Clear();
	data.Resize(14);
	data.WriteInt16(0);
	data.WriteInt16(S_LOGIN_ACCOUNT_INFO);
	data.WriteInt16(14); // server name offset
	data.WriteInt64(7281987); //??? SERVER_ID??
	data.WriteString("PlanetMT");
	data.WritePos(0);

	result = connexion_send(c, &data);
	if (!result)
		return false;

	return true;
}

bool WINAPI op_set_visible_range(connection *, void* argv[])
{
	//??
	return true;
}

bool WINAPI op_get_user_list(connection * c, void* argv[])
{
	Stream stream = Stream();

	stream.WriteInt16(0);
	stream.WriteInt16(S_GET_USER_LIST);
	uint16 count_pos = stream.NextPos();
	uint16 next = stream.NextPos();
	stream.WriteByte(0);
	stream.WriteInt32(0);
	stream.WriteInt32((int)SC_PLAYER_MAX_CHARACTER_COUNT);

	stream.WriteInt32(1);	//unk 
	stream.WriteInt16(0);
	stream.WriteInt32(40);//unk


	stream.WriteInt32(0);
	stream.WriteInt32(24);	//unk

	uint16 count = 0;
	for (uint32 i = 0; i < SC_PLAYER_MAX_CHARACTER_COUNT; i++)
	{
		player * p = c->_players[i];
		if (!p)
			continue;

		count++;
		stream.WritePos(next);
		stream.WriteInt16(stream._pos);
		next = stream.NextPos();

		stream.WriteInt32(0);

		uint16 name_pos = stream.NextPos();
		uint16 details1_pos = stream.NextPos();
		stream.WriteInt16(32);
		uint16 details2_pos = stream.NextPos();
		stream.WriteInt16(64);
		uint16 guild_name_pos = stream.NextPos();

		stream.WriteInt32(i + 1);

		stream.WriteInt32(p->pGender);
		stream.WriteInt32(p->pRace);
		stream.WriteInt32(p->pClass);
		stream.WriteInt32(p->level);

		stream.WriteInt32(6);//unk map id1 ?
		stream.WriteInt32(1231); //unk 1c050000 1308

		stream.WriteInt32(p->position.worldMapWorldId);
		stream.WriteInt32(p->position.worldMapGuardId);
		stream.WriteInt32(p->position.worldMapSectionId);

		stream.WriteInt64(p->lastOnlineUTC); //unk
		stream.WriteByte(p->banTimeUTC > 0 ? 1 : 0); //deleteion in progress
		stream.WriteInt64(p->banTimeUTC); //time until character deletion


		stream.WriteInt32(0);


		stream.WriteInt32(p->_inventory.get_item(PROFILE_WEAPON));
		stream.WriteInt32(p->_inventory.get_item(PROFILE_EARRING_L));
		stream.WriteInt32(p->_inventory.get_item(PROFILE_EARRING_R));
		stream.WriteInt32(p->_inventory.get_item(PROFILE_ARMOR));
		stream.WriteInt32(p->_inventory.get_item(PROFILE_GLOVES));
		stream.WriteInt32(p->_inventory.get_item(PROFILE_BOOTS));

		stream.WriteInt32(p->_inventory.get_item(PROFILE_NECKLACE)); //unk

		stream.WriteInt32(p->_inventory.get_item(PROFILE_RING_L));
		stream.WriteInt32(p->_inventory.get_item(PROFILE_RING_R));
		stream.WriteInt32(p->_inventory.get_item(PROFILE_INNERWARE));
		stream.WriteInt32(p->_inventory.get_item(PROFILE_MASK)); //head
		stream.WriteInt32(p->_inventory.get_item(PROFILE_HEAD_ADRONMENT)); //face

		stream.Write(p->details3, SC_PLAYER_DETAILS_3_BUFFER_SIZE);

		stream.WriteByte(c->_account.isGm ? 0x01 : 0x00);//isgm

		stream.WriteInt64(0);
		stream.WriteInt32(0); //4
		stream.WriteByte(0); //2


		stream.WriteInt32(0xA817EB64); //unk0xA85C54e6
		stream.WriteInt64(0);
		stream.WriteInt64(0);
		stream.WriteInt64(0);
		stream.WriteInt64(0);
		stream.WriteInt64(0);
		stream.WriteInt64(0);
		stream.WriteInt64(0);
		stream.WriteInt64(0);
		stream.WriteInt64(0);
		stream.WriteInt64(0);
		stream.WriteInt64(0);
		stream.WriteInt64(0);

		stream.WriteInt32(p->_inventory.get_enchant_leve(PROFILE_SKIN_HEAD));
		stream.WriteInt32(p->_inventory.get_enchant_leve(PROFILE_SKIN_FACE));
		stream.WriteInt32(p->_inventory.get_enchant_leve(PROFILE_SKIN_BACK));
		stream.WriteInt32(p->_inventory.get_enchant_leve(PROFILE_SKIN_WEAPON));
		stream.WriteInt32(p->_inventory.get_enchant_leve(PROFILE_SKIN_BODY));
		stream.WriteInt32(0);

		stream.WriteInt32(0);

		stream.WriteInt32(55311);
		stream.WriteInt32(55311);
		stream.WriteByte(1);
		stream.WriteInt32(0);
		stream.WriteByte(0);

		stream.WriteInt32(25601);
		stream.WriteByte(0);

		stream.WriteInt32(30); //achievements

		stream.WriteInt32(0);
		stream.WriteInt32(0);
		stream.WriteInt32(0);

		stream.WritePos(name_pos);
		stream.WriteString(p->name);

		stream.WritePos(details1_pos);
		stream.Write(p->details1, SC_PLAYER_DETAILS_1_BUFFER_SIZE);

		stream.WritePos(details2_pos);
		stream.Write(p->details2, SC_PLAYER_DETAILS_2_BUFFER_SIZE);

		stream.WritePos(guild_name_pos);
		stream.WriteInt16(0);

	}

	stream._pos = count_pos;
	stream.WriteInt16(count);
	stream.SetEnd();
	stream.WritePos(0);

	bool result = connexion_send(c, &stream);
	if (!result)
		return false;

	stream.Clear();
	stream.Resize(8);
	stream.WriteInt16(8);
	stream.WriteInt16(S_LOAD_CLIENT_ACCOUNT_SETTING);
	stream.WriteInt16(8);
	stream.WriteInt16(0);
	result = connexion_send(c, &stream);
	if (!result)
		return false;

	stream.Clear();
	stream.Resize(8);
	stream.WriteInt16(8);
	stream.WriteInt16(S_ACCOUNT_PACKAGE_LIST);
	stream.WriteInt32(0);
	result = connexion_send(c, &stream);
	if (!result)
		return false;

	stream.Clear();
	stream.Resize(17);
	stream.WriteInt16(17);
	stream.WriteInt16(S_CONFIRM_INVITE_CODE_BUTTON);
	stream.WriteInt32(15);
	stream.WriteInt32(0);
	stream.WriteInt32(0);
	stream.WriteByte(0);
	result = connexion_send(c, &stream);
	if (!result)
		return false;


	stream.Clear();
	stream.Resize(9);
	stream.WriteInt16(9);
	stream.WriteInt16(S_UPDATE_CONTENTS_ON_OFF);
	stream.WriteInt32(2);
	stream.WriteByte(0);
	result = connexion_send(c, &stream);
	if (!result)
		return false;

	stream.Clear();
	stream.Resize(9);
	stream.WriteInt16(9);
	stream.WriteInt16(S_UPDATE_CONTENTS_ON_OFF);
	stream.WriteInt32(3);
	stream.WriteByte(0);
	result = connexion_send(c, &stream);
	if (!result)
		return false;

	stream.Clear();
	stream.Resize(9);
	stream.WriteInt16(9);
	stream.WriteInt16(S_UPDATE_CONTENTS_ON_OFF);
	stream.WriteInt32(4);
	stream.WriteByte(0);
	result = connexion_send(c, &stream);
	if (!result)
		return false;

	stream.Clear();
	stream.Resize(9);
	stream.WriteInt16(9);
	stream.WriteInt16(S_UPDATE_CONTENTS_ON_OFF);
	stream.WriteInt32(8);
	stream.WriteByte(0);
	result = connexion_send(c, &stream);
	if (!result)
		return false;

	stream.Clear();
	stream.Resize(9);
	stream.WriteInt16(9);
	stream.WriteInt16(S_UPDATE_CONTENTS_ON_OFF);
	stream.WriteInt32(9);
	stream.WriteByte(0);
	result = connexion_send(c, &stream);
	if (!result)
		return false;

	return true;
}

bool WINAPI op_hardware_info(connection *, void* argv[])
{
	return true;
}

bool WINAPI op_request_vip_system_info(connection *c, void* argv[])
{
	Stream data = Stream();
	data.WriteInt16(0);
	data.WriteInt16(S_SEND_VIP_SYSTEM_INFO);
	uint16 next = data.NextPos();
	data.WriteByte(1); //hasTeraRewards
	data.WriteInt32(45); //teraRewardsValue
	data.WriteInt32(0); //1780
	data.WriteInt32(0);//6060
	data.WriteInt32(0); //330
	data.WriteInt32(0); //0
	data.WriteByte(1); //bool? what?

	data.WriteInt32(timer_get_day_seconds()); // 3752


	data.WriteInt32(0);
	data.WriteInt32(0);
	data.WriteInt32(0);
	data.WriteByte(1);
	data.WritePos(next);
	data.WriteInt16(0);//?
	data.WritePos(0);
	bool result = connexion_send(c, &data);
	if (!result)
		return false;
	return true;
}

bool WINAPI op_get_user_guild_logo(connection *c, void* argv[])
{
	Stream data = Stream();
	data.Resize(8);
	data.WriteInt16(8);
	data.WriteInt16(S_GET_USER_GUILD_LOGO);
	data.WriteInt32(0);

	return connexion_send(c, &data);;
}



//--------------------------------------------------------------LOBBY
bool WINAPI op_can_create_player(connection * c, void* argv[])
{
	if (!c)
		return false;
	int8 index = connexion_can_create_player(c);

	Stream data = Stream(5);
	data.WriteInt16(5);
	data.WriteInt16(S_CAN_CREATE_USER);
	data.WriteByte(index >= 0 ? 1 : 0);
	return connexion_send(c, &data);
}

bool WINAPI op_create_player(connection * c, void* argv[])
{
	int result = true;
	uint8 index = connexion_can_create_player(c);
	if (index < 0)
		result = false;
	player *p = c->_players[index] = new player();
	if (!p)
		result = false;
	if (result)
	{
		Stream * stream = &c->_recvBuffer.data;

		uint16 nameOffset = stream->ReadInt16();
		uint16 details1Offset = stream->ReadInt16();
		stream->ReadInt16();
		uint16 details2Offset = stream->ReadInt16();
		stream->ReadInt16();

		p->pGender = (e_player_gender)stream->ReadInt32();
		p->pRace = (e_player_race)stream->ReadInt32();
		p->pClass = (e_player_class)stream->ReadInt32();

		stream->Read(p->details3, SC_PLAYER_DETAILS_3_BUFFER_SIZE);

		stream->_pos = nameOffset - 4;
		stream->ReadASCIIStringTo((byte*)p->name, SC_PLAYER_NAME_MAX_LENGTH);

		stream->_pos = details1Offset - 4;
		stream->Read(p->details1, SC_PLAYER_DETAILS_1_BUFFER_SIZE);

		stream->_pos = details2Offset - 4;
		stream->Read(p->details2, SC_PLAYER_DETAILS_2_BUFFER_SIZE);

		p->creationTimeUTC = timer_get_current_UTC();
		p->lastOnlineUTC = 0;
		p->banTimeUTC = 0;

		p->exp = 1;
		p->restedExp = 1;
		p->maxExp = 1;

		player_calculate_model(p);

		p->level = 1;

		p->position.areaId = config::area.area_start_continent;
		p->position.coords[0] = config::area.area_start_position[0];
		p->position.coords[1] = config::area.area_start_position[1];
		p->position.coords[2] = config::area.area_start_position[2];
		p->position.heading = config::area.area_start_heading;
		p->position.worldMapGuardId = config::area.area_start_world_map_guard_id;
		p->position.worldMapSectionId = config::area.area_start_world_map_section_id;
		p->position.worldMapWorldId = config::area.area_start_world_map_wrold_id;

		p->position.radius = 100; //todo

		p->_inventory = inventory();
		inventory_init(p->_inventory, config::player.start_inventory_slot_count);
		inventory_new(p);

		stream->Clear();
		stream->Resize(5);
		stream->WriteInt16(5);
		stream->WriteInt16(S_CREATE_USER);
		stream->WriteByte(1);

		result = connexion_send(c, stream);
		if (!result)
			return false;

		sql::Connection *con = (sql::Connection*)argv[0];
		if (con)
		{
			std::istringstream details1 = std::istringstream(std::string((const char*)p->details1, SC_PLAYER_DETAILS_1_BUFFER_SIZE));
			std::istringstream details2 = std::istringstream(std::string((const char*)p->details2, SC_PLAYER_DETAILS_2_BUFFER_SIZE));
			std::istringstream data = std::istringstream(std::string((const char*)p->details3, SC_PLAYER_DETAILS_3_BUFFER_SIZE));
			sql::PreparedStatement * s = nullptr;
			try
			{
				s = con->prepareStatement("INSERT INTO players(username,name,x,y,z,h,race,gender,class,exp,restedExp,areaId,level,details1,details2,details3,lastOnlineUTC,creationTimeUTC,banTimeUTC,visitedSections,worldMapGuardId,worldMapWorldId,worldMapSectionId) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

				s->setString(1, c->_account.username);
				s->setString(2, p->name);

				s->setDouble(3, p->position.coords[0]);
				s->setDouble(4, p->position.coords[1]);
				s->setDouble(5, p->position.coords[2]);
				s->setInt(6, p->position.heading);

				s->setInt(7, p->pRace);
				s->setInt(8, p->pGender);
				s->setInt(9, p->pClass);

				s->setInt64(10, p->exp);
				s->setInt64(11, p->restedExp);

				s->setInt(12, p->position.areaId);
				s->setInt(13, p->level);

				s->setBlob(14, &details1);
				s->setBlob(15, &details2);
				s->setBlob(16, &data);

				s->setUInt64(17, timer_get_current_UTC());
				s->setUInt64(18, p->creationTimeUTC);
				s->setUInt64(19, p->banTimeUTC);
				s->setNull(20, 0);

				s->setInt(21, p->position.worldMapGuardId);
				s->setInt(22, p->position.worldMapWorldId);
				s->setInt(23, p->position.worldMapSectionId);

			}
			catch (const sql::SQLException& e)
			{
				std::cout << ">>Error:FAILED TO INSERT player[" << e.what() << "]\n";
			}
			try
			{
				result = s->executeUpdate();
			}
			catch (const sql::SQLException& e)
			{
				std::cout << ">>Error:FAILED TO INSERT player[" << e.what() << "]\n";
			}

			delete s;

			Stream * inventoryRaw = p->_inventory.get_raw();
			std::istringstream invBlob = std::istringstream(std::string((const char*)inventoryRaw->_raw, inventoryRaw->_size));
			s = con->prepareStatement("INSERT INTO player_inventory(username,name,items,slotCount,gold) VALUES(?,?,?,?,?)");
			s->setString(1, c->_account.username);
			s->setString(2, p->name);
			s->setBlob(3, &invBlob);

			s->setInt(4, p->_inventory.slotCount);
			s->setInt64(5, p->_inventory.gold);
			try
			{
				result = s->executeUpdate();
			}
			catch (const sql::SQLException& e)
			{
				std::cout << ">>Error:FAILED TO INSERT player_inventory" << e.what() << "\n";
			}

			delete inventoryRaw;
			delete s;

		}
	}

	c->_recvBuffer.data.Clear();
	c->_recvBuffer.data.Resize(5);
	c->_recvBuffer.data.WriteInt16(5);
	c->_recvBuffer.data.WriteInt16(S_CREATE_USER);
	c->_recvBuffer.data.WriteByte(1);
	return connexion_send(c, &c->_recvBuffer.data);
}

bool WINAPI op_change_user_slot_id(connection *, void* argv[])
{
	return false;
}

bool WINAPI op_check_username(connection *c, void* argv[])
{
	Stream data = Stream();
	data.Resize(5);
	data.WriteInt16(0);
	data.WriteInt16(S_CHECK_USERNAME);
	data.WriteByte(1);
	data.WritePos(0);

	return connexion_send(c, &data);
}

bool WINAPI op_str_evaluate_string(connection *c, void* argv[])
{
	c->_recvBuffer.data._pos = 18;
	byte name[38];
	bool nameGood = c->_recvBuffer.data.ReadASCIIStringTo(name, 38);

	Stream stream;
	stream.WriteInt16(0);
	stream.WriteInt16(S_STR_EVALUATE_LIST);

	if (nameGood)
	{
		stream.WriteInt16(1);//unk
		stream.WriteInt16(8);//unk
		stream.WriteInt16(8);//unk
		stream.WriteInt16(0);//unk
		stream.WriteInt16(22);	 //unk
		stream.WriteInt16(1);
		stream.WriteInt16(0);
		stream.WriteInt16(0);
		stream.WriteInt16(0);
		stream.WriteString((char*)name);

		//WriteHexString(stream, "010008000800 0000 1600 0100 0000 0000 0000 4E00610072006A0061007A007A002E0054000000");

	}
	else
	{
		//not tested
		stream.WriteInt16(0);//unk
		stream.WriteInt16(8);//unk
		stream.WriteInt16(8);//unk
		stream.WriteInt16(0);//unk
		stream.WriteInt16(22);	 //unk
		stream.WriteInt16(0);
		stream.WriteInt16(0);
		stream.WriteInt16(0);
		stream.WriteInt16(0);
		stream.WriteString((char*)name);
	}
	stream.WritePos(0);

	return connexion_send(c, &stream);
}

bool WINAPI op_select_player(connection *, void* argv[])
{
	return false;
}

bool WINAPI op_delete_player(connection *c, void* argv[])
{
	uint32 lobby_id = c->_recvBuffer.data.ReadInt32();
	bool result = true;

	sql::Connection* con = (sql::Connection*)argv[0];
	if (con)
	{
		sql::PreparedStatement * p = con->prepareStatement("DELETE  FROM players WHERE name=?");
		p->setString(1, c->_players[lobby_id - 1]->name);
		try
		{
			p->executeUpdate();
		}
		catch (const sql::SQLException& e)
		{
			std::cout << ">>Error:DELETE PLAYER[" << e.what() << "]\n";
		}
		delete p;

		p = con->prepareStatement("DELETE FROM player_inventory WHERE name=?");
		p->setString(1, c->_players[lobby_id - 1]->name);
		try
		{
			p->executeUpdate();
		}
		catch (const sql::SQLException& e)
		{
			std::cout << ">>Error:DELETE PLAYER[" << e.what() << "]\n";
		}
		delete p;

		p = con->prepareStatement("DELETE FROM player_bank WHERE name=?");
		p->setString(1, c->_players[lobby_id - 1]->name);
		try
		{
			p->executeUpdate();
		}
		catch (const sql::SQLException& e)
		{
			std::cout << ">>Error:DELETE PLAYER[" << e.what() << "]\n";
		}
		delete p;

		p = con->prepareStatement("DELETE FROM player_skills WHERE name=?");
		p->setString(1, c->_players[lobby_id - 1]->name);
		try
		{
			p->executeUpdate();
		}
		catch (const sql::SQLException& e)
		{
			std::cout << ">>Error:DELETE PLAYER[" << e.what() << "]\n";
		}
		delete p;

		p = con->prepareStatement("DELETE FROM player_settings WHERE name=?");
		p->setString(1, c->_players[lobby_id - 1]->name);
		try
		{
			p->executeUpdate();
		}
		catch (const sql::SQLException& e)
		{
			std::cout << ">>Error:DELETE PLAYER[" << e.what() << "]\n";
		}
		delete p;

		

	}

	if (lobby_id < SC_PLAYER_MAX_CHARACTER_COUNT&& lobby_id >= 0)
		if (c->_players[lobby_id - 1])
		{
			delete c->_players[lobby_id - 1];
			c->_players[lobby_id - 1] = NULL;
		}

	Stream data = Stream();
	data.Resize(5);
	data.WriteInt16(5);
	data.WriteInt16(S_DELETE_USER);
	data.WriteByte(result == true ? 1 : 0);

	return connexion_send(c, &data);
}

bool WINAPI op_cancel_delete_player(connection *, void* argv[])
{
	return false;
}



//--------------------------------------------------------------WORLD
bool WINAPI op_load_topo_fin(connection *, void* argv[])
{
	return false;
}

bool WINAPI op_return_to_lobby(connection *, void* argv[])
{
	return false;
}

bool WINAPI op_cancel_return_to_lobby(connection *, void* argv[])
{
	return false;
}

bool WINAPI op_guard_pk_policy(connection *, void* argv[])
{
	return false;
}

bool WINAPI op_move_player(connection *, void* argv[])
{
	return false;
}

bool WINAPI op_npc_guild_list(connection *, void* argv[])
{
	return false;
}

bool WINAPI op_raign_info(connection *, void* argv[])
{
	return false;
}

bool WINAPI op_request_gamestat_ping(connection *c, void* argv[])
{
	c->_recvBuffer.data.Clear();
	c->_recvBuffer.data.Resize(5);
	c->_recvBuffer.data.WriteInt16(4);
	c->_recvBuffer.data.WriteInt16(S_RESPONSE_GAMESTAT_PONG);
	return connexion_send(c, &c->_recvBuffer.data);
}

bool WINAPI op_social(connection *, void* argv[])
{
	return false;
}

bool WINAPI op_visit_new_section(connection *, void* argv[])
{
	return false;
}
