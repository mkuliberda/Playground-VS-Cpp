#include "cache.h"

Cache* Cache::cache = nullptr;

/**
 * Static methods should be defined outside the class.
 */
Cache *Cache::GetInstance(const std::string& _value)
{
	/**
	 * This is a safer way to create an instance. instance = new Cache is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (cache == nullptr) {
		cache = new Cache(_value);
	}
	return cache;
}

 void Cache::MethodA()
 {
 }

 void Cache::MethodB()
 {
 }

 void Cache::MethodC()
 {
 }
