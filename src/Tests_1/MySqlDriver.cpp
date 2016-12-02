#include "MySqlDriver.h"


MySqlDriver::MySqlDriver()
{
	_driver = 0;
}

MySqlDriver::~MySqlDriver()
{
	_driver = 0;
}


const bool MySqlDriver::initDriver(const char* hostAddress, const char* hostUsername, const char * hostPassword, const char* databaseName)
{
	_driver = sql::mysql::get_driver_instance(); // get driver
	if (!_driver)
		return false;

	_isValid = true;

	_hostAddress = sql::SQLString(hostAddress);
	_hostUsername = sql::SQLString(hostUsername);
	_hostPassword = sql::SQLString(hostPassword);
	_dbName = sql::SQLString(databaseName);


	printf("%s %s %s %s \n", _hostAddress.c_str(), _hostUsername.c_str(), _hostPassword.c_str(), _dbName.c_str());
	//try
	//{
	//	_connection = _driver->connect(hostAddress, hostUsername, hostPassword); //start connection
	//}
	//catch (sql::SQLException e) {}
	//
	//if (!_connection || !_connection->isValid())
	//	return false;
	//
	//_connection->setSchema(databaseName); //set databse
	//
	//_statement = _connection->createStatement(); //create statement to execute mysql query

	return true;
}



sql::PreparedStatement * MySqlDriver::GetPreparedStatement(sql::Connection * c, std::string query)
{
	if (!c)
		return NULL;

	return c->prepareStatement(query.c_str());
}

sql::Connection * MySqlDriver::open()
{
	sql::Connection * newConnecion = NULL;
	try
	{
		newConnecion = _driver->connect(_hostAddress, _hostUsername, _hostPassword);
	}
	catch (sql::SQLException & ex)
	{
		printf("SQL-EX:%s\n", ex.what());
	}

	if (!newConnecion)
		return NULL;
	else if (!newConnecion->isValid())
	{
		delete newConnecion;

		return NULL;
	}

	try
	{
		newConnecion->setSchema(_dbName);
	}
	catch (sql::SQLException & ex)
	{
		printf("SQL-EX:%s\n", ex.what());

		newConnecion->close();
		delete newConnecion;
		newConnecion = NULL;
	}
	return newConnecion;
}