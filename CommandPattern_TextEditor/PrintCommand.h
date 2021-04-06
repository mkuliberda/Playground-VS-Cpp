#pragma once
#include "Command.h"
class PrintCommand :
	public Command
{
public:
	void execute() const;
};

