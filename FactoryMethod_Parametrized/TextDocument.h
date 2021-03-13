#pragma once
#include "Document.h"

class TextDocument :
	public Document
{
public:
	void Read() override;
	void Write() override;
};

