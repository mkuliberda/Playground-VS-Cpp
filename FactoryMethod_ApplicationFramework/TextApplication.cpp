#include "TextApplication.h"
#include "TextDocument.h"
#include <iostream>

DocumentPtr TextApplication::Create()
{
	std::cout << __FUNCSIG__ << std::endl;
	return std::make_unique<TextDocument>();
}

