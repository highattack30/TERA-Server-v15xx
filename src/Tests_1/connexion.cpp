#include "connexion.h"
#include "serverEnums.h"
#include "OpCodes.h"
#include "job.h"
#include "server.h"
#include "Stream.h"
#include "account.h"

#include <string>
#include <stdio.h>
#include <random>


//init
connection::connection(SOCKET sock, uint32 id)
{
	_testId = 0;
	_session = new Session();

	byte server_key_1[128];
	byte server_key_2[128];
	for (uint8 i = 0; i < 128; i++)
	{
		server_key_1[i] = rand() % 0xff;
		server_key_2[i] = rand() % 0xff;
	}

	_session->fill_key(server_key_1, 2);
	_session->fill_key(server_key_2, 4);

	InitializeCriticalSection(&_cryptLock);
	InitializeCriticalSection(&_syncLock);

	
	memset(&_account, 0, sizeof account);

	for (uint8 i = 0; i < SC_PLAYER_MAX_CHARACTER_COUNT; i++)
		_players[i] = NULL;


	for (uint8 i = 0; i < SBS_CLEINT_SEND_QUEUE_SIZE; i++)
	{
		InitializeCriticalSection(&_sendSlots[i].lock_check);
		_sendSlots[i].lock_bit = 0;
		_sendSlots[i].buffer.flags = 0;
		_sendSlots[i].buffer.transferBytes = 0;
		_sendSlots[i].buffer.wsaBuff.buf = NULL;
		_sendSlots[i].buffer.wsaBuff.len = 0;
		_sendSlots[i].index = i;
	}

	_inLobby = true;
	_socket = sock;
	_id = id;

	_recvBuffer.data = Stream();
	_recvBuffer.data.Resize(128);
	_recvBuffer.recvStatus = CP_KEY_1;
	_recvBuffer.flags = _recvBuffer.transferBytes = 0;
	_recvBuffer.connexion_id = id;

}

connection::~connection()
{

	if (_session)
	{
		delete _session;
		_session = NULL;
	}

	DeleteCriticalSection(&_cryptLock);
	DeleteCriticalSection(&_syncLock);

	for (uint8 i = 0; i < SBS_CLEINT_SEND_QUEUE_SIZE; i++)
	{
		DeleteCriticalSection(&_sendSlots[i].lock_check);
	}


	if (_account.accountSettings)
	{
		delete[] _account.accountSettings;
		_account.accountSettings = NULL;
	}


	for (uint8 i = 0; i < SC_PLAYER_MAX_CHARACTER_COUNT; i++)
	{
		if (_players[i])
		{
			delete _players[i];
			_players[i] = NULL;
		}
	}

}

bool connection::connexion_init(HANDLE master_iocp)
{
	_iocp = CreateIoCompletionPort((HANDLE)_socket, master_iocp, NULL, 0);
	if (NULL == _iocp)
		return false;

	if (!connexion_send_init(this))
		return false;

	if (!connexion_recv(this))
	{
		connexion_close(this);
		return false;
	}
	return true;
}

bool WINAPI connexion_close(connection * c)
{
	if (!c) return false;
	shutdown(c->_socket, SD_SEND);
	closesocket(c->_socket);

	return true;
}

connection* WINAPI connexion_create(SOCKET sock, HANDLE masterIOCP, uint32 id)
{
	connection * out = new connection(sock, id);
	if (!out->connexion_init(masterIOCP))
	{
#ifdef _DEBUG
		printf("SERVER FAILED TO CREATE CONNEXION\n");
#endif
		delete out;
		return NULL;
	}
	return out;
}

bool WINAPI connexion_send_init(connection * c)
{
	if (!c) return false;
	char initData[4] = { 1,0,0,0 };
	if (!send(c->_socket, initData, 4, 0))
		return false;
	return true;
}


//crypt
bool WINAPI connexion_crypt_init(connection * c)
{
	if (!c || !c->_session) return false;
	return c->_session->init_session();
}

bool WINAPI connexion_fill_key(connection * c, uint8 keyNo)
{
	if (!c || !c->_session) return false;
	return c->_session->fill_key(c->_recvBuffer.data._raw, keyNo);
}



//recv
bool WINAPI connexion_recv(connection * c)
{
	if (!c)
		return false;

	RECV_BUFFER * buffer = &c->_recvBuffer;
	job* newJob = new job;
	ZeroMemory(&newJob->ov, sizeof WSAOVERLAPPED);

	buffer->data._pos = 0;
	buffer->wsaBuff.buf = (char*)buffer->data._raw;
	buffer->wsaBuff.len = buffer->data._size;

	newJob->argv = new void*{ (void*)buffer };
	newJob->argc = 1;
	newJob->type = J_RECV;

	int32 ret = WSARecv(c->_socket, &buffer->wsaBuff, 1, &buffer->transferBytes, &buffer->flags, &newJob->ov, NULL);
	if (ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		if (ret != WSA_IO_PENDING)
		{
			delete[] newJob->argv;
			delete newJob;
			printf("?? %d\n", ret);
			return false;
		}

	}
	else
	{
		//printf("UAHH %d %d flags:%d\n", buffer->transferBytes, ret, buffer->flags);
	}

	return true;
}

bool WINAPI connexion_process_recv_packet(connection * c, void * argv[])
{
	if (!c) return false;

	if (c->_inLobby)
	{
		EnterCriticalSection(&c->_syncLock);
		if (c->_peddingPacketCount > 0)
		{
			printf("RECV [repositing...] id[%d] size[%d]\n", c->_testId, c->_recvBuffer.data._size);
			c->_testId++;
			job* newJob = new job;
			ZeroMemory(&newJob->ov, sizeof WSAOVERLAPPED);

			newJob->argv = new void*{ (void*)&c->_recvBuffer };
			newJob->argc = 1;
			newJob->type = J_RECV;
			return PostQueuedCompletionStatus(c->_iocp, sizeof job, NULL, (LPOVERLAPPED)newJob) ? true : false; //repost job in queue
		}
		LeaveCriticalSection(&c->_syncLock);
	}

	if (!TryEnterCriticalSection(&c->_cryptLock))
	{
		printf("RECV [repositing...] id[%d] size[%d]\n", c->_testId, c->_recvBuffer.data._size);
		c->_testId++;
		job* newJob = new job;
		ZeroMemory(&newJob->ov, sizeof WSAOVERLAPPED);

		newJob->argv = new void*{ (void*)&c->_recvBuffer };
		newJob->argc = 1;
		newJob->type = J_RECV;
		return PostQueuedCompletionStatus(c->_iocp, sizeof job, NULL, (LPOVERLAPPED)newJob) ? true : false; //repost job in queue
	}

	c->_session->Decrypt(c->_recvBuffer.data._raw, c->_recvBuffer.data._size);


	uint16 size = (uint16)((c->_recvBuffer.data._raw[1] << 8) | c->_recvBuffer.data._raw[0]);
	uint16 opcode = (uint16)((c->_recvBuffer.data._raw[3] << 8) | c->_recvBuffer.data._raw[2]);
	e_opcode eopcode = (e_opcode)opcode;


	bool hasBody = false, result = true;
	op_function op_fn = opcode_resolve(eopcode, &hasBody);
	if (!op_fn)
	{
		printf("::OP_FUNCTION NOT FOUND FOR OPCODE[%04x]\n", opcode);
		LeaveCriticalSection(&c->_cryptLock);
		c->_recvBuffer.data.Clear();
		c->_recvBuffer.data.Resize(4);
		c->_recvBuffer.wsaBuff.buf = (char*)c->_recvBuffer.data._raw;
		c->_recvBuffer.wsaBuff.len = 4;

		return connexion_recv(c); // repost recv request
	}
	else if (!hasBody)
	{
		LeaveCriticalSection(&c->_cryptLock);
		if (op_fn(c, argv))
		{
			c->_recvBuffer.data.Clear();
			c->_recvBuffer.data.Resize(4);
			c->_recvBuffer.wsaBuff.buf = (char*)c->_recvBuffer.data._raw;
			c->_recvBuffer.wsaBuff.len = 4;

			return	connexion_recv(c); // repost recv request
		}
		return false;
	}

	c->_recvBuffer.data.Clear();
	c->_recvBuffer.data.Resize(size - 4);
	c->_recvBuffer.wsaBuff.buf = (char*)c->_recvBuffer.data._raw;
	c->_recvBuffer.wsaBuff.len = size - 4;


	int32 ret = recv(c->_socket, (char*)c->_recvBuffer.data._raw, size - 4, 0);
	if (ret == 0 || ret == SOCKET_ERROR)
	{
		ret = WSAGetLastError();
		printf("PACKET BODY RECV FAILED WSAE[%d]", ret);
		return false;
	}


	c->_session->Decrypt(c->_recvBuffer.data._raw, c->_recvBuffer.data._size);
	LeaveCriticalSection(&c->_cryptLock);


	if (op_fn(c, argv))
	{
		c->_recvBuffer.data.Clear();
		c->_recvBuffer.data.Resize(4);
		c->_recvBuffer.wsaBuff.buf = (char*)c->_recvBuffer.data._raw;
		c->_recvBuffer.wsaBuff.len = 4;

		return	connexion_recv(c); // repost recv request
	}
	return false;
}


//send
bool WINAPI connexion_send(connection * c, Stream * data)
{
	if (!c)
		return false;

	EnterCriticalSection(&c->_cryptLock);
	c->_session->Encrypt(data->_raw, data->_size);
	LeaveCriticalSection(&c->_cryptLock);

	int8 index = -1;
	for (uint32 i = 0; i < SBS_CLEINT_SEND_QUEUE_SIZE; i++)
	{
		if (TryEnterCriticalSection(&c->_sendSlots[i].lock_check))
			if ((c->_sendSlots[i].lock_bit & 1))
			{
				LeaveCriticalSection(&c->_sendSlots[i].lock_check);
				continue;
			}
			else
			{
				c->_sendSlots[i].lock_bit = 1;
				LeaveCriticalSection(&c->_sendSlots[i].lock_check);



				c->_sendSlots[i].buffer.connexion_id = c->_id;
				c->_sendSlots[i].buffer.data.Clear();
				c->_sendSlots[i].buffer.data.Write(data->_raw, data->_size);

				c->_sendSlots[i].buffer.wsaBuff.buf = (char*)c->_sendSlots[i].buffer.data._raw;
				c->_sendSlots[i].buffer.wsaBuff.len = data->_size; //sizeof sent data
				index = i;

				break;
			}

	}

	if (index >= 0)
	{
		job * newJob = new job;
		ZeroMemory(&newJob->ov, sizeof OVERLAPPED);
		newJob->type = J_SEND;
		newJob->argv = new  void*[2];
		newJob->argv[0] = (void*)&c->_id;
		newJob->argv[1] = (void*)&c->_sendSlots[index].index;
		newJob->argc = 2;

		uint32 ret = WSASend(c->_socket, &c->_sendSlots[index].buffer.wsaBuff, 1, &c->_sendSlots[index].buffer.transferBytes, c->_sendSlots[index].buffer.flags, (LPWSAOVERLAPPED)newJob, NULL);
		if (ret == SOCKET_ERROR)
		{
			ret = WSAGetLastError();
			if (ret != WSA_IO_PENDING)
			{
				c->_droppedPacketCount++;

				printf("SEND PACKET DROPED index[%d] droppedCount[%d]\n", index, c->_droppedPacketCount);
				delete[] newJob->argv;
				delete newJob;

				if (c->_inLobby && c->_droppedPacketCount > 0 ||
					c->_droppedPacketCount >= CS_MAX_DROP_PACKET_COUNT)
					server_connexion_remove(c->_id);

				return false;
			}
			else
			{
				EnterCriticalSection(&c->_syncLock);
				c->_peddingPacketCount++;
				LeaveCriticalSection(&c->_syncLock);
			}
		}
		else
		{
			EnterCriticalSection(&c->_syncLock);
			c->_peddingPacketCount++;
			LeaveCriticalSection(&c->_syncLock);
		}
	}
	else
	{

		c->_droppedPacketCount++;
		if (c->_inLobby && c->_droppedPacketCount > 0 ||
			c->_droppedPacketCount >= CS_MAX_DROP_PACKET_COUNT)
		{
			server_connexion_remove(c->_id);
			return false;
		}

	}
	return true;
}

bool WINAPI connexion_send(connection * c, BYTE * data, uint32 size)
{
	if (!c || !data)
		return false;
	return send(c->_socket, (const char*)data, size, 0) == 0 ? false : true;
}

bool WINAPI connexion_finish_send(connection * c, uint16 slot_index)
{
	if (!c || slot_index >= SBS_CLEINT_SEND_QUEUE_SIZE)
		return false;

	EnterCriticalSection(&c->_sendSlots[slot_index].lock_check);
	c->_sendSlots[slot_index].lock_bit = 0;
	LeaveCriticalSection(&c->_sendSlots[slot_index].lock_check);

	EnterCriticalSection(&c->_syncLock);
	c->_peddingPacketCount--;
	LeaveCriticalSection(&c->_syncLock);

	//server_lock_log();
	//printf("SEND FINISHED index[%d]\n", slot_index);
	//server_unlock_log();
	return true;

}



int8 WINAPI connexion_can_create_player(connection * c)
{
	if (!c)
		return -1;

	for (uint8 i = 0; i < SC_PLAYER_MAX_CHARACTER_COUNT; i++)
	{
		if (c->_players[i] == NULL)
			return i;
	}

	return -1;
}


