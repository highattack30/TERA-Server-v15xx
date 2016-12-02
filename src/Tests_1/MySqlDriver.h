#ifndef MYSQL_DRIVER_H
#define MYSQL_DRIVER_H

#include <vector>
#include <mutex>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <mysql_error.h>
#include <cppconn\resultset.h>
#include <cppconn\statement.h>
#include <cppconn\datatype.h>
#include <cppconn\prepared_statement.h>


#define MAX_CLIENTS_PER_CONNECTION 50

class MySqlDriver
{
public:
	MySqlDriver();
	~MySqlDriver();

public:
	const bool initDriver(const char* hostAddress, const char* hostUsername, const char * hostPassword, const char* databaseName);

	sql::PreparedStatement * GetPreparedStatement(sql::Connection * c ,std::string query);
	
	sql::Connection * open();

private:
	

	sql::Driver * _driver; 

	bool _isValid = false;

	sql::SQLString _hostAddress;
	sql::SQLString _hostUsername;
	sql::SQLString _hostPassword;
	sql::SQLString _dbName;
};
#endif
