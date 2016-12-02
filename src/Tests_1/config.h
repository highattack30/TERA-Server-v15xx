#ifndef CONFIG_H
#define CONFIG_H

#define _CRT_SECURE_NO_WARNINGS

#include "socket.h"
#include "typeDefs.h"
#include <string>


class config
{
public:
	config();
	~config();

	struct server_c
	{
		uint32 no_of_threads_per_cpu,
			job_pull_cout;
		uint32 server_Id;
		char server_Name[32];
	} static server;

	struct net_c
	{
		bool
			localhost;

		char
			ip[16];

		uint16
			port;
	} static net;

	struct dir_c
	{
		char
			dataNpc[FILENAME_MAX],
			dataNpcSkills[FILENAME_MAX],
			dataPlayerSkills[FILENAME_MAX],
			dataItems[FILENAME_MAX],
			dataAbnormalities[FILENAME_MAX],
			dataPassivities[FILENAME_MAX],
			dataEquipment[FILENAME_MAX];
	} static dir;

	struct mysql_c
	{
		char mysqlIp[16];
		uint32 mysqlPort;
		char mysqlUser[32];
		char mysqlPassword[32];
		char mysqlDbName[16];
	} static mysql;

	struct player_c
	{
		char additional_start_items[150];
		uint16 start_inventory_slot_count;
	} static player;

	struct area_c
	{
		float area_start_position[3];
		int16 area_start_heading;
		uint32 area_start_continent;
		uint32 area_start_world_map_wrold_id;
		uint32 area_start_world_map_guard_id;
		uint32 area_start_world_map_section_id;

	}static area;
};



bool init_config(STRING fileName);

#endif
