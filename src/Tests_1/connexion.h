#ifndef CONNEXION_H
#define CONNEXION_H

#include "socket.h"
#include "typeDefs.h"
#include "serverEnums.h"
#include "connexionEnums.h"
#include "opcodeEnum.h"
#include "Crypt.hpp"
#include "player.h"
#include "DBHandler.h"
#include "Stream.h"
#include "account.h"

using namespace Crypt;
class connection; struct job; 

#ifndef OP_FUNCTION
typedef bool(WINAPI *op_function)(connection *, void* argv[]);
#endif

struct SEND_BUFFER
{
	WSABUF		wsaBuff;
	DWORD		transferBytes;
	DWORD		flags;

	Stream		data;

	uint32 connexion_id;
};

struct RECV_BUFFER
{
	WSABUF		wsaBuff;
	DWORD		transferBytes;
	DWORD		flags;

	Stream		data;

	e_connexion_recv_buffer_state recvStatus;
	uint32 connexion_id;
};

struct SEND_SLOT
{
	SEND_BUFFER buffer;
	uint16 index;
	uint8 lock_bit;
	CRITICAL_SECTION lock_check;
};

class connection
{
public:
	connection(SOCKET sock, uint32 id);
	~connection();

	bool connexion_init(HANDLE master_iocp),
		_inLobby;

	uint32
		_id,
		_testId;

	uint16 _peddingPacketCount;
	uint8 _droppedPacketCount;


	SOCKET _socket;
	HANDLE _iocp;

	RECV_BUFFER _recvBuffer;
	SEND_SLOT _sendSlots[SBS_CLEINT_SEND_QUEUE_SIZE];

	player * _players[SC_PLAYER_MAX_CHARACTER_COUNT];
	account  _account;

	CRITICAL_SECTION _syncLock;
	CRITICAL_SECTION _cryptLock;
	Session *  _session;
};

bool WINAPI connexion_close(connection * c);
connection* WINAPI connexion_create(SOCKET s, HANDLE masterIOCP, uint32 id);
bool WINAPI connexion_send_init(connection *c);

bool WINAPI connexion_fill_key(connection * c, uint8 keyNo);
bool WINAPI connexion_crypt_init(connection * c);

bool WINAPI connexion_recv(connection * c);
bool WINAPI connexion_process_recv_packet(connection * c, void * argv[]);

bool WINAPI connexion_send(connection * c, Stream * data);
bool WINAPI connexion_send(connection * c, BYTE * data, uint32 size);
bool WINAPI connexion_finish_send(connection * c, uint16 slot_index);


int8 WINAPI connexion_can_create_player(connection * c);

#endif