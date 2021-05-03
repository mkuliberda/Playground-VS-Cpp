#ifndef __HAL_UART_MsgBroker_H
#define __HAL_UART_MsgBroker_H

#pragma once

#include "MsgBroker.h"
#include <iostream>
#include <string>
#include "stm32f4xx_hal.h"

#define BUFFER_SIZE 64

class HAL_UART_DMA_MsgBroker :
	public MsgBroker
{
public:

	HAL_UART_DMA_MsgBroker() =default;
    HAL_UART_DMA_MsgBroker(void* _dev_handle){
       if (_dev_handle != nullptr){
		   UART_Handle = (UART_HandleTypeDef*)_dev_handle;
		   devValid = true;
	   } 
    }
	bool assignDevice(void* _dev_handle) override;
	bool sendMsg(const ExternalObject& _recipient, const std::string& _msg, const bool& _wait_until_cplt = false) override;
	bool publishData(const ExternalObject& _recipient, const char* _publisher, std::unordered_map<const char*, int32_t> _values, const bool& _wait_until_cplt = false) override;
	bool requestData(const ExternalObject& _recipient, const std::string& _data_key, const bool& _wait_until_cplt = false) override;
	//bool setParser(MsgParser *_parser) override;
	// setEncoder(MsgEncoder *_encoder) override;
	void setAddresses(std::unordered_map<ExternalObject_t, std::string> &_addresses) override;
	bool readData(const size_t& _size, void(*action)(const std::string&)) override;
	bool readData(const size_t& _size) override;
	HAL_UART_DMA_MsgBroker(HAL_UART_DMA_MsgBroker const &) = delete;
	HAL_UART_DMA_MsgBroker& operator=(HAL_UART_DMA_MsgBroker const&) = delete;
	~HAL_UART_DMA_MsgBroker() =default;


private:
    bool transmit(const std::string& _str, const bool& _blocking_mode); 
	const char *pub_hdr = "$PUB\\";
	const char *get_hdr = "$GET\\";
	const char *msg_ending = "\n";
	UART_HandleTypeDef *UART_Handle{};
	uint8_t txBuffer[BUFFER_SIZE]{};
	uint8_t rxBuffer[BUFFER_SIZE]{};
	std::unordered_map<ExternalObject_t, std::string> address_map;
	//MsgParser *parserInstance{};
	bool devValid{false};
};

#endif

