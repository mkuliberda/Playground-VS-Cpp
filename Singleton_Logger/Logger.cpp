#include "Logger.h"

bool f_close(FIL *file) {
	return true;
}

bool f_open(FIL *log_file, const char *file_name, uint8_t flags) {
	return true;
}

bool f_printf(
	FIL* fp,			/* Pointer to the file object */
	const TCHAR* fmt,	/* Pointer to the format string */
	...					/* Optional arguments... */
)
{
	va_list arp;
	TCHAR c, d, str[32], *p;

	return true;
}


HAL_FatFs_Logger& HAL_FatFs_Logger::createInstance()
{
	static HAL_FatFs_Logger instance;
	return instance;
}

void HAL_FatFs_Logger::writeLog(log_msg *msg)
{
	std::cout << __FUNCSIG__ << std::endl;
	char text[] = "";
	strncpy(text, msg->text, msg->len);
	if (f_open(&log_file, file_name, FA_WRITE | FA_OPEN_APPEND)) {
		f_printf(&log_file, log_format, msg->time.day, msg->time.month, msg->time.year, msg->time.hours, msg->time.minutes, msg->time.seconds, reporter[msg->reporter_id], text);
		while (!f_close(&log_file));
	}

}

void HAL_FatFs_Logger::setMsgMaxLength(const uint32_t & _log_text_max_len)
{
	if (_log_text_max_len > 0) log_text_max_len = _log_text_max_len;
}
