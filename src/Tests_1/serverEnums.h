#ifndef SERVERENUMS_H
#define SERVERENUMS_H

#include "typeDefs.h"

enum serve_request : unsigned long
{
	NONE = 0,
	CONNEXION = 1
};

enum server_contants : unsigned long
{
	SC_PLAYER_NAME_MAX_LENGTH = 38,
	SC_PLAYER_MAX_CHARACTER_COUNT = 6,
	SC_PLAYER_DETAILS_1_BUFFER_SIZE = 32,
	SC_PLAYER_DETAILS_2_BUFFER_SIZE = 64,
	SC_PLAYER_DETAILS_3_BUFFER_SIZE = 8
};

enum server_buffer_settings :uint32
{
	SBS_SERVER_MAX_CLIENTS = 1000,
	SBS_CLEINT_SEND_QUEUE_SIZE = 16,
};

#endif // 
