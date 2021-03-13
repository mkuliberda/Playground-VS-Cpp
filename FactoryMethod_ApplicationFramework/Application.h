#pragma once
#include <memory>
#include "Document.h"

using DocumentPtr = std::unique_ptr<Document>; //alias for Document pointer

class Application
{
private:
	DocumentPtr p_document;

	virtual DocumentPtr Create();

public:

	void New();
	void Open();
	void Save();
};

