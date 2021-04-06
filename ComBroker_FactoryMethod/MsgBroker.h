#pragma once
#include <string>
#include <map>


typedef unsigned char uint8_t;//TODO: delete on stm32

enum recipient_t : uint8_t {
	ntp_server,
	raspberry_pi,
	google_home,
	my_phone
};

class MsgBroker
{
public:
	virtual bool assignDevice(void* DevHandle) =0;
	virtual bool sendMsg(const recipient_t& _recipient, const std::string& _msg) =0;
	virtual bool publishData(const recipient_t& _recipient, std::map<std::string_view, double> *values) =0;
	virtual bool publishData(const recipient_t& _recipient, std::map<std::string_view, uint8_t> *values) = 0;
	virtual double requestData(const recipient_t& _recipient, const std::string_view& _data_addr) =0;
	virtual ~MsgBroker() =0;

};

