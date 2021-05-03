#ifndef __MsgBroker_H
#define __MsgBroker_H

#pragma once
#include <map>
#include <string>
#include <memory>
#include <unordered_map>
#include "Messages.h"


// #define RASPBERRY_PI_STR "rpi\\"
// #define GOOGLE_HOME_STR "goo\\"
// #define NTP_SERVER_STR "ntp\\"
// #define BROADCAST_STR "all\\"


class MsgBroker
{
public:
	virtual bool assignDevice(void* _dev_handle) =0;
	virtual bool sendMsg(const ExternalObject& _recipient, const std::string& _msg, const bool& _wait_until_cplt = false) =0;
	virtual bool publishData(const ExternalObject& _recipient, const InternalObject& _publisher, std::unordered_map<const char*, int32_t> _values, const bool& _wait_until_cplt = false) =0;
	virtual bool requestData(const ExternalObject& _recipient, const std::string& _data_key, const bool& _wait_until_cplt = false) =0;
	virtual bool readData(const size_t& _size, void(*action)(const std::string&)) =0;
	virtual bool readData(const size_t& _size) =0;
	//virtual bool setParser(MsgParser *_parser) =0;
	//virtual bool setEncoder(MsgEncoder *_encoder) =0;
	virtual void setExternalAddresses(std::unordered_map<ExternalObject_t, std::string>& _addresses) =0;
	virtual void setInternalAddresses(std::unordered_map<InternalObject_t, std::string>& _addresses) =0;
	virtual ~MsgBroker() =default;

};

using MsgBrokerPtr = std::unique_ptr<MsgBroker>; //alias for MsgBroker pointer

#endif