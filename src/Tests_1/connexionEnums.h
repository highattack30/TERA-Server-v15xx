#ifndef CONNEXIONENUMS_H
#define CONNEXIONENUMS_H

#include "typeDefs.h"


//connection phase
enum e_connexion_recv_buffer_state : uint8
{
	CP_KEY_1 = 1,
	CP_KEY_2 = 2,

	CP_TERA_PACKET = 3,

	CP_MAX
};

enum e_connexion_settings :uint32 {
	CS_STREAM_MAX_SIZE = 0xffff,
	CS_MAX_DROP_PACKET_COUNT = 4,

	CS_MAX
};



#endif
