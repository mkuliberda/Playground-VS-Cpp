#pragma once
#include "main.h"
#include <iostream>


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
		std::cout << "SensorImp standard constructor " << std::endl;
	}
	~SensorImp() {
		std::cout << "SensorImp dtor " << std::endl;
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	SensorImp(SensorImp const &) = delete;
	SensorImp& operator=(SensorImp const&) = delete;

	virtual void						read();
	virtual sensor_type_t				getType();
	virtual void						getResult(bool& val);
	virtual void						getResult(float& val);
protected:
	sensor_type_t						sensor_type;
};

class OpticalWaterLevelSensorImp : public SensorImp {
public:
	OpticalWaterLevelSensorImp(const float& _mount_pos_meters, const struct gpio_s& _pinout, const sensor_type_t& _type = sensor_type_t::waterlevel_sensor) : SensorImp(_type) {
		mount_pos_meters = _mount_pos_meters;
		sensor_type = _type;
		pinout = _pinout;
		std::cout << "OpticalWaterLevelSensorImp standard constructor " << std::endl;
	}

	OpticalWaterLevelSensorImp(const float&& _mount_pos_meters, const struct gpio_s& _pinout, const sensor_type_t&& _type = sensor_type_t::waterlevel_sensor) : SensorImp(_type) {
		mount_pos_meters = std::move(_mount_pos_meters);
		sensor_type = std::move(_type);
		pinout = std::move(_pinout);
		std::cout << "OpticalWaterLevelSensorImp move constructor " << std::endl;
	}
	~OpticalWaterLevelSensorImp() {
		std::cout << "OpticalWaterLevelSensorImp dtor " << std::endl;
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	OpticalWaterLevelSensorImp(OpticalWaterLevelSensorImp const &) = delete;
	OpticalWaterLevelSensorImp& operator=(OpticalWaterLevelSensorImp const&) = delete;

	/* virtual */
	void									read();
	void									getResult(bool& val);

protected:
	float 									mount_pos_meters;
	fixedwaterlevelsensorstate_t 			state;
	struct gpio_s 							pinout;
};

class WaterFlowSensorImp : public SensorImp {
public:
	WaterFlowSensorImp(const sensor_type_t& _type = sensor_type_t::waterflow_sensor) : SensorImp(_type) {
		sensor_type = _type;
		std::cout << "WaterFlowSensorImp standard constructor " << std::endl;
	}
	~WaterFlowSensorImp() {
		std::cout << "WaterFlowSensorImp dtor " << std::endl;
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	WaterFlowSensorImp(WaterFlowSensorImp const &) = delete;
	WaterFlowSensorImp& operator=(WaterFlowSensorImp const&) = delete;

	/* virtual */
	void									read();
	void									getResult(float& val);

protected:
	char zone_[3] = "+1";
};

class TemperatureSensorImp : public SensorImp {
public:
	TemperatureSensorImp(const sensor_type_t& _type = sensor_type_t::temperature_sensor) : SensorImp(_type) {
		sensor_type = _type;
		std::cout << "TemperatureSensorImp standard constructor " << std::endl;
	}
	~TemperatureSensorImp() {
		std::cout << "TemperatureSensorImp dtor " << std::endl;
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	TemperatureSensorImp(TemperatureSensorImp const &) = delete;
	TemperatureSensorImp& operator=(TemperatureSensorImp const&) = delete;

	/* virtual */
	void									read();
	void									getResult(float& val);

protected:
	char zone_[3] = "-1";
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
		std::cout << "Sensor dtor " << std::endl;
		delete imp_;
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	Sensor(Sensor const &) = delete;
	Sensor& operator=(Sensor const&) = delete;

	virtual void							read();
	virtual sensor_type_t					getType();
	virtual void							getResult(bool& val);
	virtual void							getResult(float& val);
protected:
	SensorImp								*imp_;
};

class OpticalWaterLevelSensor : public Sensor {
public:
	OpticalWaterLevelSensor(const float& _mount_pos_meters, const struct gpio_s& _pinout) {
		imp_ = new OpticalWaterLevelSensorImp(_mount_pos_meters, _pinout);
		std::cout << "OpticalWaterLevelSensor standard constructor " << std::endl;
	}
	OpticalWaterLevelSensor(const float&& _mount_pos_meters, const struct gpio_s&& _pinout) {
		std::cout << "OpticalWaterLevelSensor move constructor " << std::endl;
		imp_ = new OpticalWaterLevelSensorImp(std::move(_mount_pos_meters), std::move(_pinout));
	}
	~OpticalWaterLevelSensor() {
		std::cout << "OpticalWaterLevelSensor dtor " << std::endl;
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	OpticalWaterLevelSensor(OpticalWaterLevelSensor const &) = delete;
	OpticalWaterLevelSensor& operator=(OpticalWaterLevelSensor const&) = delete;
};

class WaterFlowSensor : public Sensor {
public:
	WaterFlowSensor() {
		imp_ = new WaterFlowSensorImp();
	}
	~WaterFlowSensor() {
		std::cout << "WaterFlowSensor dtor " << std::endl;
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	WaterFlowSensor(WaterFlowSensor const &) = delete;
	WaterFlowSensor& operator=(WaterFlowSensor const&) = delete;
};

class TemperatureSensor : public Sensor {
public:
	TemperatureSensor() {
		imp_ = new TemperatureSensorImp();
	}
	~TemperatureSensor() {
		std::cout << "TemperatureSensor dtor " << std::endl;
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	TemperatureSensor(TemperatureSensor const &) = delete;
	TemperatureSensor& operator=(TemperatureSensor const&) = delete;
};