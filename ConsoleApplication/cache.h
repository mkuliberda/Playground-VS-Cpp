#pragma once

#include <string>
#include <iostream>
#include "main.h"

//Cache class defined with Cache pattern

/**
 * The Cache class defines the `GetInstance` method that serves as an
 * alternative to constructor and lets clients access the same instance of this
 * class over and over.
 */
class Cache
{

	/**
	 * The Cache's constructor should always be private to prevent direct
	 * construction calls with the `new` operator.
	 */

protected:
	Cache(const std::string _value) : value(_value)
	{
	}

	static Cache* cache;

	std::string value;

public:

	/**
	 * Caches should not be cloneable.
	 */
	Cache(Cache &other) = delete;
	/**
	 * Caches should not be assignable.
	 */
	void operator=(const Cache &) = delete;
	/**
	 * This is the static method that controls the access to the cache
	 * instance. On the first run, it creates a cache object and places it
	 * into the static field. On subsequent runs, it returns the client existing
	 * object stored in the static field.
	 */

	static Cache *GetInstance(const std::string& _value);
	/**
	 * Finally, any cache should define some business logic, which can be
	 * executed on its instance.
	 */
	void SomeBusinessLogic()
	{
		std::cout << value << "Some business logic" << std::endl;
	}

	std::string getValue() const {
		return value;
	}

	void MethodA();
	void MethodB();
	void MethodC();

};

