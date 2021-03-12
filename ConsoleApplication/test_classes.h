#pragma once

#include <iostream>

class Base
{
public:
	void func(float x) { std::cout << "Base::func " << x << std::endl; }
};

class Derived : public Base
{
public:
	void func(double x) { std::cout << "Derived::func " << x <<std::endl; }
};

class IBase
{
public:
	virtual void func(float x) = 0;
};

class ImpBase : public IBase{
public:
	virtual void func(float x) { std::cout << "ImpBase::func " << x << std::endl; };
};

class Client
{
private:
	IBase *base;
public:
	Client(IBase *_base):
		base(_base)
	{};
	~Client() {};
	
	void update(const float& _dt) { base->func(_dt); };
};
