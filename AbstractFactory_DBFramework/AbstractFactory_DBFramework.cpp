// AbstractFactory_DBFramework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DbFactory.h"
#include "RecordSet.h"
#include "Connection.h"
#include "MySqlConnection.h"
#include "SqlConnection.h"
#include "MySqlCommand.h"
#include "SqlCommand.h"
#include <string_view>


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::cout << "Hello Abstract Factory example!\n";
	std::string_view db_type{ "sql" };

	Connection *pCon = DBFactory::CreateConnection(db_type) ;
	pCon->SetConnectionString("Just some connection string");
	pCon->Open();

	Command *pCmd = DBFactory::CreateCommand(db_type);
	pCmd->SetConnection(pCon);
	pCmd->SetCommand("Just some sample command");
	pCmd->ExecuteCommand();

	RecordSet *pRecSet = pCmd->ExecuteQuery();
	while (pRecSet->HasNext()) {
		std::cout << pRecSet->Get() << std::endl;
	}



	delete pCon;
	delete pCmd;
	delete pRecSet;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
