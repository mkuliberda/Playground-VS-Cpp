#pragma once
#include "Connection.h"
#include <iostream>

class SqlConnection :
	public Connection
{
public:
	void Open() override;
};

