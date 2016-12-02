#ifndef SERVERDEFS_H
#define SERVERDEFS_H

#include <stdio.h>
#include <mutex>

#include "socket.h"
#include "typeDefs.h"
#include "logService.h"
#include "serverEnums.h"


#define WSA_VERSION MAKEWORD(2,2)

class connection;

struct server_data
{
	SOCKET listeningSocket;
	struct sockaddr_in listenerData;

	WSADATA wsaData;
	WSAEVENT acceptEvent;
	WSAEVENT shutdownEvent;

	DWORD acceptThreadID;
	HANDLE acceptThread;

	uint32 noOfThreads;
	SYSTEM_INFO sysInfo;
	HANDLE * workThreads;
	HANDLE _IOCP;

	std::mutex clientListMutex;
	uint32 clientsCount;
	connection * clients[SBS_SERVER_MAX_CLIENTS];

	CRITICAL_SECTION log_sync;
};

static server_data server;

DWORD WINAPI server_worker_func(void* argv);
DWORD WINAPI server_accept_func(void* argv);

bool WINAPI server_init();
void WINAPI server_close();

bool WINAPI server_connexion_add(SOCKET sock, sockaddr_in sockData);
bool WINAPI server_connexion_remove(uint32 id);

void server_lock_log();
void server_unlock_log();

#endif // !SERVERDEFS_H

