#include "Application.h"
#include "Document.h"
#include <iostream>

DocumentPtr Application::Create()
{
	std::cout << __FUNCSIG__ << std::endl;
	return nullptr;
}

void Application::New()
{
	std::cout << __FUNCSIG__ << std::endl;
	p_document = Create();
}

void Application::Open()
{
	std::cout << __FUNCSIG__ << std::endl;
	p_document = Create();
	p_document->Read();
}

void Application::Save()
{
	std::cout << __FUNCSIG__ << std::endl;
	p_document->Write();
}
