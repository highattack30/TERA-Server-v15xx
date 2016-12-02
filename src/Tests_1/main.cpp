

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>

#include "socket.h"
#include "logService.h"
#include "server.h"
#include "OpCodes.h"
#include "config.h"
#include "DBHandler.h"
#include "dataservice.h"

void release();

int main(int32 argc, char** argv)
{
	srand((unsigned int)time(0));

	std::string cmdLine;


	if (!init_config(".//Data//config.txt"))

		goto error_proc;

	TITLE(config::server.server_Name);

	if (!opcode_init())
	{
		LOG("\nOpcodes init failed!");
		goto error_proc;
	}

	if (!mysql_init())
	{
		LOG("\nMysql init failed!");
		goto error_proc;
	}

	if (!data_service::data_init())
	{
		LOG("\nData init failed!");
		goto error_proc;
	}

	if (!server_init())
	{
		LOG("\nServerInit failed!");
		goto error_proc;
	}




#pragma region cmdLine

	while (1)
	{
		std::cin >> cmdLine;

		if (cmdLine == "exit")
		{
			break;
		}
		else if (cmdLine == "cls")
			system("cls");
		Sleep(0);
	}
#pragma endregion

	release();
	system("pause");
	return 0;

error_proc:

	release();
	LOG("\nERROR!\n");

	_getch();
	return 0;
}

void release()
{
	data_service::data_release();
	server_close();
	opcode_release();
	mysql_release();
}