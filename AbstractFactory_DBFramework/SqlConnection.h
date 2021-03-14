#pragma once
#include "Connection.h"
#include <iostream>

class SqlConnection :
	public Connection
{
	void Open() override;
};

