#include "PdfApplication.h"
#include <iostream>

DocumentPtr PdfApplication::Create()
{
	std::cout << __FUNCSIG__ << std::endl;
	return std::make_unique<PdfDocument>();
}
