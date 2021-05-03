#pragma once
#include <string>
#include <unordered_map>


enum class ExternalObject_t {
	ntp_server,
	raspberry_pi,
	google_home,
	my_phone,
	broadcast
};

enum class InternalObject_t {
	unspecified,
	sector,
	plant,
	watertank,
	system,
	all
};

enum class MsgType {
	unknown,
	command,
	query,
	info
};

enum class CommandType {
	none,
	irrigation_start,
	irrigation_stop
};

enum class QueryType {
	none,
	get_status,
	get_time_and_date
};

inline std::unordered_map<ExternalObject_t, std::string> esp01s_external_addresses{  // NOLINT(clang-diagnostic-exit-time-destructors)
	{ExternalObject_t::ntp_server, "ntp\\"},
	{ExternalObject_t::raspberry_pi, "rpi\\"},
	{ExternalObject_t::google_home, "goo\\"},
	{ExternalObject_t::broadcast, "all\\"},
};

inline std::unordered_map<ExternalObject_t, std::string> mobile_numbers{  // NOLINT(clang-diagnostic-exit-time-destructors)
	{ExternalObject_t::my_phone, "506696574"},
};

struct ExternalObject {

	ExternalObject_t object;
	uint16_t id;
};

struct InternalObject {

	InternalObject_t object;
	uint16_t id;
};

struct InternalMessage
{
	InternalObject recipient;
	InternalObject sender;
	MsgType msg_type;
};

struct IncomingMessage
{
	ExternalObject sender;
	InternalObject recipient;
	MsgType msg_type;

};

struct OutgoingMessage
{
	ExternalObject recipient;
	InternalObject sender;
	MsgType msg_type;
};
