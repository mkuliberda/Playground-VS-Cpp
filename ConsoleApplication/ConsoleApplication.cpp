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
#include "pumps.h"
#include "watertank.h"
#include "sensors.h"


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

	if (vPlants.at(1)->getPlantType() == plant_type_t::plant_with_dma_moisture_sensor) {
		std::cout << "plant_type::plant_with_dma_moisture_sensor" << std::endl;
	}

	//plants.emplace_back()
	delete anPlantWithDMAMoistureSensor;
	std::cout << std::endl;

}

void builder_test(void) {
	const struct gpio_s pump1gpio_in1 = { 0, 0 };
	const struct gpio_s pump1gpio_in2 = { 0, 1 };
	const std::array<struct gpio_s, 2> pump1gpio = { pump1gpio_in1, pump1gpio_in2 };
	const struct gpio_s pump1led = { 2, 0 };
	const struct gpio_s pump1fault = { 0, 2 };
	const struct gpio_s pump1mode = { 0, 3 };


	ConcreteIrrigationSectorBuilder* sector_builder = new ConcreteIrrigationSectorBuilder; //leave as pointer to delete when not needed anymore
	sector_builder->ProducePartA();
	sector_builder->ProducePartB();
	sector_builder->ProducePartC();
	std::unique_ptr<IrrigationSector>(p_sector1);
	p_sector1 = sector_builder->GetProduct();

	sector_builder->ProducePartA();
	sector_builder->ProducePartC();
	sector_builder->producePlantWithDMAMoistureSensor("Pelargonia");
	sector_builder->producePlantWithDMAMoistureSensor("Kroton");
	sector_builder->producePumpWithController(pumpcontrollermode_t::external, 60, 300, pump1gpio, pump1led, pump1fault, pump1mode);
	std::unique_ptr<IrrigationSector>(p_sector2);
	p_sector2 = sector_builder->GetProduct();

	delete sector_builder;

	p_sector1->ListParts();
	p_sector2->ListParts();
	int id = p_sector2->vPlants.at(0)->getId();
	std::cout << id << std::endl;
	id = p_sector2->vPlants.at(1)->getId();
	std::cout << id << std::endl;
	p_sector2->vPlants.at(1)->isRainExposed();
	bool water = false;
	p_sector2->pump_controller.update(1, water);
	p_sector2->update();
	if (p_sector2->setPlantMoistureByName("Pelargonia", 89)) std::cout << "-----------------------------setPlantMoistureByName success"  << std::endl;
	std::cout << "Pelargonia health: "<< p_sector2->getPlantHealth("Pelargonia") << std::endl;

	ConcreteWatertankBuilder* watertank_builder = new ConcreteWatertankBuilder; //leave as pointer to delete when not needed anymore
	watertank_builder->ProducePartA().ProducePartC();
	std::unique_ptr<Watertank>(p_watertank);
	p_watertank = watertank_builder->GetProduct();

	delete watertank_builder;
	
	p_watertank->ListParts();

}


void controller_test(bool _watering, const double& _dt) {
	const struct gpio_s pump1gpio_in1 = { 0, 0 };
	const struct gpio_s pump1gpio_in2 = { 0, 1 };
	const std::array<struct gpio_s, 2> pump1gpio = { pump1gpio_in1, pump1gpio_in2 };
	const struct gpio_s pump1led = { 2, 0 };
	const struct gpio_s pump1fault = { 0, 2 };
	const struct gpio_s pump1mode = { 0, 3 };

	PumpController controller1;
	controller1.setMode(pumpcontrollermode_t::external);
	controller1.createPump(pumptype_t::drv8833_dc);
	controller1.p8833Pump->init(0, 60, 300, pump1gpio, pump1led, pump1fault, pump1mode);

	controller1.update(_dt, _watering);
}

void bridge_test() {
	std::cout << "-------------------------------bridge test-----------------------------------" << std::endl;
	const struct gpio_s optwlsensor1gpio_in = { 3, 2 };
	std::vector<std::unique_ptr<Sensor>>(vSensors);

	vSensors.emplace_back(new OpticalWaterLevelSensor(0.3, { 3, 2 }));
	vSensors.emplace_back(new OpticalWaterLevelSensor(0.8, { 3, 3 }));


	Sensor *sensor1 = new Sensor(sensor_type_t::generic_sensor);
	Sensor *sensor2 = new OpticalWaterLevelSensor(0.2, { 3, 2 });
	Sensor *sensor3 = new ZuluSensor(6);

	sensor1->read();
	sensor2->read();
	sensor3->read();
	if (sensor3->getType() == sensor_type_t::temperature_sensor) std::cout << "------------------temp sensor" << std::endl;
	if (sensor2->getType() == sensor_type_t::waterlevel_sensor) std::cout << "------------------wl sensor" << std::endl;
	if (sensor1->getType() == sensor_type_t::generic_sensor) std::cout << "------------------gen sensor" << std::endl;

	delete sensor1;
	delete sensor2;
	delete sensor3;

	bool is_submersed = true;

	vSensors.back()->read();
	vSensors.back()->getResult(is_submersed);

	if (is_submersed == false) std::cout << "Submersed false" << std::endl;

}


int main()
{
	std::cout << "Hello World!\n";
	double dt = 0;
	bool watering = false;

	while (1) {
		decorator_test();
		builder_test();
		bridge_test();

		if (dt < 15) watering = true;
		else if (dt >= 15 && dt < 30) watering = false;
		else {
			dt = 0;
			watering = true;
		}
		//controller_test(watering, 1);

		if (watering) std::cout << "controller1 update: " << dt << " watering true" << std::endl;
		else std::cout << "controller1 update: " << dt << " watering false" << std::endl;
		dt++;
		Sleep(50);
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
