#include "server.h"
#include "config.h"
#include "connexion.h"
#include "job.h"
#include "Stream.h"
#include "DBHandler.h"

bool WINAPI server_init() {

	GetSystemInfo(&server.sysInfo);
	server.noOfThreads = config::server.no_of_threads_per_cpu * server.sysInfo.dwNumberOfProcessors;

	printf("::Number of processors on host: %d\n", server.sysInfo.dwNumberOfProcessors);
	printf("::The following number of work threads will be created: %d\n", server.noOfThreads);

	server.workThreads = new HANDLE[server.noOfThreads];

	if (WSAStartup(WSA_VERSION, &server.wsaData) != NO_ERROR)
		return false;

	server.shutdownEvent = CreateEvent(NULL, true, false, NULL);

	if (NULL == (server._IOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)))
		return false;


	for (uint32 i = 0; i < server.noOfThreads; i++)
		server.workThreads[i] = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)server_worker_func, (void*)server._IOCP, 0, NULL);

	/*-----------------------------------------------------------*/
	server.listenerData.sin_addr.S_un.S_addr = config::net.localhost ? INADDR_ANY : inet_addr(config::net.ip);
	server.listenerData.sin_family = AF_INET;
	server.listenerData.sin_port = htons(config::net.port); // config.net.port

	if (
		((server.listeningSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) ||
		(SOCKET_ERROR == (bind(server.listeningSocket, (const sockaddr *)&server.listenerData, sizeof(server.listenerData)))) ||
		(SOCKET_ERROR == (listen(server.listeningSocket, SOMAXCONN))) ||
		((server.acceptEvent = WSACreateEvent()) == WSA_INVALID_EVENT) ||
		(SOCKET_ERROR == (WSAEventSelect(server.listeningSocket, server.acceptEvent, FD_ACCEPT)))
		)
		return false;

	server.acceptThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)server_accept_func, (void*)&server.listeningSocket, 0, NULL);

	InitializeCriticalSection(&server.log_sync);

	return true;
}

void WINAPI server_close()
{
	CloseHandle(server._IOCP);
	SetEvent(server.shutdownEvent);

	WaitForSingleObject(server.acceptThread, INFINITE);

	for (uint32 i = 0; i < server.noOfThreads; i++)
	{
		PostQueuedCompletionStatus(server._IOCP, 0, (DWORD)NULL, NULL);
	}

	WaitForMultipleObjects((DWORD)server.noOfThreads, server.workThreads, true, INFINITE);
	delete[] server.workThreads;

	WSACloseEvent(server.acceptEvent);
	WSACloseEvent(server.shutdownEvent);



	closesocket(server.listeningSocket);
	shutdown(server.listeningSocket, SD_BOTH);


	for (uint32 i = 0; i < SBS_SERVER_MAX_CLIENTS; i++)
	{
		if (server.clients[i])
		{
			connexion_close(server.clients[i]);
			delete server.clients[i];
			server.clients[i] = NULL;
		}
	}

	DeleteCriticalSection(&server.log_sync);
	WSACleanup();
	LOG("\n\nServer Closed!\n");
}

bool WINAPI server_connexion_add(SOCKET sock, sockaddr_in sockData)
{
	if (server.clientsCount + 1 > SBS_SERVER_MAX_CLIENTS)
		return false;

	std::lock_guard<std::mutex> locker(server.clientListMutex);
	for (uint32 i = 0; i < SBS_SERVER_MAX_CLIENTS; i++)
	{
		if (!server.clients[i])
		{
			connection * newConnexion = connexion_create(sock, server._IOCP, i);
			if (newConnexion)
			{
				server.clients[i] = newConnexion;
				server.clientsCount++;
				printf("new connection\n");
				return true;
			}
			else
				break;
		}
	}

	return false;
}

bool WINAPI server_connexion_remove(uint32 id)
{
	if (id >= SBS_SERVER_MAX_CLIENTS)
		return false;

	std::lock_guard<std::mutex> locker(server.clientListMutex);
	if (server.clients[id])
	{
		connection * c = server.clients[id];
		server.clients[id] = NULL;

		connexion_close(c);
		delete c;

		server.clientsCount--;

		printf("CONNEXION_REMOVED ID[%d]\n", id);
		return true;
	}

	return false;
}


void server_lock_log()
{
	EnterCriticalSection(&server.log_sync);
}

void server_unlock_log()
{
	LeaveCriticalSection(&server.log_sync);
}

DWORD WINAPI server_worker_func(void* argv)
{
	HANDLE iocpPort = (HANDLE)argv;
	OVERLAPPED_ENTRY* ov = NULL;
	
	DWORD noOfBytesTransfered = 0;
	PULONG_PTR key = new unsigned long long;
	ULONG no_of_job_pulled = 0;
	sql::Connection * sqlCon = mysql_get_driver()->open();
	if (!sqlCon)
	{
		printf("::WORKER_THREAD::SQL CONNECTION BAD!!\n");
		return 1;
	}

	while (WAIT_OBJECT_0 != WaitForSingleObject(server.shutdownEvent, 0))
	{
		ov = new OVERLAPPED_ENTRY[config::server.job_pull_cout];
		memset(ov, 0, sizeof(OVERLAPPED_ENTRY) * config::server.job_pull_cout);
		BOOL retVal = GetQueuedCompletionStatusEx(iocpPort,ov, config::server.job_pull_cout, &no_of_job_pulled, INFINITE, false);
		
		if (WAIT_OBJECT_0 == WaitForSingleObject(server.shutdownEvent, 0) || ERROR_ABANDONED_WAIT_0 == WSAGetLastError())
			break;
		if (ov)
		{
			for (uint32 i = 0; i < no_of_job_pulled; i++)
			{
				noOfBytesTransfered = ov[i].dwNumberOfBytesTransferred;

				job* j = (job*)ov[i].lpOverlapped;
				if (j)
				{
					switch (j->type)
					{
					case J_RECV:
					{
						connection * c = NULL;
						RECV_BUFFER * buffer = (RECV_BUFFER*)j->argv[0];
						if (buffer && buffer->connexion_id >= 0 && buffer->connexion_id < SBS_SERVER_MAX_CLIENTS)
						{
							c = server.clients[buffer->connexion_id];
						}

						if (c)
						{
							if (noOfBytesTransfered)
							{
								bool result = false;
								switch (buffer->recvStatus)
								{
								case CP_TERA_PACKET:
								{
									result = connexion_process_recv_packet(c, (void**)&sqlCon);
								}break;

								case CP_KEY_1:
								{
									result = connexion_fill_key(c, 1);
									if (result)
										result = c->_session->send_server_key(c->_socket, 1);

									if (result)
									{
										buffer->recvStatus = CP_KEY_2;
										result = connexion_recv(c);
									}
								}break;

								case CP_KEY_2:
								{

									result = connexion_fill_key(c, 3);
									if (result)
										result = c->_session->send_server_key(c->_socket, 2);
									if (result)
										result = connexion_crypt_init(c);
									if (result)
									{
										buffer->data.Clear();
										buffer->data.Resize(4);
										buffer->recvStatus = CP_TERA_PACKET;
										result = connexion_recv(c);
									}
								}
								}break;

								if (!result)
								{
									printf("CONNEXION LOST:[%d]\n", c->_id);
									server_connexion_remove(c->_id);
								}
							}
							else
							{
								printf("CONNEXION LOST:[%d]\n", c->_id);
								server_connexion_remove(c->_id);
							}
						}
						else
						{
							printf("RECV INTERNAL ERROR\n");
						}
					}break;

					case J_SEND:
					{
						if (j->argc == 2)
						{
							uint32 connexion_index = *(uint32*)j->argv[0];
							uint16 send_slot_index = *(uint16*)j->argv[1];
							connexion_finish_send(server.clients[connexion_index], send_slot_index);
						}
						else
							printf("INTERNAL ERROR 2\n");

					}break;
					}

					ov = NULL;

					if (j->argv)
					{
						delete[] j->argv;
						j->argv = NULL;
					}

					delete j;
					j = NULL;

				}
				else
				{
					printf("ERROR::JOB NULL\n");
				}
			}
			
			delete[] ov;
		}
		else
			break;
	}

	

	if (sqlCon)
	{
		sqlCon->close();
		delete sqlCon;
	}

	printf("THREAD CLOSED!\n");
	delete key;
	return 0;
}

DWORD WINAPI server_accept_func(void* argv)
{
	SOCKET listenSock = *(SOCKET*)argv;

	WSANETWORKEVENTS WSAEvents;

	while (WAIT_OBJECT_0 != WaitForSingleObject(server.shutdownEvent, 0))
	{
		if (WSA_WAIT_TIMEOUT != WSAWaitForMultipleEvents(1, &server.acceptEvent, false, 200, false))
		{
			WSAEnumNetworkEvents(listenSock, server.acceptEvent, &WSAEvents);
			if ((WSAEvents.lNetworkEvents & FD_ACCEPT) && (0 == WSAEvents.iErrorCode[FD_ACCEPT_BIT]))
			{
				sockaddr_in client_address;
				int addressSize = sizeof(client_address);

				SOCKET client_socket = accept(listenSock, (sockaddr*)&client_address, &addressSize);
				if (client_socket != INVALID_SOCKET)
					server_connexion_add(client_socket, client_address);
			}
		}
	}
	return 0;
}



