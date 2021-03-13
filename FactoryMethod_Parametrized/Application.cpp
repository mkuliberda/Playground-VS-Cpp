#include "Application.h"
#include "DocumentFactory.h"
#include <iostream>


void Application::New(const std::string& file_name)
{
	std::cout << __FUNCSIG__ << std::endl;
	p_document = DocumentFactory::Create(file_name);
}

void Application::Open(const std::string& file_name)
{
	std::cout << __FUNCSIG__ << std::endl;
	p_document = DocumentFactory::Create(file_name);
	p_document->Read();
}

void Application::Save()
{
	std::cout << __FUNCSIG__ << std::endl;
	p_document->Write();
}
