#pragma once
#include "Application.h"
#include "PdfDocument.h"

class PdfApplication :
	public Application
{
private:
	DocumentPtr Create() override;
};

