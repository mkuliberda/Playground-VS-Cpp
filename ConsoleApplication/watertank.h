#pragma once
#include "main.h"
#include "sensors.h"
#include <iostream> //TODO: delete on STM32
#include <vector>
#include <string>

#define W_SUCCESS		(uint8_t)1
#define W_FAIL			(uint8_t)0
#define W_LIMIT_REACHED (uint8_t)2

#define FIXED_WATER_LEVEL_SENSORS_LIMIT 10
#define TEMPERATURE_SENSORS_LIMIT 3

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

inline double operator"" _C(long double c) {
	return c;
}

inline double operator"" _F(long double f) {
	return (f - 32) * 5 / 9;
}

inline double operator"" _K(long double k) {
	return k - 273.15;
}

struct WatertankInfo_s {
	uint32_t	errors;
	uint8_t		water_level;
	uint8_t		id;
};

/**
 * It makes sense to use the Builder pattern only when your objects are quite
 * complex and require extensive configuration.
 *
 * Unlike in other creational patterns, different concrete builders can produce
 * unrelated objects. In other words, results of various builders may not
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

	float								mean_water_temperature_celsius;
	contentlevel_t						water_level;
	contentstate_t 						water_state;
	double	 							tank_height_meters;
	double		 						tank_volume_liters;
	uint8_t								fixed_water_level_sensors_count;
	uint8_t								temperature_sensors_count;

	void 								setWaterLevel(const contentlevel_t& _water_level);
	contentlevel_t&						getWaterLevel(void);
	void 								setWaterState(const contentstate_t& _water_state);
	contentstate_t&						getWaterState(void);
	uint8_t 							waterlevelConvertToPercent(const float& _val_meters); //TODO:literals?

public:
	std::vector<std::unique_ptr<Sensor>>		vSensors;
	const uint8_t 						fixed_water_level_sensors_limit;
	const uint8_t 						temperature_sensors_limit;

	bool 								update(const double& _dt, uint32_t&  errcodeBitmask);
	float& 								getWaterTemperatureCelsius(void);
	float 								getWaterTemperatureKelvin(void);
	float 								getWaterTemperatureFahrenheit(void);
	uint8_t		 						getWaterLevelPercent(void);
	uint8_t&							getId(void);
	void								setVolume(const double& _volume);
	void								setHeight(const double& _height); 
	double&								getHeightMeters(void);
	uint8_t								incrementTemperatureSensorsCount(void);
	uint8_t								incrementFixedWaterLevelSensorsCount(void);


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
		mean_water_temperature_celsius(0.0_C),
		water_level(contentlevel_t::unknown),
		water_state(contentstate_t::unknown),
		tank_height_meters(0.0_m),
		tank_volume_liters(0.0_l),
		fixed_water_level_sensors_limit(10),
		fixed_water_level_sensors_count(0),
		temperature_sensors_limit(3),
		temperature_sensors_count(0)
	{
		watertank_info.id = _id;
		std::cout << "Standard Constructor Watertank" << std::endl;//TODO: delete on STM32
	};

	Watertank(const uint8_t&& _id):
		mean_water_temperature_celsius(0.0_C),
		water_level(contentlevel_t::unknown),
		water_state(contentstate_t::unknown),
		tank_height_meters(0.0_m),
		tank_volume_liters(0.0_l),
		fixed_water_level_sensors_limit(10),
		fixed_water_level_sensors_count(0),
		temperature_sensors_limit(3),
		temperature_sensors_count(0) 
	{
		watertank_info.id = std::move(_id);
		std::cout << "Move Constructor Watertank" << std::endl;//TODO: delete on STM32
	}

	~Watertank() {
		std::cout << "Watertank dtor" << std::endl;//TODO: delete on STM32
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
	virtual void ProducePartA() = 0;
	virtual void ProducePartB() = 0;
	virtual void ProducePartC() = 0;
	virtual void produceOpticalWaterLevelSensor(const float& _mount_pos_meters, const struct gpio_s& _pinout) = 0;
	virtual void produceDS18B20TemperatureSensor(const struct gpio_s& _pinout, TIM_HandleTypeDef* _tim_baseHandle) = 0;
	virtual void setWatertankVolume(const double& _volume) = 0;
	virtual void setWatertankHeight(const double& _height) = 0;

	//virtual WatertankBuilder& ProduceSensor(const sensor_type_t& _type, ) = 0;

};

/**
 * The Concrete Builder classes follow the Builder interface and provide
 * specific implementations of the building steps. Your program may have several
 * variations of Builders, implemented differently.
 */
class ConcreteWatertankBuilder : public WatertankBuilder {
private:
	int8_t watertank_count = -1;
	std::unique_ptr<Watertank>		watertank;

	/**
	 * A fresh builder instance should contain a blank sector object, which is
	 * used in further assembly.
	 */
public:

	ConcreteWatertankBuilder() {
		std::cout << "ConcreteWatertankBuilder" << std::endl;//TODO: delete on STM32
		this->Reset();
	}

	~ConcreteWatertankBuilder() {
		std::cout << "ConcreteWatertankBuilder dtor" << std::endl;//TODO: delete on STM32
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	ConcreteWatertankBuilder(ConcreteWatertankBuilder const &) = delete;
	ConcreteWatertankBuilder& operator=(ConcreteWatertankBuilder const&) = delete;

	void							Reset();
	void							ProducePartA() override;
	void							ProducePartB() override;
	void							ProducePartC() override;
	void							produceOpticalWaterLevelSensor(const float& _mount_pos_meters, const struct gpio_s& _pinout) override;
	void							produceDS18B20TemperatureSensor(const struct gpio_s& _pinout, TIM_HandleTypeDef* _tim_baseHandle);
	void							setWatertankVolume(const double& _volume) override;
	void							setWatertankHeight(const double& _height) override;
	std::unique_ptr<Watertank>		GetProduct();

};
