// FactoryMethod_Parametrized.cpp : This file contains the 'main' function. Program execution begins and ends there.
// DocumentFactory class is the main thing that makes the Parametrized Factory Method different from Factory Method

#include <iostream>
#include "Application.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::cout << "Hello Parametrized Factory Method!\n";
	Application app;

	app.New("pit.txt");
	app.Open("pit.txt");
	app.Save();
	
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
