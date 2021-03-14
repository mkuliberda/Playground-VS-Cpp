#pragma once
#include "Command.h"
#include "SqlRecordSet.h"

class SqlCommand :
	public Command
{
	void ExecuteCommand();
	SqlRecordSet * ExecuteQuery();
};

