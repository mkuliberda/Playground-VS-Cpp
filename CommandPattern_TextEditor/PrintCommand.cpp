#include "PrintCommand.h"
#include <iostream>

void PrintCommand::execute() const
{
	std::cout<< __FUNCSIG__ << "printing..."<<std::endl;
}
