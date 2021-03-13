#pragma once
#include "Document.h"
#include <string>

class Application
{
private:
	DocumentPtr p_document;

public:
	void New(const std::string& file_name);
	void Open(const std::string& file_name);
	void Save();
};

