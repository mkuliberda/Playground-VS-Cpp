#include "SqlCommand.h"
#include "Connection.h"
#include <iostream>

void SqlCommand::ExecuteCommand() {
	std::cout
		<< __FUNCSIG__
		<<" Executing command on: "
		<< m_pConnection->GetConnectionString()
		<< std::endl;
}

SqlRecordSet* SqlCommand::ExecuteQuery() {
	std::cout << __FUNCSIG__<<std::endl;
	return new SqlRecordSet();
}