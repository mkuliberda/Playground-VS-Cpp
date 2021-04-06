#include "CopyCommand.h"
#include <iostream>

void CopyCommand::execute() const
{
	std::cout <<__FUNCSIG__ << "copying..." << std::endl;
}
