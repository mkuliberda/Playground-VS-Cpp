#pragma once
#include <string>
#include <iostream>
#include "main.h"
#include <vector>
#include "plants.h"
#include "pumps.h"
/**
 * It makes sense to use the Builder pattern only when your sectors are quite
 * complex and require extensive configuration.
 *
 * Unlike in other creational patterns, different concrete builders can produce
 * unrelated sectors. In other words, results of various builders may not
 * always follow the same interface.
 */

class Sector{
private:
	const uint8_t 										id;
	const uint8_t 										plants_count_limit = 20;
	uint8_t 											plants_count;
	uint8_t 											encoded_status;
	bool												watering;

public:
	std::vector<std::unique_ptr<PlantInterface>>		vPlants;
	PumpController										pump_controller;

	const uint8_t getId() const{
		return id;
	}

	const uint8_t getPlantsCountLimit() const{
		return plants_count_limit;
	}

	uint8_t getPlantsCount() const {
		return plants_count;
	}

	uint8_t incrementPlantsCount(){
		return plants_count++ <= plants_count_limit ? plants_count : plants_count_limit;
	}
    std::vector<std::string> parts_;
    void ListParts()const{
        std::cout << "Sector parts: ";
        for (size_t i=0;i<parts_.size();i++){
            if(parts_[i]== parts_.back()){
                std::cout << parts_[i];
            }else{
                std::cout << parts_[i] << ", ";
            }
        }
        std::cout << "\n\n"; 
    }
	Sector(const uint8_t& _id):
		id(_id)
	{
		std::cout << "Standard Constructor Sector" << std::endl;
	}
	Sector(const uint8_t&& _id):
		id(std::move(_id)) 
	{
		std::cout << "Move Constructor Sector" << std::endl;
	}
	~Sector() {
		std::cout << "Sector dtor" << std::endl;
	}

};


/**
 * The Builder interface specifies methods for creating the different parts of
 * the Sector objects.
 */
class SectorBuilder{
    public:
    virtual ~SectorBuilder(){}
    virtual void ProducePartA() const =0;
    virtual void ProducePartB() const =0;
    virtual void ProducePartC() const =0;
	virtual void producePlantWithDMAMoistureSensor(const std::string& _p_name, const float& _ref_voltage = 3.3, const uint32_t& _quantization_levels = 4095) const =0;
	virtual void producePumpWithController(const pumpcontrollermode_t& _controller_mode, const uint32_t& _idletime_required_seconds, 
		const uint32_t& _runtime_limit_seconds, const std::array<struct gpio_s, 2>& _pinout, const struct gpio_s& _led_pinout, 
		const struct gpio_s& _fault_pinout, const struct gpio_s& _mode_pinout) const =0;
	virtual void producePumpWithController(const pumpcontrollermode_t& _controller_mode, const uint32_t& _idletime_required_seconds, 
		const uint32_t& _runtime_limit_seconds, const struct gpio_s& _pinout, const struct gpio_s& _led) const =0;
};
/**
 * The Concrete Builder classes follow the Builder interface and provide
 * specific implementations of the building steps. Your program may have several
 * variations of Builders, implemented differently.
 */
class ConcreteSectorBuilder : public SectorBuilder{
    private:
	std::unique_ptr<Sector> sector;
	int8_t sector_count = -1;

    /**
     * A fresh builder instance should contain a blank sector object, which is
     * used in further assembly.
     */
    public:

    ConcreteSectorBuilder(){
        this->Reset();
    }

    ~ConcreteSectorBuilder(){
		std::cout << "ConcreteSectorBuilder dtor" << std::endl;
    }

    void Reset(){
		this->sector.reset(new Sector(static_cast<uint8_t>(++sector_count)));
    }
    /**
     * All production steps work with the same sector instance.
     */

    void ProducePartA()const override{
        this->sector->parts_.push_back("PartA1");
    }

    void ProducePartB()const override{
        this->sector->parts_.push_back("PartB1");
    }

    void ProducePartC()const override{
        this->sector->parts_.push_back("PartC1");
    }

	void producePlantWithDMAMoistureSensor(const std::string& _p_name, const float& _ref_voltage = 3.3, const uint32_t& _quantization_levels = 4095)const override {
		uint8_t idx = this->sector->incrementPlantsCount() - 1;
		if (idx != this->sector->getPlantsCountLimit()) {
			this->sector->vPlants.emplace_back(std::unique_ptr<PlantInterface>(new PlantWithDMAMoistureSensor(new Plant(_p_name, idx), _ref_voltage, _quantization_levels)));
			this->sector->vPlants.shrink_to_fit();
			this->sector->parts_.push_back(this->sector->vPlants.at(idx)->getName());
		}
	}

	void producePumpWithController(const pumpcontrollermode_t& _controller_mode, const uint32_t& _idletime_required_seconds, const uint32_t& _runtime_limit_seconds, \
		const std::array<struct gpio_s, 2>& _pinout, const struct gpio_s& _led_pinout,
		const struct gpio_s& _fault_pinout, const struct gpio_s& _mode_pinout)const override {

		this->sector->pump_controller.setMode(_controller_mode);
		if (this->sector->pump_controller.createPump(pumptype_t::drv8833_dc) == true) {
			this->sector->pump_controller.p8833Pump->init(0, _idletime_required_seconds,
				_runtime_limit_seconds, _pinout, _led_pinout, _fault_pinout, _mode_pinout);
			this->sector->parts_.push_back("DRV8833Pump");
		}
	}

	void producePumpWithController(const pumpcontrollermode_t& _controller_mode, const uint32_t& _idletime_required_seconds, const uint32_t& _runtime_limit_seconds, \
		const struct gpio_s& _pinout, const struct gpio_s& _led)const override {

		this->sector->pump_controller.setMode(_controller_mode);
		if (this->sector->pump_controller.createPump(pumptype_t::binary) == true) {
			this->sector->pump_controller.pBinPump->init(0, _idletime_required_seconds,
				_runtime_limit_seconds,	_pinout, _led);
			this->sector->parts_.push_back("BinaryPump");
		}
	}


    /**
     * Concrete Builders are supposed to provide their own methods for
     * retrieving results. That's because various types of builders may create
     * entirely different sectors that don't follow the same interface.
     * Therefore, such methods cannot be declared in the base Builder interface
     * (at least in a statically typed programming language). Note that PHP is a
     * dynamically typed language and this method CAN be in the base interface.
     * However, we won't declare it there for the sake of clarity.
     *
     * Usually, after returning the end result to the client, a builder instance
     * is expected to be ready to start producing another sector. That's why
     * it's a usual practice to call the reset method at the end of the
     * `getProduct` method body. However, this behavior is not mandatory, and
     * you can make your builders wait for an explicit reset call from the
     * client code before disposing of the previous result.
     */

    /**
     * Please be careful here with the memory ownership. Once you call
     * GetProduct the user of this function is responsible to release this
     * memory. Here could be a better option to use smart pointers to avoid
     * memory leaks
     */

    std::unique_ptr<Sector> GetProduct() {
		std::unique_ptr<Sector>(result);
		result = std::move(this->sector);
        this->Reset();
        return result;
    }
};

