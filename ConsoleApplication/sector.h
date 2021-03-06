#pragma once
#include <string>
#include <iostream>  //TODO: delete on STM32
#include "main.h"
#include <vector>
#include "plants.h"
#include "pumps.h"

#define PLANT_NAME_LEN 20
#define PLANT_ALIAS_LEN 4
#define PLANTS_COUNT_LIMIT 20
#define PLANTS_FIELD_LEN PLANT_ALIAS_LEN * PLANTS_COUNT_LIMIT + (PLANTS_COUNT_LIMIT-1)

struct IrrigationSectorInfo_s {
	uint32_t state;
	uint32_t errors;
	uint8_t id;
	char plants_aliases[PLANTS_FIELD_LEN];
};

/**
 * It makes sense to use the Builder pattern only when your sectors are quite
 * complex and require extensive configuration.
 *
 * Unlike in other creational patterns, different concrete builders can produce
 * unrelated sectors. In other words, results of various builders may not
 * always follow the same interface.
 */

class IrrigationSector{
private:
	const uint8_t 										plants_count_limit = PLANTS_COUNT_LIMIT;
	const float											plant_dry_level = 10;
	bool												watering;
	struct IrrigationSectorInfo_s						sector_info;

	void												encodeErrors();
	void												encodeState();
	void												encodePlants();


public:
	std::vector<std::unique_ptr<PlantInterface>>		vPlants;
	PumpController										pump_controller;

	const uint8_t&										getId();
	const uint8_t&										getPlantsCountLimit();
	uint8_t												getPlantsCount() const;
	float												getPlantHealth(const std::string& _name) const;
	float												getPlantHealth(const uint8_t& _id) const;
	void												setWateringState(const bool& _watering_active);
	const bool&											getWateringState();
	void												update();
	const struct IrrigationSectorInfo_s&				getInfo();
	void												addPlantNameToList(const std::string&& _plant_name) const;
	bool												setPlantMoistureByName(const std::string& _plant_name, const float& _moisture_percent) const;

    std::vector<std::string> parts_;	//TODO: delete on STM32
    void ListParts()const {				//TODO: delete on STM32
        std::cout << "IrrigationSector parts: ";
        for (size_t i=0;i<parts_.size();i++){
            if(parts_[i]== parts_.back()){
                std::cout << parts_[i];
            }else{
                std::cout << parts_[i] << ", ";
            }
        }
        std::cout << "\n\n"; 
    }

	IrrigationSector(const uint8_t& _id){
		sector_info.id = _id;
		std::cout << "Standard Constructor IrrigationSector" << std::endl;//TODO: delete on STM32
	}
	IrrigationSector(const uint8_t&& _id){
		sector_info.id = std::move(_id);
		std::cout << "Move Constructor IrrigationSector" << std::endl;//TODO: delete on STM32
	}
	~IrrigationSector() {
		std::cout << "IrrigationSector dtor" << std::endl;//TODO: delete on STM32
	}

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	IrrigationSector(IrrigationSector const &) = delete;
	IrrigationSector& operator=(IrrigationSector const&) = delete;

};


/**
 * The Builder interface specifies methods for creating the different parts of
 * the IrrigationSector objects.
 */
class IrrigationSectorBuilder{
    public:
    virtual ~IrrigationSectorBuilder(){}
    virtual IrrigationSectorBuilder& ProducePartA() =0;
    virtual IrrigationSectorBuilder& ProducePartB() =0;
    virtual IrrigationSectorBuilder& ProducePartC() =0;
	virtual IrrigationSectorBuilder& producePlantWithDMAMoistureSensor(const std::string& _p_name, const float& _ref_voltage = 3.3,
		const uint32_t& _quantization_levels = 4095) =0;
	virtual IrrigationSectorBuilder& produceDRV8833PumpWithController(const pump_controller_mode_t& _controller_mode, const uint32_t& _idletime_required_seconds,
		const uint32_t& _runtime_limit_seconds, const std::array<struct gpio_s, 2>& _pinout, const struct gpio_s& _led_pinout, 
		const struct gpio_s& _fault_pinout, const struct gpio_s& _mode_pinout) = 0;
	virtual IrrigationSectorBuilder& produceDRV8833PumpWithController(const pump_controller_mode_t& _controller_mode, const uint32_t& _idletime_required_seconds,
		const uint32_t& _runtime_limit_seconds, const std::array<struct gpio_s, 4>& _pinout, const struct gpio_s& _led_pinout,
		const struct gpio_s& _fault_pinout, const struct gpio_s& _mode_pinout) = 0;
	virtual IrrigationSectorBuilder& produceBinaryPumpWithController(const pump_controller_mode_t& _controller_mode, const uint32_t& _idletime_required_seconds,
		const uint32_t& _runtime_limit_seconds, const struct gpio_s& _pinout, const struct gpio_s& _led) = 0;
};
/**
 * The Concrete Builder classes follow the Builder interface and provide
 * specific implementations of the building steps. Your program may have several
 * variations of Builders, implemented differently.
 */
class ConcreteIrrigationSectorBuilder : public IrrigationSectorBuilder{
private:
	std::unique_ptr<IrrigationSector> sector;
	int8_t sector_count = -1;

    /**
     * A fresh builder instance should contain a blank sector object, which is
     * used in further assembly.
     */
public:

    ConcreteIrrigationSectorBuilder(){
        this->Reset();
    }

    ~ConcreteIrrigationSectorBuilder(){
		std::cout << "ConcreteIrrigationSectorBuilder dtor" << std::endl;//TODO: delete on STM32
    }

	//To avoid runtime errors, delete copy constructor and copy assignment operator. If sth's wrong, compile time error will fire.
	ConcreteIrrigationSectorBuilder(ConcreteIrrigationSectorBuilder const &) = delete;
	ConcreteIrrigationSectorBuilder& operator=(ConcreteIrrigationSectorBuilder const&) = delete;

	void														Reset();
	ConcreteIrrigationSectorBuilder&							ProducePartA() override;
	ConcreteIrrigationSectorBuilder&							ProducePartB() override;
	ConcreteIrrigationSectorBuilder&							ProducePartC() override;
	ConcreteIrrigationSectorBuilder&							producePlantWithDMAMoistureSensor(const std::string& _p_name, const float& _ref_voltage = 3.3,
																const uint32_t& _quantization_levels = 4095) override;
	ConcreteIrrigationSectorBuilder&							produceDRV8833PumpWithController(const pump_controller_mode_t& _controller_mode, const uint32_t& _idletime_required_seconds, \
																const uint32_t& _runtime_limit_seconds, const std::array<struct gpio_s, 2>& _pinout, const struct gpio_s& _led_pinout, \
																const struct gpio_s& _fault_pinout, const struct gpio_s& _mode_pinout) override;
	ConcreteIrrigationSectorBuilder&							produceDRV8833PumpWithController(const pump_controller_mode_t& _controller_mode, const uint32_t& _idletime_required_seconds, \
																const uint32_t& _runtime_limit_seconds, const std::array<struct gpio_s, 4>& _pinout, const struct gpio_s& _led_pinout, \
																const struct gpio_s& _fault_pinout, const struct gpio_s& _mode_pinout) override;
	ConcreteIrrigationSectorBuilder&							produceBinaryPumpWithController(const pump_controller_mode_t& _controller_mode, const uint32_t& _idletime_required_seconds, \
																const uint32_t& _runtime_limit_seconds, const struct gpio_s& _pinout, const struct gpio_s& _led) override;
	std::unique_ptr<IrrigationSector>							GetProduct();
};

