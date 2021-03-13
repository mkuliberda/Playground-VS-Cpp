#pragma once
#include "Document.h"

class PdfDocument :
	public Document
{
public:
	void Read() override;
	void Write() override;
};

