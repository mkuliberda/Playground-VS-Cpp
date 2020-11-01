#pragma once

#include <string>
#include <iostream>
#include "main.h"

enum plant_type: uint8_t {
	plant_only,
	plant_with_dma_moisture_sensor
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
	virtual void setPlantType(const plant_type& _p_type=plant_only) = 0;
	virtual plant_type getPlantType() = 0;
	virtual void setRainExposition(const bool& _rain_exposed) = 0;
	virtual bool isRainExposed() = 0;
};

class Plant : public PlantInterface {
private:

	plant_type											p_type;
	std::string											name;
	float												soil_moisture_percent = -1000;
	uint8_t												id;
	bool												rain_exposed = true;

public:
	Plant(const std::string& _name, const uint8_t& _id) :
		name(_name),
		id(_id)
	{
		std::cout << "Plant standard constructor" << std::endl;
		name.shrink_to_fit();
		plant_type p_type = plant_only;
	}

	Plant(const std::string&& _name, const uint8_t&& _id) :
		name(std::move(_name)),
		id(std::move(_id))
	{
		std::cout << "Plant move constructor" << std::endl;
		name.shrink_to_fit();
		plant_type p_type = plant_only;
	}

	~Plant()
	{
		std::cout << "Plant dtor		";
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

	void setPlantType(const plant_type& _p_type = plant_only) {
		this->p_type = _p_type;
	}

	plant_type getPlantType(void) {
		return this->p_type;
	}

	void setRainExposition(const bool& _rain_exposed) {
		rain_exposed = _rain_exposed;
	}

	bool isRainExposed() {
		return rain_exposed;
	}

};

class PlantWithSensor : public PlantInterface {
public:
	PlantWithSensor(PlantInterface *inner) {
		m_wrappee = inner;
	}
	~PlantWithSensor() {
		std::cout << "PlantWithSensor  dtor		";
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

	void setPlantType(const plant_type& _p_type = plant_only) {
		m_wrappee->setPlantType(_p_type);
	}

	plant_type getPlantType(void) {
		return m_wrappee->getPlantType();
	}

	void setRainExposition(const bool& _rain_exposed) {
		m_wrappee->setRainExposition(_rain_exposed);
	}

	bool isRainExposed() {
		return m_wrappee->isRainExposed();
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
		setPlantType();
		std::cout << "PlantWithDMAMoistureSensor standard constructor" << std::endl;
	}

	PlantWithDMAMoistureSensor(PlantInterface *core, const float&& _ref_voltage = 3.3, const uint32_t&& _quantization_levels = 4095) :
		PlantWithSensor(core),
		ref_voltage(std::move(_ref_voltage)),
		quantization_levels(std::move(_quantization_levels)) {
		setPlantType();
		std::cout << "PlantWithDMAMoistureSensor move constructor" << std::endl;
	}

	~PlantWithDMAMoistureSensor() {
		std::cout << "PlantWithDMAMoistureSensor  dtor	";
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

	void setPlantType(const plant_type& _p_type = plant_only) {
		PlantWithSensor::setPlantType(plant_type::plant_with_dma_moisture_sensor);
	}

	plant_type getPlantType() {
		return PlantWithSensor::getPlantType();
	}

	void setRainExposition(const bool& _rain_exposed) {
		PlantWithSensor::setRainExposition(_rain_exposed);
	}

	bool isRainExposed() {
		return PlantWithSensor::isRainExposed();
	}
};
