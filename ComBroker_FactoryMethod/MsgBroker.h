#pragma once
#include <string>
#include <memory>
#include <map>


typedef unsigned char uint8_t;//TODO: delete on stm32

enum recipient_t : uint8_t {
	ntp_server,
	raspberry_pi,
	google_home,
	my_phone,
	broadcast
};

class MsgBroker
{
public:
	virtual bool assignDevice(void* _dev_handle) =0;
	virtual bool sendMsg(const recipient_t& _recipient, const std::string& _msg) =0;
	virtual bool publishData(const recipient_t& _recipient, const char* _publisher, const std::map<const char*, double> &_values) =0;
	virtual bool requestData(const recipient_t& _recipient, const std::string& _data_key) =0;
	virtual ~MsgBroker() =default;

};

using MsgBrokerPtr = std::unique_ptr<MsgBroker>; //alias for MsgBroker pointer

