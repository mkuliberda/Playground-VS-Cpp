#include "watertank.h"
#include <bitset>



void ConcreteWatertankBuilder::Reset() {
	watertank.reset(new Watertank(static_cast<uint8_t>(++watertank_count)));
}
/**
 * All production steps work with the same watertank instance.
 */

ConcreteWatertankBuilder& ConcreteWatertankBuilder::ProducePartA() {  //TODO: delete this on STM32
	watertank->parts_.push_back("PartA1");
	return *this;
}

ConcreteWatertankBuilder& ConcreteWatertankBuilder::ProducePartB() {  //TODO: delete this on STM32
	watertank->parts_.push_back("PartB1");
	return *this;
}

ConcreteWatertankBuilder& ConcreteWatertankBuilder::ProducePartC() {  //TODO: delete this on STM32
	watertank->parts_.push_back("PartC1");
	return *this;
}

std::unique_ptr<Watertank> ConcreteWatertankBuilder::GetProduct() {
	std::unique_ptr<Watertank>(result);
	result = std::move(watertank);
	Reset();
	return result;
}