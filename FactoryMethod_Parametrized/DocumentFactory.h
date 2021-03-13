#pragma once
#include <string>
#include <vector>
#include "Document.h"

class DocumentFactory
{
public:
	static std::vector<std::string> string_split(const std::string& s, char delimiter);
	static DocumentPtr Create(const std::string& document_name);
};

