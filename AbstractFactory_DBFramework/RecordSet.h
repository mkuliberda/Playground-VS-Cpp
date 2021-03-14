#pragma once
#include <string>

//Just a helper class simulate database response

class RecordSet
{
public:
	virtual const std::string & Get() = 0;
	virtual bool HasNext() = 0;
	virtual ~RecordSet() = default;
};

