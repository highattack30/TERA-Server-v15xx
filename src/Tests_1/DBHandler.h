#ifndef DBHANDLER_H
#define DBHANDLER_H

#include "socket.h"
#include "typeDefs.h"
#include "MySqlDriver.h"

static MySqlDriver  *mysqlDriver;


bool WINAPI mysql_init();

void WINAPI mysql_release();

sql::PreparedStatement * WINAPI mysql_get_preapared_statememnt(sql::Connection * c,const char* query);

MySqlDriver *  WINAPI mysql_get_driver();

#endif // DBHANDLER_H
