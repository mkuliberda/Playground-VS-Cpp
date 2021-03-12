#pragma once
#include "plants.h"
#include <mutex>

//std::mutex m_mtx;

class SimpleClass
{
	Plant *plant_;
public:
	void createSth(Plant* &plant);
};

