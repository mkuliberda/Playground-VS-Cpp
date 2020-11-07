#pragma once
#include "main.h"
#include "sensors.h"
#include <iostream> //TODO: delete on STM32
#include <vector>
#include <string>

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


public:
	//std::vector<std::unique_ptr<PlantInterface>>		vPlants;

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

	Watertank(const uint8_t& _id) {
		watertank_info.id = _id;
		std::cout << "Standard Constructor Watertank" << std::endl;
	}
	Watertank(const uint8_t&& _id) {
		watertank_info.id = std::move(_id);
		std::cout << "Move Constructor Watertank" << std::endl;
	}
	~Watertank() {
		std::cout << "Watertank dtor" << std::endl;
	}

};

/**
 * The Builder interface specifies methods for creating the different parts of
 * the Watetank objects.
 */
class WatertankBuilder {
public:
	virtual ~WatertankBuilder() {}
	virtual WatertankBuilder& ProducePartA() = 0;
	virtual WatertankBuilder& ProducePartB() = 0;
	virtual WatertankBuilder& ProducePartC() = 0;

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
		this->Reset();
	}

	~ConcreteWatertankBuilder() {
		std::cout << "ConcreteWatertankBuilder dtor" << std::endl;
	}

	void												Reset();
	ConcreteWatertankBuilder&							ProducePartA() override;
	ConcreteWatertankBuilder&							ProducePartB() override;
	ConcreteWatertankBuilder&							ProducePartC() override;
	std::unique_ptr<Watertank>							GetProduct();
};
