#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string_view>

// This is now Meyer's singleton

#define FA_WRITE 0
#define FA_OPEN_APPEND 1
#define FR_OK (bool)true;
#define LOG_TEXT_LEN 28
#define TIME_FORMAT "%02d-%02d-%02d %02d:%02d:%02d.%03d"
#define TIME_FORMAT_LEN (sizeof(TIME_FORMAT))

#define REPORTERS C(Sector1)C(Sector2)C(Sector3)C(Sector4)C(Task_SDCard)C(Task_Irrigation)C(Task_Wireless)C(Task_SysMonitor)C(Generic)
#define C(x) x,
enum reporter_t { REPORTERS TOP };
#undef C
#define C(x) #x,
const char * const reporter[] = { REPORTERS };

struct log_time {
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t day;
	uint8_t month;
	uint8_t year;
};

struct log_msg {
	char 			text[LOG_TEXT_LEN];
	uint8_t			len;
	reporter_t 		reporter_id;
	log_time		time;
};



typedef char TCHAR;
typedef struct FIL {
	uint8_t sim;
};
typedef struct FATFS {
	uint8_t sim;
};
typedef struct DIR {
	uint8_t sim;
};
typedef struct FILINFO {
	uint8_t sim;
};

class HAL_FatFs_Logger
{
private:
	uint32_t log_text_max_len{ LOG_TEXT_LEN };
	const char *file_name = "LOG.TXT";
	const TCHAR *log_format = "%02d-%02d-%02d %02d:%02d:%02d %s: %s\n";
	FIL log_file;
	char logical_drive[4] = { 0, 0, 0, 0 };   /* SD logical drive path */
	FATFS file_system;    /* File system object for SD logical drive */
	DIR directory;
	FILINFO file_info;
	char cwd_buffer[80] = "/";
	const char time_format[TIME_FORMAT_LEN] = TIME_FORMAT;
	std::vector<std::pair<std::string_view, int>> logs;

	HAL_FatFs_Logger() {};

public:
	~HAL_FatFs_Logger() = default;
	HAL_FatFs_Logger(const HAL_FatFs_Logger&) = delete;
	HAL_FatFs_Logger& operator =(const HAL_FatFs_Logger&) = delete;

	static HAL_FatFs_Logger& createInstance();
	void writeLog(log_msg *msg);
	void setMsgMaxLength(const uint32_t& _log_text_max_len);
	void accumulateLogs();
	void releaseLogsToFile();

};