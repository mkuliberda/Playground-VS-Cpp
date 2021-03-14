#include "MySqlCommand.h"
#include "MySqlRecordSet.h"
#include "Connection.h"
#include <iostream>

void MySqlCommand::ExecuteCommand()
{
	std::cout<<__FUNCSIG__
		<< " Executing command on: "
		<< m_pConnection->GetConnectionString()
		<< std::endl;
}

MySqlRecordSet * MySqlCommand::ExecuteQuery()
{
	std::cout << __FUNCSIG__ << std::endl;
	return new MySqlRecordSet();
}
