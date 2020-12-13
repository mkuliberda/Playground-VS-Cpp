#pragma once
#include "main.h"
#include "sensors.h"
#include <iostream> //TODO: delete on STM32
#include <vector>
#include <string>

inline double operator"" _l(long double l) {
	return l;
}
inline double operator"" _ml(long double ml) {
	return ml / 1000;
}

inline double operator"" _m(long double m) {
	return m;
}
inline double operator"" _cm(long double cm) {
	return cm / 100;
}
inline double operator"" _mm(long double mm) {
	return mm / 1000;
}

struct WatertankInfo_s {
	uint32_t	errors;
	uint8_t		water_level;
	uint8_t		id;
};

/**
 * It makes sense to use the Builder pattern only when your sectors are quite
 * complex and require extensive configuration.
 *
 * Unlike in other creational patterns, different concrete builders can produce
 * unrelated sectors. In other words, results of various builders may not
 * always follow the same interface.
 */

class Watertank {
private:

	struct WatertankInfo_s								watertank_info;

	enum class contentstate_t : uint8_t {
		unknown = 255,
		liquid = 1,
		frozen = 2,
		boiling = 3
	};

	enum class contentlevel_t : uint8_t {
		unknown = 255,
		empty = 0,
		above10 = 10,
		above20 = 20,
		above30 = 30,
		above40 = 40,
		above50 = 50,
		above60 = 60,
		above70 = 70,
		above80 = 80,
		above90 = 90,
		full = 100
	};

	float								mean_water_temperature_celsius; //TODO literals?
	contentlevel_t						water_level;
	contentstate_t 						water_state;
	double	 							tank_height_meters; //TODO: literals?
	double		 						tank_volume_liters; //TODO: literals?
	const uint8_t 						water_level_sensors_limit;
	uint8_t								water_level_sensors_count;
	const uint8_t 						temperature_sensors_limit;
	uint8_t								temperature_sensors_count;

	void 								setWaterLevel(const contentlevel_t& _water_level);
	contentlevel_t&						getWaterLevel(void);
	void 								setState(const contentstate_t& _water_state);
	contentstate_t&						getState(void);
	uint8_t 							waterlevelConvertToPercent(const float& _val_meters); //TODO:literals?

public:
	std::vector<std::unique_ptr<Sensor>>		vSensors;

	bool 								checkStateOK(const double& _dt, uint32_t&  errcodeBitmask);
	float& 								getTemperatureCelsius(void);
	uint8_t		 						getWaterLevelPercent(void);
	//bool 								waterlevelSensorCreate(const waterlevelsensortype_t & _sensortype);
	//bool 								temperatureSensorCreate(const temperaturesensortype_t & _sensortype);
	uint8_t&							getId(void);
	void								setVolume(const double& _volume); //TODO: literals?
	void								setHeight(const double& _height); //TODO: 
	double&								getHeight(void);

	std::vector<std::string> parts_;	//TODO: delete on STM32
	void ListParts()const {				//TODO: delete on STM32
		std::cout << "Watertank parts: ";
		for (size_t i = 0; i < parts_.size(); i++) {
			if (parts_[i] == parts_.back()) {
				std::cout << parts_[i];
			}
			else {
				std::cout << parts_[i] << ", ";
			}
		}
		std::cout << "\n\n";
	}

	Watertank(const uint8_t& _id) :
		mean_water_temperature_celsius(0.0),
		water_level(contentlevel_t::unknown),
		water_state(contentstate_t::unknown),
		tank_height_meters(0.0_m),
		tank_volume_liters(0.0_l),
		water_level_sensors_limit(20),
		water_level_sensors_count(0),
		temperature_sensors_limit(3),
		temperature_sensors_count(0)
	{
		watertank_info.id = _id;
		std::cout << "Standard Constructor Watertank" << std::endl;
	};

	Watertank(const uint8_t&& _id):
		mean_water_temperature_celsius(0.0),
		water_level(contentlevel_t::unknown),
		water_state(contentstate_t::unknown),
		tank_height_meters(0),
		tank_volume_liters(0),
		water_level_sensors_limit(20),
		water_level_sensors_count(0),
		temperature_sensors_limit(3),
		temperature_sensors_count(0) 
	{
		watertank_info.id = std::move(_id);
		std::cout << "Move Constructor Watertank" << std::endl;
	}

	~Watertank() {
		std::cout << "Watertank dtor" << std::endl;
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	Watertank(Watertank const &) = delete;
	Watertank& operator=(Watertank const&) = delete;

};

/**
 * The Builder interface specifies methods for creating the different parts of
 * the Watertank objects.
 */
class WatertankBuilder {
public:
	virtual ~WatertankBuilder() {}
	virtual WatertankBuilder& ProducePartA() = 0;
	virtual WatertankBuilder& ProducePartB() = 0;
	virtual WatertankBuilder& ProducePartC() = 0;
	//virtual WatertankBuilder& ProduceSensor(const sensor_type_t& _type, ) = 0;

};

/**
 * The Concrete Builder classes follow the Builder interface and provide
 * specific implementations of the building steps. Your program may have several
 * variations of Builders, implemented differently.
 */
class ConcreteWatertankBuilder : public WatertankBuilder {
private:
	std::unique_ptr<Watertank> watertank;
	int8_t watertank_count = -1;

	/**
	 * A fresh builder instance should contain a blank sector object, which is
	 * used in further assembly.
	 */
public:

	ConcreteWatertankBuilder() {
		std::cout << "ConcreteWatertankBuilder" << std::endl;
		this->Reset();
	}

	~ConcreteWatertankBuilder() {
		std::cout << "ConcreteWatertankBuilder dtor" << std::endl;
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	ConcreteWatertankBuilder(ConcreteWatertankBuilder const &) = delete;
	ConcreteWatertankBuilder& operator=(ConcreteWatertankBuilder const&) = delete;

	void												Reset();
	ConcreteWatertankBuilder&							ProducePartA() override;
	ConcreteWatertankBuilder&							ProducePartB() override;
	ConcreteWatertankBuilder&							ProducePartC() override;
	std::unique_ptr<Watertank>							GetProduct();
};
