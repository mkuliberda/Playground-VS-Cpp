#include "DbFactory.h"
#include "MySqlCommand.h"
#include "SqlCommand.h"
#include "MySqlConnection.h"
#include "SqlConnection.h"

Command * DBFactory::CreateCommand(std::string_view type)
{
	if (type == "sql") {
		return new SqlCommand{};
	}
	else if (type == "mysql") {
		return new MySqlCommand{};
	}
	return nullptr;
}

Connection * DBFactory::CreateConnection(std::string_view type)
{
	if (type == "sql") {
		return new SqlConnection{};
	}
	else if (type == "mysql") {
		return new MySqlConnection{};
	}
	return nullptr;
}
