// SubSecondsTimeDiff.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <ctime>
#include "time.h"
#include <stdlib.h>
#include <cstdlib>

typedef struct {
	uint8_t weekday;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint32_t milliseconds;
}TimeStamp_t;


//TODO: implement TimeStamp_ts diff with milliseconds
double calculateTimeDiff(const TimeStamp_t& _past, const TimeStamp_t& _now) {
	struct tm tm_timestamp_now { _now.seconds, _now.minutes, _now.hours, _now.day, _now.month - 1, _now.year + 100 };
	struct tm tm_timestamp_past { _past.seconds, _past.minutes, _past.hours, _past.day, _past.month - 1, _past.year + 100 };
	time_t now = mktime(&tm_timestamp_now);
	time_t past = mktime(&tm_timestamp_past);

	typedef std::chrono::milliseconds milliseconds;
	std::chrono::system_clock::time_point t_now = std::chrono::system_clock::from_time_t(now) + std::chrono::milliseconds(_now.milliseconds);
	std::chrono::system_clock::time_point t_past = std::chrono::system_clock::from_time_t(past) + std::chrono::milliseconds(_past.milliseconds);
	auto dt_ms = std::chrono::duration_cast<milliseconds>(t_now - t_past);

	return dt_ms.count() / 1000.0;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	TimeStamp_t now{ 6,17,01,00,18,4,21,900 };
	TimeStamp_t prev{ 6,17,01,00,18,4,21,0 };

    std::cout << "Hello World!\n";
	uint32_t i = 0;
	while (true) {

		double result = calculateTimeDiff(prev, now);
		std::cout << result << std::endl;
		prev = { now };
		now = { 6,17,2,00,18,4,21,100+i };
		result = calculateTimeDiff(prev, now);
		std::cout << result << std::endl;
		i++;
		//Sleep(200);
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
