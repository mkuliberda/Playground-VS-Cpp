#include "sector.h"

void IrrigationSector::encodeErrors() const {
	//TODO: implement this
}

void IrrigationSector::encodeState() const {
	//TODO: implement this
}

const uint8_t&  IrrigationSector::getId() {
	return sector_info.id;
}

const uint8_t&  IrrigationSector::getPlantsCountLimit() {
	return plants_count_limit;
}

uint8_t  IrrigationSector::getPlantsCount() const {
	return static_cast<uint8_t>(vPlants.size());
}
float IrrigationSector::getPlantHealth(const std::string& _name) const {
	for (auto &plant : vPlants) {
		if (_name.compare(plant->getName()) == 0) return plant->getMoisturePercent();
	}
	return -1;
}

float IrrigationSector::getPlantHealth(const uint8_t& _id) const {
	for (auto &plant : vPlants) {
		if (plant->getId() == _id) return plant->getMoisturePercent();
	}
	return -1;
}

void IrrigationSector::setWateringState(const bool& _watering_active) {
	watering = _watering_active;
}

const bool& IrrigationSector::getWateringState() {
	return watering;
}

void IrrigationSector::update() const {
	encodeState();
	encodeErrors();
}

const struct IrrigationSectorInfo_s&	IrrigationSector::getInfo() {
	update();
	//TODO: implement this
	//sector_info.state = 0; //todo
	//sector_info.errors = 0; //todo
	//for (auto &plant : vPlants) {
	//	plants += plant->getName();
	//	plants += ",";
	//}
	//std::strcpy(sector_info.plants, plants.c_str());
	return sector_info;
}

void IrrigationSector::addPlantNameToLIst(const std::string&& _plant_name) const {
	//TODO: implement this
}

bool IrrigationSector::setPlantMoistureByName(const std::string& _plant_name, const float& _moisture_percent) const {
	for (auto &plant : vPlants) {
		if (plant->getName() == _plant_name) {
			return plant->setMoisturePercent(_moisture_percent);
		}
	}
	return false;
}


void ConcreteIrrigationSectorBuilder::Reset() {
	sector.reset(new IrrigationSector(static_cast<uint8_t>(++sector_count)));
}
/**
 * All production steps work with the same sector instance.
 */

ConcreteIrrigationSectorBuilder& ConcreteIrrigationSectorBuilder::ProducePartA(){  //TODO: delete this on STM32
	sector->parts_.push_back("PartA1");
	return *this;
}

ConcreteIrrigationSectorBuilder& ConcreteIrrigationSectorBuilder::ProducePartB(){  //TODO: delete this on STM32
	sector->parts_.push_back("PartB1");
	return *this;
}

ConcreteIrrigationSectorBuilder& ConcreteIrrigationSectorBuilder::ProducePartC(){  //TODO: delete this on STM32
	sector->parts_.push_back("PartC1");
	return *this;
}

ConcreteIrrigationSectorBuilder& ConcreteIrrigationSectorBuilder::producePlantWithDMAMoistureSensor(const std::string& _p_name, const float& _ref_voltage, const uint32_t& _quantization_levels){
	uint8_t idx = static_cast<uint8_t>(sector->vPlants.size());
	if (idx <= sector->getPlantsCountLimit()) {
		sector->vPlants.emplace_back(std::unique_ptr<PlantInterface>(new PlantWithDMAMoistureSensor(new Plant(_p_name, idx), _ref_voltage, _quantization_levels)));
		sector->vPlants.shrink_to_fit();
		sector->addPlantNameToLIst(sector->vPlants.back()->getName());
		sector->parts_.push_back(sector->vPlants.at(idx)->getName()); //TODO: delete this on STM32
	}
	return *this;
}

ConcreteIrrigationSectorBuilder& ConcreteIrrigationSectorBuilder::producePumpWithController(const pumpcontrollermode_t& _controller_mode, const uint32_t& _idletime_required_seconds, const uint32_t& _runtime_limit_seconds, \
	const std::array<struct gpio_s, 2>& _pinout, const struct gpio_s& _led_pinout,
	const struct gpio_s& _fault_pinout, const struct gpio_s& _mode_pinout){

	sector->pump_controller.setMode(_controller_mode);
	if (sector->pump_controller.createPump(pumptype_t::drv8833_dc) == true) {
		sector->pump_controller.p8833Pump->init(0, _idletime_required_seconds,
			_runtime_limit_seconds, _pinout, _led_pinout, _fault_pinout, _mode_pinout);
		sector->parts_.push_back("DRV8833Pump");  //TODO: delete this on STM32
	}
	return *this;
}

ConcreteIrrigationSectorBuilder& ConcreteIrrigationSectorBuilder::producePumpWithController(const pumpcontrollermode_t& _controller_mode, const uint32_t& _idletime_required_seconds, const uint32_t& _runtime_limit_seconds, \
	const struct gpio_s& _pinout, const struct gpio_s& _led){

	sector->pump_controller.setMode(_controller_mode);
	if (this->sector->pump_controller.createPump(pumptype_t::binary) == true) {
		this->sector->pump_controller.pBinPump->init(0, _idletime_required_seconds,
			_runtime_limit_seconds, _pinout, _led);
		this->sector->parts_.push_back("BinaryPump");  //TODO: delete this on STM32
	}
	return *this;
}

std::unique_ptr<IrrigationSector> ConcreteIrrigationSectorBuilder::GetProduct() {
	std::unique_ptr<IrrigationSector>(result);
	result = std::move(sector);
	Reset();
	return result;
}
