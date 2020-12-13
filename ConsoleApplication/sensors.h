#pragma once
#include "main.h"
#include <iostream>


enum class fixedwaterlevelsensorstate_t {
	undetermined,
	wet,
	dry
};

enum class waterlevelsensortype_t {
	unknown,
	optical,
	capacitive,
	resistive
};

enum class waterlevelsensorsubtype_t {
	unknown,
	fixed,
	floating
};

enum class sensor_type_t {
	generic_sensor,
	waterlevel_sensor,
	temperature_sensor,
	waterflow_sensor
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

class ZuluSensorImp : public SensorImp {
public:
	ZuluSensorImp(const int& _zone, const sensor_type_t& _type = sensor_type_t::temperature_sensor) : SensorImp(_type) {
		if (_zone == 5)
			strcpy_s(zone_, " Eastern Standard Sensor");
		else if (_zone == 6)
			strcpy_s(zone_, " Central Standard Sensor");
		sensor_type = _type;
		std::cout << "ZuluSensorImp standard constructor " << std::endl;
	}
	~ZuluSensorImp() {
		std::cout << "ZuluSensorImp dtor " << std::endl;
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	ZuluSensorImp(ZuluSensorImp const &) = delete;
	ZuluSensorImp& operator=(ZuluSensorImp const&) = delete;

	/* virtual */
	void									read();
	void									getResult(float& val);

protected:
	char zone_[30];
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
	SensorImp *imp_;
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

class ZuluSensor : public Sensor {
public:
	ZuluSensor(int zone) {
		imp_ = new ZuluSensorImp(zone);
	}
	~ZuluSensor() {
		std::cout << "ZuluSensor dtor " << std::endl;
	}
	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	ZuluSensor(ZuluSensor const &) = delete;
	ZuluSensor& operator=(ZuluSensor const&) = delete;
};