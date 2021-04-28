// Singleton_Logger.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iostream>
#include "Logger.h"



int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::cout << "Hello World!\n";

	log_msg sys_message = {};
	HAL_FatFs_Logger &logger = HAL_FatFs_Logger::createInstance();
	logger.writeLog(&sys_message);

	HAL_FatFs_Logger &logger1 = HAL_FatFs_Logger::createInstance();
	logger1.writeLog(&sys_message);

	std::cout << &logger1 << " " << &logger << std::endl;

	logger1.accumulateLogs();
	logger1.releaseLogsToFile();
	logger1.accumulateLogs();
	logger1.releaseLogsToFile();

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
