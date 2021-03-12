#include "pch.h"
#include "Logger.h"

Logger& Logger::Instance()
{
	/*std::call_once(flag, []() {
		p_instance = new Logger;
	});*/
		return *p_instance;
}
