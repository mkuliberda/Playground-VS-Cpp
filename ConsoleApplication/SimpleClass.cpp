#include "SimpleClass.h"

void SimpleClass::createSth(Plant* &plant)
{
	//std::lock_guard<std::mutex> lock{ m_mtx };
	plant_ = new Plant("Pelargonia", 0);
	std::cout << plant->getName() << std::endl;

	delete plant_;
}
