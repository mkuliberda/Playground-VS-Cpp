#pragma once
#include "main.h"
#include <iostream>
#include "ds18b20.h"


enum class fixedwaterlevelsensorstate_t {
	undetermined,
	wet,
	dry
};

enum class sensor_type_t {
	generic_sensor,
	waterlevel_sensor,
	temperature_sensor,
	waterflow_sensor,
	precipitation_sensor
};

class SensorImp {
public:
	SensorImp(const sensor_type_t& _type) {
		sensor_type = _type;
		std::cout << "SensorImp standard constructor " << std::endl;//TODO: delete on STM32
	}
	~SensorImp() {
		std::cout << "SensorImp dtor " << std::endl;//TODO: delete on STM32
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	SensorImp(SensorImp const &) = delete;
	SensorImp& operator=(SensorImp const&) = delete;

	virtual float						read(const double& _dt=0);
	virtual sensor_type_t				getType();
protected:
	sensor_type_t						sensor_type;
};

class OpticalWaterLevelSensorImp : public SensorImp {
public:
	OpticalWaterLevelSensorImp(const float& _mount_height_meters, const struct gpio_s& _pinout, const sensor_type_t& _type = sensor_type_t::waterlevel_sensor) : SensorImp(_type) {
		mount_height_meters = _mount_height_meters;
		sensor_type = _type;
		pinout = _pinout;
		std::cout << "OpticalWaterLevelSensorImp standard constructor " << std::endl;//TODO: delete on STM32
	}

	OpticalWaterLevelSensorImp(const float&& _mount_height_meters, const struct gpio_s& _pinout, const sensor_type_t&& _type = sensor_type_t::waterlevel_sensor) : SensorImp(_type) {
		mount_height_meters = std::move(_mount_height_meters);
		sensor_type = std::move(_type);
		pinout = std::move(_pinout);
		std::cout << "OpticalWaterLevelSensorImp move constructor " << std::endl;//TODO: delete on STM32
	}
	~OpticalWaterLevelSensorImp() {
		std::cout << "OpticalWaterLevelSensorImp dtor " << std::endl;//TODO: delete on STM32
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	OpticalWaterLevelSensorImp(OpticalWaterLevelSensorImp const &) = delete;
	OpticalWaterLevelSensorImp& operator=(OpticalWaterLevelSensorImp const&) = delete;

	/* virtual */
	float									read(const double& _dt = 0);
	void									init(const float& _mount_height_meters, const struct gpio_s& _pinout);
	const float&							getMountHeightMeters(void);
	const bool&								isValid(void) const;
	bool									isSubmersed(void);

protected:
	float 									mount_height_meters = 0.0;
	fixedwaterlevelsensorstate_t 			state = fixedwaterlevelsensorstate_t::undetermined;
	struct gpio_s 							pinout = {0, 0};
	bool									initialized = false;

};

class WaterFlowSensorImp : public SensorImp {
public:
	WaterFlowSensorImp(const struct gpio_s& _pinout, const sensor_type_t& _type = sensor_type_t::waterflow_sensor) : SensorImp(_type) {
		sensor_type = _type;
		pinout = _pinout;
		std::cout << "WaterFlowSensorImp standard constructor " << std::endl;//TODO: delete on STM32
	}
	~WaterFlowSensorImp() {
		std::cout << "WaterFlowSensorImp dtor " << std::endl;//TODO: delete on STM32
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	WaterFlowSensorImp(WaterFlowSensorImp const &) = delete;
	WaterFlowSensorImp& operator=(WaterFlowSensorImp const&) = delete;

	/* virtual */
	float									read(const double& _dt = 0);

protected:
	char zone_[3] = "+1";//TODO: delete on STM32
	struct gpio_s 							pinout;
};

class DS18B20TemperatureSensorImp : public SensorImp, public DS18B20{
public:
	DS18B20TemperatureSensorImp(const struct gpio_s& _pinout, const sensor_type_t& _type = sensor_type_t::temperature_sensor) : SensorImp(_type) {
		sensor_type = _type;
		this->init(_pinout, 0);
		std::cout << "DS18B20TemperatureSensorImp standard constructor " << std::endl;//TODO: delete on STM32
	}
	~DS18B20TemperatureSensorImp() {
		std::cout << "DS18B20TemperatureSensorImp dtor " << std::endl;//TODO: delete on STM32
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	DS18B20TemperatureSensorImp(DS18B20TemperatureSensorImp const &) = delete;
	DS18B20TemperatureSensorImp& operator=(DS18B20TemperatureSensorImp const&) = delete;

	/* virtual */
	float									read(const double& _dt = 0);

protected:
	char zone_[3] = "-1";//TODO: delete on STM32

};

class Sensor {
public:
	Sensor() {}
	Sensor(const sensor_type_t& _type) {
		imp_ = new SensorImp(_type);
	}
	Sensor(const sensor_type_t&& _type) {
		imp_ = new SensorImp(std::move(_type));
	}
	~Sensor() {
		std::cout << "Sensor dtor " << std::endl;//TODO: delete on STM32
		delete imp_;
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	Sensor(Sensor const &) = delete;
	Sensor& operator=(Sensor const&) = delete;

	virtual float							read(const double& _dt = 0);
	virtual sensor_type_t					getType();
protected:
	SensorImp								*imp_;
};

class OpticalWaterLevelSensor : public Sensor {
public:
	OpticalWaterLevelSensor(const float& _mount_height_meters, const struct gpio_s& _pinout) {
		imp_ = new OpticalWaterLevelSensorImp(_mount_height_meters, _pinout);
		std::cout << "OpticalWaterLevelSensor standard constructor " << std::endl;//TODO: delete on STM32
	}
	OpticalWaterLevelSensor(const float&& _mount_height_meters, const struct gpio_s&& _pinout) {
		std::cout << "OpticalWaterLevelSensor move constructor " << std::endl;//TODO: delete on STM32
		imp_ = new OpticalWaterLevelSensorImp(std::move(_mount_height_meters), std::move(_pinout));
	}
	~OpticalWaterLevelSensor() {
		std::cout << "OpticalWaterLevelSensor dtor " << std::endl;//TODO: delete on STM32
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	OpticalWaterLevelSensor(OpticalWaterLevelSensor const &) = delete;
	OpticalWaterLevelSensor& operator=(OpticalWaterLevelSensor const&) = delete;
};

class WaterFlowSensor : public Sensor {
public:
	WaterFlowSensor(const struct gpio_s& _pinout) {
		imp_ = new WaterFlowSensorImp(_pinout);
	}
	~WaterFlowSensor() {
		std::cout << "WaterFlowSensor dtor " << std::endl;//TODO: delete on STM32
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	WaterFlowSensor(WaterFlowSensor const &) = delete;
	WaterFlowSensor& operator=(WaterFlowSensor const&) = delete;
};

class DS18B20TemperatureSensor : public Sensor {
public:
	DS18B20TemperatureSensor(const struct gpio_s& _pinout) {
		imp_ = new DS18B20TemperatureSensorImp(_pinout);
	}
	~DS18B20TemperatureSensor() {
		std::cout << "DS18B20TemperatureSensor dtor " << std::endl;//TODO: delete on STM32
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	DS18B20TemperatureSensor(DS18B20TemperatureSensor const &) = delete;
	DS18B20TemperatureSensor& operator=(DS18B20TemperatureSensor const&) = delete;
};