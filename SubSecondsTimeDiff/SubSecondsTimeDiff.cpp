// SubSecondsTimeDiff.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>

typedef struct {
	uint8_t weekday;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t day;
	uint8_t month;
	uint8_t year;
	double seconds_fraction;
}TimeStamp_t;


//TODO: implement TimeStamp_ts diff with milliseconds
double calculateTimeDiff(const TimeStamp_t& _past, const TimeStamp_t& _now) {
	struct tm tm_timestamp_now { _now.seconds, _now.minutes, _now.hours, _now.day, _now.month - 1, _now.year + 100 };
	struct tm tm_timestamp_past { _past.seconds, _past.minutes, _past.hours, _past.day, _past.month - 1, _past.year + 100 };
	time_t now = mktime(&tm_timestamp_now);
	time_t past = mktime(&tm_timestamp_past);
	return difftime(now, past);
}

int main()
{
    std::cout << "Hello World!\n";
	TimeStamp_t now{};
	TimeStamp_t prev{};
	std::cout << calculateTimeDiff(prev, now) << std::endl;
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
