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
#include <algorithm>
#include "cache.h"
#include "test_classes.h"
#include "SimpleClass.h"
#include <mutex>
#include <iterator>
#include <string_view>
#include <cassert>
#include <stdexcept>
#include <queue>
#include <map>


namespace rest {
	int variable1;
};

namespace rest2 {
	int variable1;
};

void decorator_test(void) {

	rest::variable1 = 3;

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
#define SECTORS_AMOUNT 4
#define PUMP_IDLETIME_REQUIRED_SEC 30
#define PUMP_RUNTIME_LIMIT_SEC 300

void builder_test(void) {
	std::cout << "-------------------------------builder test-----------------------------------" << std::endl;
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


	std::unique_ptr<IrrigationSector>(p_sector2);
	sector_builder->ProducePartC()
		.ProducePartA()
		.ProducePartA();
	sector_builder->ProducePartA();
	sector_builder->ProducePartC();
	sector_builder->producePlantWithDMAMoistureSensor("Pelargonia1");
	sector_builder->producePlantWithDMAMoistureSensor("Kroton1");
	sector_builder->producePlantWithDMAMoistureSensor("Kroton2");
	sector_builder->producePlantWithDMAMoistureSensor("Truskawka1");
	sector_builder->producePlantWithDMAMoistureSensor("Truskawka2");
	sector_builder->produceDRV8833PumpWithController(pump_controller_mode_t::external, 60, 300, pump1gpio, pump1led, pump1fault, pump1mode);
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
	if (p_sector2->setPlantMoistureByName("Pelargonia", 89)) std::cout << "setPlantMoistureByName success"  << std::endl;
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
	p_watertank->update(0.02_sec);
	std::cout << error_code << std::endl;

	IrrigationSectorInfo_s sector2_mail;

	sector2_mail = p_sector2->getInfo();
	std::cout << "-----------------------------sector2_mail: "<<sector2_mail.plants_aliases << std::endl;
	//p_watertank->setHeight(78.0_cm);
	//std::cout << "literals-----------------" << p_watertank->getHeightMeters() << std::endl;


	std::cout << "-----------------------------sectors_builder start: " << std::endl;

	std::array<std::array<struct gpio_s, 2>, SECTORS_AMOUNT> pump_ctrl_gpio;
	pump_ctrl_gpio[0] = { {{ 0, 0 }, { 0, 0 }} };
	pump_ctrl_gpio[1] = { {{ 0, 0 }, { 0, 0 }} };
	pump_ctrl_gpio[2] = { {{ 0, 0 }, { 0, 0 }} };
	pump_ctrl_gpio[3] = { {{ 0, 0 }, { 0, 0 }} };
	const struct gpio_s pump_led_gpio[SECTORS_AMOUNT] = { {0, 0},{0, 0},{0, 0},{0, 0} };
	const struct gpio_s pump_fault_gpio[SECTORS_AMOUNT] = { {0, 0},{0, 0},{0,0},{0, 0} };
	const struct gpio_s pump_mode_gpio[SECTORS_AMOUNT] = { {0,0},{0, 0},{0, 0}, {0,0} };
	const struct gpio_s opt_wl_sensor1_gpio = { 1, 1 };
	std::unique_ptr<IrrigationSector>(p_sector[SECTORS_AMOUNT]);


	ConcreteIrrigationSectorBuilder* sectors_builder = new ConcreteIrrigationSectorBuilder[SECTORS_AMOUNT]; //leave as pointer to delete when not needed anymore
	for (uint8_t i = 0; i < SECTORS_AMOUNT; ++i) {
		sectors_builder[i].produceDRV8833PumpWithController(pump_controller_mode_t::external, PUMP_IDLETIME_REQUIRED_SEC, PUMP_RUNTIME_LIMIT_SEC, pump_ctrl_gpio[i], pump_led_gpio[i], pump_fault_gpio[i], pump_mode_gpio[i]);
		sectors_builder[i].ProducePartA();
	}

	for (uint8_t i = 0; i < SECTORS_AMOUNT; ++i) {
		p_sector[i] = sectors_builder[i].GetProduct();
		p_sector[i]->ListParts();
	}

	delete[] sectors_builder;
	std::cout << "-----------------------------sectors_builder finished" << std::endl;
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
	if (sensor4->getType() == sensor_type_t::temperature_sensor) std::cout << "temp sensor" << std::endl;
	if (sensor3->getType() == sensor_type_t::waterflow_sensor) std::cout << "waterflow sensor" << std::endl;
	if (sensor2->getType() == sensor_type_t::waterlevel_sensor) std::cout << "wl sensor" << std::endl;
	if (sensor1->getType() == sensor_type_t::generic_sensor) std::cout << "gen sensor" << std::endl;

	delete sensor1;
	delete sensor2;
	delete sensor3;
	delete sensor4;

	bool is_submersed = true;

	is_submersed = static_cast<bool>(vSensors.back()->read());
	//vSensors.back()->getResult(is_submersed);

	if (is_submersed == false) std::cout << "Submersed false" << std::endl;
	if (vSensors.at(0)->getType() == sensor_type_t::waterflow_sensor) std::cout << "vSensors waterflow sensor" << std::endl;

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

	if (_abs_time_ms < 9.99_sec) {
		if (double_equals(_abs_time_ms, 1.0_sec)) {tank_delay.set_state_and_update(true, _abs_time_ms); std::cout << "1000_msec " << std::endl;}
		if (double_equals(_abs_time_ms, 2.0_sec)) { tank_delay.set_state_and_update(true, _abs_time_ms); std::cout << "2000_msec " << std::endl; }
		else tank_delay.set_state_and_update(true, _abs_time_ms);
	}
	else if (_abs_time_ms >= 9.99_sec && _abs_time_ms < 20.0_sec) {
		if (double_equals(_abs_time_ms, 10.0_sec, 0.01_sec)) { tank_delay.set_state_and_update(false, _abs_time_ms); std::cout << "10_sec " << std::endl;}
		if (double_equals(_abs_time_ms, 11.0_sec, 0.01_sec)) { tank_delay.set_state_and_update(false, _abs_time_ms); std::cout << "11_sec " << std::endl; }
		else tank_delay.set_state_and_update(false, _abs_time_ms);
	}

	std::cout << "time: " << _abs_time_ms << ", state: " << state << std::endl;

}


#define LOG_TEXT_LEN 26
#define LOG_FORMAT "%02d-%02d-%02d %02d:%02d:%02d %s: %s\n"
#define LOG_FORMAT_LEN (sizeof(LOG_FORMAT))


enum msg_priority_t : uint8_t {
	INFO,
	LOW,
	MEDIUM,
	HIGH,
	CRITICAL
};

struct log_msg {
	char 			text[LOG_TEXT_LEN]{};
	uint8_t			len{};
	msg_priority_t	priority{};
};

#define MINIMUM(a,b)            (((a) < (b)) ? (a) : (b))
#define MAXIMUM(a,b)            (((a) > (b)) ? (a) : (b))
uint16_t publishLogMessage(std::string_view msg_txt, const uint8_t &_maxlen)
{
	std::string test_str{ msg_txt };
	uint16_t retval = 0x00;

	log_msg msg = {"", _maxlen };

	std::cout << "lengths: " << msg_txt.length() << " " << std::to_string(_maxlen) << std::endl;
	std::cout << "msg_txt: " << msg_txt << std::endl;
	std::cout << "msg->text: " << msg.text << std::endl;

	std::cout <<"string_view vs string: "<< sizeof(msg_txt) << " "<<sizeof(test_str) << std::endl;
	//msg_txt.shrink_to_fit();
	if (msg_txt.length() >= _maxlen) {
		msg_txt = msg_txt.substr(msg_txt.length() - _maxlen + 1, _maxlen - 1);
		std::cout << "substr msg_txt: " << msg_txt << std::endl;
		retval = 0x0001;
	}
	msg.len = msg_txt.length() + 1;
	msg_txt.copy(msg.text, msg_txt.length(), 0);
	msg.text[MINIMUM((_maxlen - 1), msg_txt.length())] = '\0';

	std::cout << "lengths: " << msg_txt.length() << " " << std::to_string(msg.len) << std::endl;
	std::cout << "msg_txt: " << msg_txt << std::endl;
	std::cout << "msg->text: " << msg.text << std::endl;


	return retval;
}

void pointer_reference_test() {
	int test = 7;
	int arr_test[5] = { 0,1,2,3,4 };
	


	int *p_test = &test;
	int& r_test = test;
	int **pp_test = &p_test;
	int test2 = *p_test;

	std::cout << "pointer_reference_test:" << std::endl;
	std::cout << "variable val: " << test << " "<< &test<<std::endl;
	std::cout << "p_test vals: " << p_test <<" "<< *p_test <<std::endl;
	std::cout << "pp_test vals: " << pp_test << " " << *pp_test << " " << **pp_test << " " << *(*pp_test) << std::endl;
	std::cout << "r_test vals: " << r_test <<" "<< &r_test << std::endl;

	for (int *i=std::begin(arr_test); i!=std::end(arr_test); i++ )
	{
		//*(arr_test++);
		std::cout << *i <<" "<< i<<" "<<sizeof(i)<< sizeof(*i)<<std::endl;
	}

	for (auto &i : arr_test) {
		std::cout << i << std::endl;
	}
}

void singleton_test() {
	Cache* cache1 = Cache::GetInstance("FOO");
	Cache* cache2 = Cache::GetInstance("BAR");
	Cache* cache3 = Cache::GetInstance("HAH");
	std::cout << cache1->getValue() << std::endl;
	std::cout << cache2->getValue() << std::endl;
	std::cout << cache3->getValue() << std::endl;


}

void _2d_array_memory_allocation_test()
{
	std::cout << "started 2d allocation" << std::endl;
	int rows = 3, columns = 4;
	int *arr = (int *)malloc(rows * columns * sizeof(int));

	int i, j, count = 0;
	for (i = 0; i < rows; i++)
		for (j = 0; j < columns; j++)
		{
			*(arr + i * columns + j) = std::rand();
		}


	std::string line;
	int value = 0;
	count = 0;
	for (i = 0; i < rows; i++){
		for (j = 0; j < columns; j++) {
			value = *(arr + i * columns + j);
			line += "|" + std::to_string(value);
			++count;
			if (count == rows) {
				count = 0;
				std::cout << line <<"|"<< std::endl;
				line.clear();
			}
		}
	}
	std::cout << "finished 2d allocation" << std::endl;
}

template<typename T, int size> 
void print_array(T(&ref)[size]) {
	for (int i = 0; i < size; ++i) {
		std::cout << ref[i] << " " << std::endl;
	}
}
template<typename T, int rows, int cols>
void print_2darray(T(&ref)[rows][cols]) {
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			std::cout << ref[i][j] << " ";
		}
	}
	std::cout<<std::endl;
}

void heapbased_2darray(const int _rows, int _cols) {
	const int rows = _rows;
	const int cols = _cols;
	assert(rows > 0);
	assert(cols > 0);
	if (rows <= 0 || cols <= 0) {
		throw std::runtime_error("Bad input");
	}

	int *array_values = new int[rows*cols];

	for (int i = 0; i < (rows*cols); i++) {
		array_values[i] = rand();
		std::cout << array_values[i] << std::endl;
	}


	int **pp_arr = new int *[rows];
	for (int i = 0; i < rows; ++i) {
		pp_arr[i] = new int[cols];
	}

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			pp_arr[i][j] = array_values[i*cols + j%cols];
		}
	}
	/* 0+0-> 0*3+0%3 -> 0
		0+1->1 0*3+1%3->1
		0+2->2 0*3+2%3 ->2
		1+0->3 1*3+0%3 ->3
		1+1->4 1*3+1%3 ->4
		1+2->5 1*3+2%3 ->5
		*/
	delete[] array_values;

	std::cout << "resulting array is: " << std::endl;
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			std::cout << pp_arr[i][j]<<" ";
		}
		std::cout << std::endl;
	}
}

void placement_new() {
	const uint8_t tank_cnt = 5;
	Watertank *ptr_tank = static_cast<Watertank*>(operator new(sizeof(Watertank) * tank_cnt));
	for (uint8_t i = 0; i < tank_cnt; ++i) {
		new (ptr_tank + i)Watertank{i};
	}
	std::cout << sizeof(ptr_tank) <<"B "<< sizeof(*ptr_tank) <<"B "<<sizeof(*ptr_tank)/1024.0<<"kB"<< std::endl;

	for (uint8_t i = 0; i < tank_cnt; ++i) {
		ptr_tank[i].~Watertank(); //operator new requires to call destructors manually
	}

	operator delete(ptr_tank);


}



typedef std::pair<msg_priority_t, log_msg*> PAIR;
struct cmp {
	bool operator()(const PAIR &a, const PAIR &b) {
		return a.first < b.first;
	};
};

void priority_queue() {
	log_msg *msg1 = new log_msg{ "1Irrigation Ctrl task sta", 0, msg_priority_t::LOW };
	log_msg *msg2 = new log_msg{ "2Irrigation Ctrl task sta", 0, msg_priority_t::HIGH };
	log_msg *msg3 = new log_msg{ "3Irrigation Ctrl task sta", 0, msg_priority_t::LOW };
	std::multimap<msg_priority_t, log_msg*> msg_map = {};// {msg1->priority, msg1}, { msg2->priority, msg2 }, { msg3->priority, msg3 }};
	msg_map.insert({ msg1->priority, msg1 });
	msg_map.insert({ msg2->priority, msg2 });
	msg_map.insert({ msg3->priority, msg3 });
	log_msg *msg4 = new log_msg{ "4Irrigation Ctrl task sta", 0, msg_priority_t::CRITICAL };
	msg_map.insert({ msg4->priority, msg4 });
	std::priority_queue<PAIR, std::vector<PAIR>, cmp> logs_queue;
	for (const auto &msg: msg_map ) {
		logs_queue.push(msg);
	}

	std::cout << logs_queue.size() << std::endl;
	std::cout << LOG_FORMAT_LEN << std::endl;

	while (!logs_queue.empty()) {
		PAIR top = logs_queue.top() ;
		std::cout << top.second->text << " " << std::endl;
		logs_queue.pop();

	}

	delete msg1;
	delete msg2;
	delete msg3;
	delete msg4;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::cout << "Hello World!\n";
	double dt = 0.0_sec;
	bool watering = false;

	tank_delay.set_hysteresis_time_from(false, 2000.0_msec);
	tank_delay.set_hysteresis_time_from(true, 2000.0_msec);

	std::vector<std::string> vPlants;
	vPlants.push_back("Kroton1");
	vPlants.push_back("Kroton2");
	std::string plants;

	std::cout << plants << std::endl;

	for (auto &plant: vPlants) plants += plant.substr(0, 3) + plant.substr(plant.length()-1, 1) + ";";
	std::cout << plants << std::endl;

	//Sleep(5000);

	float z = 2.0;
	IBase *baseobject = new ImpBase();
	Client clientobject(baseobject);

	Derived obj;

	SimpleClass simpleobj;
	Plant *plant = new Plant("Kroton", 1);

	simpleobj.createSth(plant);

	delete baseobject;
	delete plant;

	int arr[] = { 0, 9, 8, 7, 6, 0 };
	int arr_2[2][3]{ 1,2,3,4,5,6 };
	//int (*p_arr_2)[3] = arr_2;

	//print_array(arr);
	//print_2darray(arr_2);

	int arr1[5] = { 1,2,3,4,5 };
	std::array<int, 5> arr2{ 1,2,3,4,5 };

	//std::cout << "Arrays sizes are: " << sizeof(arr1) <<" "<< sizeof(arr2) << std::endl;

	int *ptr = new int{5};
	delete ptr;

	int sth = 1;
	ptr = &sth;

	void *ptr1;
	ptr1 = new int{ 4 };
	delete ptr1;

	const int rows = 2;
	const int cols = 3;
	//std::cout << *ptr << std::endl;
	//heapbased_2darray(rows,cols);


	while (1) {
		//decorator_test();
		//builder_test();
		//bridge_test();
		//pointer_reference_test();
		//_2d_array_memory_allocation_test();
		//singleton_test();
		
		//obj.func(z);
		//clientobject.update(z);
		//placement_new();
		priority_queue();

		

		if (dt < 15.0_sec) watering = true;
		else if (dt >= 15.0_sec && dt < 30.0_sec) watering = false;
		else {
			dt = 0.0_sec;
			watering = true;
		}

		//publishLogMessage("Irrigation Ctrl task started", LOG_TEXT_LEN);
		//controller_test(watering, 1);

		//if (watering) std::cout << "controller1 update: " << dt << " watering true" << std::endl;
		//else std::cout << "controller1 update: " << dt << " watering false" << std::endl;
		
		Sleep(100);
		dt += 100.0_msec;
		//hysteresis_test(dt);

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
