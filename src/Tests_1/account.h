#ifndef ACCOUNTDATA_H
#define ACCOUNTDATA_H

#include "DBHandler.h"
#include "typeDefs.h"
#include "serverEnums.h"

class connection;

typedef struct account
{
	char username[32];
	char password[33]; //md5 + \0

	bool isGm;

	uint32
		id,
		accountSettingsSize,
		coins,
		playerCount;

	uint64
		remainingPlayTime,
		lastOnlineUTC;

	byte * accountSettings;

} account, *lpaccount_data;


bool account_perform_login(connection * c ,sql::Connection * con , byte ticket[33], byte username[SC_PLAYER_NAME_MAX_LENGTH]);
bool account_upadate(connection * c, sql::Connection * con);

#endif // 
