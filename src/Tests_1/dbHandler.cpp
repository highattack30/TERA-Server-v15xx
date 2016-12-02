#include "DBHandler.h"
#include "config.h"


bool WINAPI mysql_init()
{
	mysqlDriver =  new MySqlDriver();

	return mysqlDriver->initDriver(config::mysql.mysqlIp, config::mysql.mysqlUser, config::mysql.mysqlPassword, config::mysql.mysqlDbName);
}

void WINAPI mysql_release()
{
	if (mysqlDriver)
	{
		delete mysqlDriver;
		mysqlDriver = NULL;
	}
}

sql::PreparedStatement * WINAPI mysql_get_preapared_statememnt(sql::Connection * c, const char* query)
{
	return mysqlDriver->GetPreparedStatement(c, query);
}

MySqlDriver * WINAPI mysql_get_driver()
{
	return mysqlDriver;
}
