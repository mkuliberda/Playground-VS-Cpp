#pragma once
#include <string_view>
class Connection;
class Command;

class DBFactory
{
public:
	static Command* CreateCommand(std::string_view type);
	static Connection* CreateConnection(std::string_view type);
};

