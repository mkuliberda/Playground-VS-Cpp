#include "watertank.h"
#include <bitset>


/*********************************** /
/*! Watertank class implementation */
/***********************************/

float& Watertank::getTemperatureCelsius(void) {
	return this->mean_water_temperature_celsius;
}

void Watertank::setWaterLevel(const contentlevel_t & _water_level) {
	this->water_level = _water_level;
}

Watertank::contentlevel_t& Watertank::getWaterLevel(void) {
	return this->water_level;
}

void Watertank::setState(const contentstate_t & _state) {
	this->water_state = _state;
}

Watertank::contentstate_t& Watertank::getState(void) {
	return this->water_state;
}

bool Watertank::checkStateOK(const double& _dt, uint32_t& errcodeBitmask) {

	/******************************errcodeBitmask****************************************
	 * *Upper 16 bits										Lower 16 bits
	 * 00000000 00000000 									00000000 00000000
	 * |||||||| ||||||||->water temperature too low	 (16)	|||||||| ||||||||->(0)
	 * |||||||| |||||||-->water temperature too high (17)	|||||||| |||||||-->(1)
	 * |||||||| ||||||--->water level too low		 (18)	|||||||| ||||||--->(2)
	 * |||||||| |||||---->temperature sensor1 invalid(19)	|||||||| |||||---->(3)
	 * |||||||| ||||----->temperature sensor2 invalid(20)	|||||||| ||||----->(4)
	 * |||||||| |||------>temperature sensor3 invalid(21)	|||||||| |||------>(5)
	 * |||||||| ||------->wl sensor1 invalid         (22)	|||||||| ||------->(6)
	 * |||||||| |-------->wl sensor2 invalid         (23)	|||||||| |-------->(7)
	 * ||||||||---------->wl sensor3 invalid         (24)	||||||||---------->(8)
	 * |||||||----------->wl sensor4 invalid         (25)	|||||||----------->(9)
	 * ||||||------------>wl sensor5 invalid         (26)	||||||------------>(10)
	 * |||||------------->wl sensor6 invalid         (27)	|||||------------->(11)
	 * ||||-------------->wl sensor7 invalid         (28)	||||-------------->(12)
	 * |||--------------->wl sensor8 invalid         (29)	|||--------------->(13)
	 * ||---------------->wl sensor9 invalid         (30)	||---------------->(14)
	 * |----------------->wl sensor10 invalid        (31)	|----------------->(15)
	 */

	uint8_t temp_waterlevelPercent = 0;
	uint8_t waterlevelPercent = 0;
	bool isOK = true;
	bool tempReadingValid = false;
	std::bitset<32> errcode; errcode.set();  //initialize bitset and set all bits to 1


	/*if (this->temperatureSensorsCount > 0) {

		std::vector <float> vTemperature;

		for (uint8_t i = 0; i < this->temperatureSensorsCount; i++) {
			if (this->vTemperatureSensors[i].isValid() == true) {
				float temp = this->vTemperatureSensors[i].temperatureCelsiusRead(_dt);
				vTemperature.push_back(temp);
				errcode.reset(19 + i);
				tempReadingValid = true;
			}
		}

		if (tempReadingValid == true) {
			this->mean_watertemperatureCelsius = (accumulate(vTemperature.begin(), vTemperature.end(), 0)) / this->temperatureSensorsCount;

			if (this->mean_watertemperatureCelsius < 0.0) {
				this->stateSet(contentstate_t::frozen);
				errcode.reset(17);
				isOK = false;
			}
			else if (this->mean_watertemperatureCelsius > 100.0)
			{
				this->stateSet(contentstate_t::boiling);
				errcode.reset(16);
				isOK = false;
			}
			else {
				this->stateSet(contentstate_t::liquid);
				errcode.reset(16);
				errcode.reset(17);
			}
		}
		else {
			//if temperature reading isn't valid then don't update state
		}
	}
	else {
		//isOK = false;//let's let it work without temperature sensor
	}*/

	//TODO: sensors implement to this class
	/*if (this->waterlevelSensorsCount > 0) {
		for (uint8_t i = 0; i < this->waterlevelSensorsCount; i++) {
			if (this->vOpticalWLSensors[i].isValid() == true) {
				errcode.reset(22 + i);
				if (this->vOpticalWLSensors[i].isSubmersed()) {
					temp_waterlevelPercent = this->waterlevelConvertToPercent(this->vOpticalWLSensors[i].mountpositionGet());
					if (temp_waterlevelPercent > waterlevelPercent) waterlevelPercent = temp_waterlevelPercent;
				}
			}
		}
	}*/

	if (waterlevelPercent >= 98) { this->setWaterLevel(contentlevel_t::full); errcode.reset(18); }
	else if (waterlevelPercent > 90) { this->setWaterLevel(contentlevel_t::above90); errcode.reset(18); }
	else if (waterlevelPercent > 80) { this->setWaterLevel(contentlevel_t::above80); errcode.reset(18); }
	else if (waterlevelPercent > 70) { this->setWaterLevel(contentlevel_t::above70); errcode.reset(18); }
	else if (waterlevelPercent > 60) { this->setWaterLevel(contentlevel_t::above60); errcode.reset(18); }
	else if (waterlevelPercent > 50) { this->setWaterLevel(contentlevel_t::above50); errcode.reset(18); }
	else if (waterlevelPercent > 40) { this->setWaterLevel(contentlevel_t::above40); errcode.reset(18); }
	else if (waterlevelPercent > 30) { this->setWaterLevel(contentlevel_t::above30); errcode.reset(18); }
	else if (waterlevelPercent > 20) { this->setWaterLevel(contentlevel_t::above20); errcode.reset(18); }
	else if (waterlevelPercent > 10) { this->setWaterLevel(contentlevel_t::above10); errcode.reset(18); }
	else if (waterlevelPercent >= 0) { this->setWaterLevel(contentlevel_t::empty); isOK = false; }

	errcodeBitmask = errcode.to_ulong();

	return isOK;
}

uint8_t Watertank::waterlevelConvertToPercent(const float& _val_meters) {
	return static_cast<uint8_t>(_val_meters / this->tank_height_meters * 100);
}

uint8_t Watertank::getWaterLevelPercent(void) {
	return static_cast<uint8_t>(this->water_level);
}

uint8_t& Watertank::getId(void) {
	return this->watertank_info.id;
}

void Watertank::setVolume(const double& _volume) {
	this->tank_volume_liters = _volume;
}

void Watertank::setHeight(const double& _height) {
	this->tank_height_meters = _height;
}

double& Watertank::getHeight(void) {
	return this->tank_height_meters;
}



void ConcreteWatertankBuilder::Reset() {
	watertank.reset(new Watertank(static_cast<uint8_t>(++watertank_count)));
}
/**
 * All production steps work with the same watertank instance.
 */

ConcreteWatertankBuilder& ConcreteWatertankBuilder::ProducePartA() {  //TODO: delete this on STM32
	watertank->parts_.push_back("PartA1");
	return *this;
}

ConcreteWatertankBuilder& ConcreteWatertankBuilder::ProducePartB() {  //TODO: delete this on STM32
	watertank->parts_.push_back("PartB1");
	return *this;
}

ConcreteWatertankBuilder& ConcreteWatertankBuilder::ProducePartC() {  //TODO: delete this on STM32
	watertank->parts_.push_back("PartC1");
	return *this;
}

std::unique_ptr<Watertank> ConcreteWatertankBuilder::GetProduct() {
	std::unique_ptr<Watertank>(result);
	result = std::move(watertank);
	Reset();
	return result;
}
