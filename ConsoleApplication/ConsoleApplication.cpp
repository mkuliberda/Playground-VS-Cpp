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
#include "hysteresis.h"


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
	uint32_t error_code = 0;
	TIM_HandleTypeDef Timer;
	TIM_HandleTypeDef *pTimer = &Timer;
	bool activate_watering1 = true;


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
	sector_builder->produceDRV8833PumpWithController(pump_controller_mode_t::external, 60, 300, pump1gpio, pump1led, pump1fault, pump1mode);
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
	p_sector2->pump_controller.update(0.02, activate_watering1);

	ConcreteWatertankBuilder* watertank_builder = new ConcreteWatertankBuilder; //leave as pointer to delete when not needed anymore
	watertank_builder->setWatertankHeight(75.7_cm);
	watertank_builder->setWatertankVolume(57.0_l);
	watertank_builder->ProducePartA();
	watertank_builder->ProducePartC();
	watertank_builder->produceOpticalWaterLevelSensor(3.8_cm, { 3,1 });
	watertank_builder->produceOpticalWaterLevelSensor(50.2_cm, { 3,2 });
	watertank_builder->produceOpticalWaterLevelSensor(87.0_cm, { 3,3 });
	watertank_builder->produceDS18B20TemperatureSensor({ 3,1 }, pTimer);
	std::unique_ptr<Watertank>(p_watertank);
	p_watertank = watertank_builder->GetProduct();

	delete watertank_builder;
	
	std::cout << "watertank parts" << std::endl;
	p_watertank->ListParts();
	p_watertank->update(0.02, error_code);
	std::cout << error_code << std::endl;
	//p_watertank->setHeight(78.0_cm);
	//std::cout << "literals-----------------" << p_watertank->getHeightMeters() << std::endl;

}


void controller_test(bool _watering, const double& _dt) {
	const struct gpio_s pump1gpio_in1 = { 0, 0 };
	const struct gpio_s pump1gpio_in2 = { 0, 1 };
	const std::array<struct gpio_s, 2> pump1gpio = { pump1gpio_in1, pump1gpio_in2 };
	const struct gpio_s pump1led = { 2, 0 };
	const struct gpio_s pump1fault = { 0, 2 };
	const struct gpio_s pump1mode = { 0, 3 };

	PumpController controller1;
	controller1.setMode(pump_controller_mode_t::external);
	controller1.createPump(pump_type_t::drv8833_dc, PumpId++, 10, 30, { pump1gpio_in1, pump1gpio_in2, 0, 0}, pump1led, pump1fault, pump1mode);
	controller1.update(_dt, _watering);
}

void bridge_test() {
	std::cout << "-------------------------------bridge test-----------------------------------" << std::endl;
	const struct gpio_s optwlsensor1gpio_in = { 3, 2 };
	std::vector<std::unique_ptr<Sensor>>(vSensors);
	TIM_HandleTypeDef Timer;
	TIM_HandleTypeDef *pTimer = &Timer;


	vSensors.emplace_back(new WaterFlowSensor({ 1, 2 }));
	vSensors.emplace_back(new OpticalWaterLevelSensor(0.3, { 3, 2 }));
	vSensors.emplace_back(new OpticalWaterLevelSensor(0.8, { 3, 1 }));

	Sensor *sensor1 = new Sensor(sensor_type_t::generic_sensor);
	Sensor *sensor2 = new OpticalWaterLevelSensor(0.2, { 3, 2 });
	Sensor *sensor3 = new WaterFlowSensor({ 1, 2 });
	Sensor *sensor4 = new DS18B20TemperatureSensor({ 1, 2 }, pTimer);

	sensor1->read();
	sensor2->read();
	sensor3->read();
	if (sensor4->getType() == sensor_type_t::temperature_sensor) std::cout << "------------------temp sensor" << std::endl;
	if (sensor3->getType() == sensor_type_t::waterflow_sensor) std::cout << "------------------waterflow sensor" << std::endl;
	if (sensor2->getType() == sensor_type_t::waterlevel_sensor) std::cout << "------------------wl sensor" << std::endl;
	if (sensor1->getType() == sensor_type_t::generic_sensor) std::cout << "------------------gen sensor" << std::endl;

	delete sensor1;
	delete sensor2;
	delete sensor3;
	delete sensor4;

	bool is_submersed = true;

	is_submersed = static_cast<bool>(vSensors.back()->read());
	//vSensors.back()->getResult(is_submersed);

	if (is_submersed == false) std::cout << "Submersed false" << std::endl;
	if (vSensors.at(0)->getType() == sensor_type_t::waterflow_sensor) std::cout << "-----------------------------vSensors waterflow sensor" << std::endl;

	Pump *pump0 = new Pump(0);
	Pump *pump1 = new DRV8833DcPump(1, 10, 30, { 0,1 }, { 5,1 }, { 1,1 }, { 2,1 });
	bool cmd_consumed = false;
	pump0->run(0.02, cmd_consumed, pump_cmd_t::start);
	pump1->run(0.02, cmd_consumed, pump_cmd_t::start);

	delete pump0;
	delete pump1;


}

Hysteresis tank_delay;


void hysteresis_test(const double& _abs_time_ms) {
	bool state = tank_delay.get_state();

	if (_abs_time_ms < 10.0_sec) {
		if (double_equals(_abs_time_ms, 1.0_sec)) {tank_delay.set_state_and_update(true, _abs_time_ms); std::cout << "1000_msec " << std::endl;}
		else tank_delay.update(_abs_time_ms);
	}
	else if (_abs_time_ms >= 10.0_sec && _abs_time_ms < 20.0_sec) {
		if (double_equals(_abs_time_ms, 10.0_sec, 0.1)) { tank_delay.set_state_and_update(false, _abs_time_ms); std::cout << "10_sec " << std::endl;}
		else tank_delay.update(_abs_time_ms);
	}

	std::cout << "time: " << _abs_time_ms << ", state: " << state << std::endl;

}


int main()
{
	std::cout << "Hello World!\n";
	double dt = 0.0_sec;
	bool watering = false;

	tank_delay.set_hysteresis_time_from(false, 2000.0_msec);
	tank_delay.set_hysteresis_time_from(true, 2000.0_msec);


	while (1) {
		decorator_test();
		builder_test();
		bridge_test();
		hysteresis_test(dt);

		if (dt < 15.0_sec) watering = true;
		else if (dt >= 15.0_sec && dt < 30.0_sec) watering = false;
		else {
			dt = 0.0_sec;
			watering = true;
		}
		//controller_test(watering, 1);

		//if (watering) std::cout << "controller1 update: " << dt << " watering true" << std::endl;
		//else std::cout << "controller1 update: " << dt << " watering false" << std::endl;
		dt+=100.0_msec;
		Sleep(100);
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
