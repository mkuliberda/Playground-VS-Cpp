#pragma once

#include <string>
#include <iostream>
#include "main.h"

enum plant_type_t: uint8_t {
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
	virtual void setPlantType(const plant_type_t& _p_type=plant_only) = 0;
	virtual plant_type_t getPlantType() = 0;
	virtual void setRainExposition(const bool& _rain_exposed) = 0;
	virtual bool isRainExposed() = 0;
};

class Plant : public PlantInterface {
private:

	plant_type_t									p_type;
	std::string										name;
	float											soil_moisture_percent = -1000;
	uint8_t											id;
	bool											rain_exposed = true;

public:
	Plant(const std::string& _name, const uint8_t& _id) :
		name(_name),
		id(_id)
	{
		std::cout << "Plant standard constructor" << std::endl;
		name.shrink_to_fit();
		plant_type_t p_type = plant_only;
	}

	Plant(const std::string&& _name, const uint8_t&& _id) :
		name(std::move(_name)),
		id(std::move(_id))
	{
		std::cout << "Plant move constructor" << std::endl;
		name.shrink_to_fit();
		plant_type_t p_type = plant_only;
	}

	~Plant()
	{
		std::cout << "Plant dtor		";
	}
	/*virtual*/
	float											getMoisturePercent(void);
	std::string										getName(void);
	uint8_t											getId(void);
	bool											setMoisturePercent(const float& _moisture);
	void											updateRaw(const uint32_t& _raw_measurement);
	void											setPlantType(const plant_type_t& _p_type = plant_only);
	plant_type_t									getPlantType(void);
	void											setRainExposition(const bool& _rain_exposed);
	bool											isRainExposed();
};

class PlantWithSensor : public PlantInterface {
private:
	PlantInterface									*m_wrappee;

public:
	PlantWithSensor(PlantInterface *inner) {
		m_wrappee = inner;
	}
	~PlantWithSensor() {
		std::cout << "PlantWithSensor  dtor		";
		delete m_wrappee;
	}

	float											getMoisturePercent();
	bool											setMoisturePercent(const float& _moisture);
	std::string										getName();
	uint8_t											getId();
	void											updateRaw(const uint32_t &_raw_measurement);
	void											setPlantType(const plant_type_t& _p_type = plant_only);
	plant_type_t									getPlantType(void);
	void											setRainExposition(const bool& _rain_exposed);
	bool											isRainExposed();
};

class PlantWithDMAMoistureSensor : public PlantWithSensor {
private:
	uint32_t										quantization_levels;
	float											ref_voltage;
	float											soil_moisture_volts;
	float											soil_moisture_raw;

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
	float											getMoisturePercent();
	float											getMoistureVolts();
	float											getMoistureRaw();
	std::string										getName();
	uint8_t											getId();
	void											updateRaw(const uint32_t& _raw_measurement);
	void											setPlantType(const plant_type_t& _p_type = plant_only);
	plant_type_t									getPlantType();
	void											setRainExposition(const bool& _rain_exposed);
	bool											isRainExposed();
};
