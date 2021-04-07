#pragma once
#include "MsgBroker.h"
#include <iostream>
#include <string>

/* TODO: remove on stm32 */
enum Result {
	HAL_FAIL,
	HAL_OK
};

struct UART_HandleTypeDef {
	unsigned int address;
	int value;
};

bool HAL_UART_Transmit_DMA(UART_HandleTypeDef *uart, uint8_t *buffer, unsigned short size);
bool HAL_UART_Receive_DMA(UART_HandleTypeDef *uart, uint8_t *buffer, unsigned short size);

#define RX_BUFFER_SIZE 64

class HAL_UART_MsgBroker :
	public MsgBroker
{
public:
	~HAL_UART_MsgBroker() =default;
	bool assignDevice(void* DevHandle) override;
	bool sendMsg(const recipient_t& _recipient, const std::string& _msg) override;
	bool publishData(const recipient_t& _recipient, const char* _publisher, const std::map<const char*, double> &_values) override;
	bool requestData(const recipient_t& _recipient, const std::string& _data_key) override;
private:
	const char *pub_hdr = "PUB\\";
	const char *get_hdr = "GET\\";
	UART_HandleTypeDef *UART_Handle{};
	uint8_t RxBuffer[RX_BUFFER_SIZE]{};
};

