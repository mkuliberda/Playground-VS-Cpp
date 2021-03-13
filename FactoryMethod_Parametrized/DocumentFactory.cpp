#include "DocumentFactory.h"
#include <sstream>
#include <iostream>
#include "PdfDocument.h"
#include "TextDocument.h"
#include <iostream>


std::vector<std::string> DocumentFactory::string_split(const std::string & s, char delimiter)
{
	//std::cout << __FUNCSIG__ << std::endl;
	std::vector<std::string> splits;
	std::string split;
	std::istringstream ss(s);
	while (std::getline(ss, split, delimiter))
	{
		splits.push_back(split);
	}
	return splits;
}

DocumentPtr DocumentFactory::Create(const std::string & document_name)
{
	std::cout << __FUNCSIG__ << std::endl;
	if (string_split(document_name, '.').back() == "txt") {
		return std::make_unique<TextDocument>();
	}
	else if (string_split(document_name, '.').back() == "pdf") {
		return std::make_unique<PdfDocument>();
	}
	else return nullptr;
}
