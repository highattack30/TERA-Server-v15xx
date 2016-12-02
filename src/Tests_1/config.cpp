#include "config.h"

#include "stringUtils.h"
#include <fstream>



bool init_config(STRING fileName)
{
	std::fstream file = std::fstream(fileName);
	if (!file.is_open())
		return false;
	std::string error_message;
	std::string line;
	uint32 lineCount = 0;
	while (!file.eof())
	{
		std::getline(file, line);
		lineCount++;

		if (
			(line[0] == '/' && line[1] == '/') ||
			(line[0] == '#')
			)
			continue;
		else if (stringStartsWith(line, "server.name="))
		{
			if (!sscanf(line.c_str(), "server.name=%s", config::server.server_Name))
			{
				error_message = "server_name max 32 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.id="))
		{
			if (!sscanf(line.c_str(), "server.id=%d", &config::server.server_Id))
			{
				error_message = "bad server_id value!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.number_of_threads_per_cpu="))
		{
			if (!sscanf(line.c_str(), "server.number_of_threads_per_cpu= %d", &config::server.no_of_threads_per_cpu))
			{
				error_message = "bad number_of_threads_per_cpu value!";
				goto error_proc;
			}

			if (config::server.no_of_threads_per_cpu <= 0 ||
				config::server.no_of_threads_per_cpu > 50)
			{
				error_message = "value within [1 - 50]!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.job_pull_count="))
		{
			if (!sscanf(line.c_str(), "server.job_pull_count= %d", &config::server.job_pull_cout))
			{
				error_message = "bad job_pull_count value!";
				goto error_proc;
			}

			if (config::server.job_pull_cout <= 0 ||
				config::server.job_pull_cout > 32)
			{
				error_message = "value within [1 - 32]!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.localhost="))
		{
			char value[10];
			if (!sscanf(line.c_str(), "server.localhost= %s", value))
				goto error_proc;
			if (strcmp(value, "True"))
			{
				config::net.localhost = true;
			}
			else if (strcmp(value, "False"))
				config::net.localhost = false;
			else
			{
				error_message = "use 'True' or 'False' as value!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.ip="))
		{
			if (!sscanf(line.c_str(), "server.ip= %s", config::net.ip))
			{
				error_message = "wrong ip format!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.port="))
		{
			if (!sscanf(line.c_str(), "server.port= %hu", &config::net.port))
				goto error_proc;
		}
		else if (stringStartsWith(line, "server.mysql.ip="))
		{
			if (!sscanf(line.c_str(), "server.mysql.ip= %s", config::mysql.mysqlIp))
			{
				error_message = "wrong ip format!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.mysql.port="))
		{
			if (!sscanf(line.c_str(), "server.mysql.port= %d", &config::mysql.mysqlPort))
			{
				error_message = "bad port value!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.mysql.db_name="))
		{
			if (!sscanf(line.c_str(), "server.mysql.db_name= %s", config::mysql.mysqlDbName))
			{
				error_message = "db_name max 16 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.mysql.username="))
		{
			if (!sscanf(line.c_str(), "server.mysql.username= %s", &config::mysql.mysqlUser))
			{
				error_message = "username max 32 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.mysql.password="))
		{
			if (!sscanf(line.c_str(), "server.mysql.password= %s", &config::mysql.mysqlPassword))
			{
				error_message = "password max 32 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.dir.npc="))
		{
			if (!sscanf(line.c_str(), "server.dir.npc= %s", config::dir.dataNpc))
			{
				error_message = "npcs dir. max 259 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.dir.skills.player="))
		{
			if (!sscanf(line.c_str(), "server.dir.skills.player= %s", config::dir.dataPlayerSkills))
			{
				error_message = "player skills dir. max 259 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.dir.skills.npc="))
		{
			if (!sscanf(line.c_str(), "server.dir.skills.npc= %s", config::dir.dataNpcSkills))
			{
				error_message = "npc skills dir. max 259 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.dir.items="))
		{
			if (!sscanf(line.c_str(), "server.dir.items= %s", &config::dir.dataItems))
			{
				error_message = "items dir. max 259 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.dir.abnormalities="))
		{
			if (!sscanf(line.c_str(), "server.dir.abnormalities= %s", &config::dir.dataAbnormalities))
			{
				error_message = "abnormalities dir. max 259 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.dir.passivities="))
		{
			if (!sscanf(line.c_str(), "server.dir.passivities= %s", &config::dir.dataPassivities))
			{
				error_message = "passivities dir. max 259 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "server.dir.equipment="))
		{
			if (!sscanf(line.c_str(), "server.dir.equipment= %s", &config::dir.dataEquipment))
			{
				error_message = "equipment dir. max 259 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "player.additional_start_items="))
		{
			if (!sscanf(line.c_str(), "player.additional_start_items= %s", config::player.additional_start_items))
			{
				error_message = "additional_start_items max 149 characters!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "player.start_inventory_slot_cout="))
		{
			if (!sscanf(line.c_str(), "player.start_inventory_slot_cout= %d", &config::player.start_inventory_slot_count))
			{
				error_message = "bad start_inventory_slot_count max[0xffff] value!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "area.start_positon="))
		{
			if (!sscanf(line.c_str(), "area.start_positon= %f %f %f", &config::area.area_start_position[0], &config::area.area_start_position[1], &config::area.area_start_position[2]))
			{
				error_message = "bad position value! [x] [y] [z]";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "area.start_heading="))
		{
			if (!sscanf(line.c_str(), "area.start_heading= %d", &config::area.area_start_heading))
			{
				error_message = "bad heading value!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "area.start_continent="))
		{
			if (!sscanf(line.c_str(), "area.start_continent= %d", &config::area.area_start_continent))
			{
				error_message = "bad start_continent value!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "area.start_world_map_world_id="))
		{
			if (!sscanf(line.c_str(), "area.start_world_map_world_id= %d", &config::area.area_start_world_map_wrold_id))
			{
				error_message = "bad start_world_map_world_id value!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "area.start_world_map_guard_id="))
		{
			if (!sscanf(line.c_str(), "area.start_world_map_guard_id= %d", &config::area.area_start_world_map_guard_id))
			{
				error_message = "bad start_world_map_guard_id value!";
				goto error_proc;
			}
		}
		else if (stringStartsWith(line, "area.start_world_map_section_id="))
		{
			if (!sscanf(line.c_str(), "area.start_world_map_section_id= %d", &config::area.area_start_world_map_section_id))
			{
				error_message = "bad start_world_map_section_id value!";
				goto error_proc;
			}
		}


	}

	file.close();

	return true;

error_proc:
	printf("\nFailed to load config_file. ERROR LINE-NO[%d]\nVALUE[%s]\nMESSAGE[%s]\n", lineCount, line.c_str(), error_message.c_str());
	file.close();
	return false;
}

config::config()
{

}

config::~config()
{

}

config::dir_c			config::dir;
config::server_c		config::server;
config::mysql_c			config::mysql;
config::net_c			config::net;
config::player_c		config::player;
config::area_c			config::area;