#pragma once
#include "Command.h"
#include "MySqlRecordSet.h"

class MySqlCommand :
	public Command
{
	void ExecuteCommand();
	MySqlRecordSet * ExecuteQuery();
};

