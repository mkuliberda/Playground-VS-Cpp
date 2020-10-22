#pragma once
#include <string>
#include <array>
#include <vector>

using namespace std;

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

struct gpio_s {
	uint32_t port;
	uint16_t pin;
};


//Decorator design pattern used for plants
class PlantInterface {
public:
	virtual ~PlantInterface() {}
	virtual float getMoisturePercent() = 0;
	virtual bool setMoisturePercent(const float &_moisture) = 0;
	virtual std::string getName() = 0;
	virtual uint8_t getId() = 0;
	virtual void updateRaw(const uint32_t &_raw_measurement) = 0;
};

class Plant : public PlantInterface {
private:

	std::string name;
	float soil_moisture_percent = -1000;
	uint8_t id;

public:

	Plant(const std::string& _name, const uint8_t& _id) :
		name(_name),
		id(_id)
	{
		cout << "Plant standard constructor" << endl;
		name.shrink_to_fit();
	}

	Plant(const std::string&& _name, const uint8_t&& _id) :
		name(std::move(_name)),
		id(std::move(_id))
	{
		cout << "Plant move constructor" << endl;
		name.shrink_to_fit();
	}


	~Plant()
	{
		cout << "Plant dtor		";
	}
	/*virtual*/
	float getMoisturePercent(void) {
		return soil_moisture_percent;
	}

	std::string getName(void) {
		return name;
	}

	uint8_t getId(void) {
		return id;
	}

	bool setMoisturePercent(const float& _moisture) {
		if (_moisture <= 100 && _moisture >= 0) {
			soil_moisture_percent = _moisture;
			return true;
		}
		else {
			return false;
		}
	}

	void updateRaw(const uint32_t& _raw_measurement) {
	}

};

class PlantWithSensor : public PlantInterface {
public:
	PlantWithSensor(PlantInterface *inner) {
		m_wrappee = inner;
	}
	~PlantWithSensor() {
		cout << "PlantWithSensor  dtor		";
		delete m_wrappee;
	}

	float getMoisturePercent() {
		return m_wrappee->getMoisturePercent();
	}

	bool setMoisturePercent(const float& _moisture) {
		return m_wrappee->setMoisturePercent(_moisture);
	}

	std::string getName() {
		return m_wrappee->getName();
	}

	uint8_t getId() {
		return m_wrappee->getId();
	}

	void updateRaw(const uint32_t &_raw_measurement) {
	}

private:
	PlantInterface *m_wrappee;
};

class PlantWithDMAMoistureSensor : public PlantWithSensor {
private:
	uint32_t quantization_levels;
	float ref_voltage;
	float soil_moisture_volts;
	float soil_moisture_raw;

public:
	PlantWithDMAMoistureSensor(PlantInterface *core, const float& _ref_voltage = 3.3, const uint32_t& _quantization_levels = 4095) :
		PlantWithSensor(core),
		ref_voltage(_ref_voltage),
		quantization_levels(_quantization_levels) {
		cout << "PlantWithDMAMoistureSensor standard constructor" << endl;
	}

	PlantWithDMAMoistureSensor(PlantInterface *core, const float&& _ref_voltage = 3.3, const uint32_t&& _quantization_levels = 4095) :
		PlantWithSensor(core),
		ref_voltage(std::move(_ref_voltage)),
		quantization_levels(std::move(_quantization_levels)) {
		cout << "PlantWithDMAMoistureSensor move constructor" << endl;
	}

	~PlantWithDMAMoistureSensor() {
		cout << "PlantWithDMAMoistureSensor  dtor	";
	}
	/*virtual*/
	float getMoisturePercent() {
		return PlantWithSensor::getMoisturePercent();
	}

	float getMoistureVolts() {
		return soil_moisture_volts;
	}

	float getMoistureRaw() {
		return soil_moisture_raw;
	}

	std::string getName() {
		return PlantWithSensor::getName();
	}

	uint8_t getId() {
		return PlantWithSensor::getId();
	}

	void updateRaw(const uint32_t& _raw_measurement) {
		soil_moisture_raw = _raw_measurement;
		soil_moisture_volts = _raw_measurement * ref_voltage / quantization_levels;
		float moisture_percent = soil_moisture_raw * 100 / quantization_levels;
		PlantWithSensor::setMoisturePercent(moisture_percent);
	}

};

