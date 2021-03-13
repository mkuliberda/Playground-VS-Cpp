#pragma once
#include "Application.h"


class TextApplication :
	public Application
{
private:
	DocumentPtr Create() override;
};

