// ConsoleApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "main.h"
#include <bitset>
#include <stdlib.h>     //for using the function sleep
#include <windows.h>
#include "plants.h"
#include <vector>
#include "sector.h"


void decorator_test(void) {
	std::string name = "Pelargonia";
	uint8_t id = 0;
	PlantInterface *anPlantWithDMAMoistureSensor = new PlantWithDMAMoistureSensor(new Plant(name, 0), 3.3, 4095);
	std::cout << anPlantWithDMAMoistureSensor->getMoisturePercent() << std::endl;
	anPlantWithDMAMoistureSensor->updateRaw((uint16_t)3);
	std::cout << anPlantWithDMAMoistureSensor->getMoisturePercent() << std::endl;
	std::cout << anPlantWithDMAMoistureSensor->getName() << std::endl;
	std::cout << anPlantWithDMAMoistureSensor->getId() << std::endl;


	//for System Builder std::unique_pointer with vector will need to be used
	std::vector< std::unique_ptr<PlantInterface>> vPlants;
	vPlants.emplace_back(std::unique_ptr<PlantInterface>(new PlantWithDMAMoistureSensor(new Plant("Pelargonia", 0), 3.0, 4095)));
	std::cout<< "vPlants size: "<< vPlants.size() << " capacity: "<< vPlants.capacity()<<std::endl;
	vPlants.emplace_back(std::unique_ptr<PlantInterface>(new PlantWithDMAMoistureSensor(new Plant("Kroton", 0), 3.0, 4095)));
	std::cout << "vPlants size: " << vPlants.size() << " capacity: " << vPlants.capacity() << std::endl;
	vPlants.emplace_back(std::unique_ptr<PlantInterface>(new PlantWithDMAMoistureSensor(new Plant("Surfinia", 0), 3.0, 4095)));
	std::cout << "vPlants size: " << vPlants.size() << " capacity: " << vPlants.capacity() << std::endl;


	//plants.emplace_back()
	delete anPlantWithDMAMoistureSensor;
	std::cout << std::endl;

}

void builder_test(void) {

	SectorBuilder* sector1_builder = new SectorBuilder;
	sector1_builder->ProducePartA();
	sector1_builder->ProducePartB();
	sector1_builder->ProducePartC();
	std::unique_ptr<Sector>(p_sector1);
	p_sector1 = sector1_builder->GetProduct();
	delete sector1_builder;


	p_sector1->ListParts();

}


int main()
{
    std::cout << "Hello World!\n";
	while (1) {
		decorator_test();
		builder_test();
		Sleep(1000);
	}
	return 0;

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
