#pragma once
#include "MsgBroker.h"
#include <iostream>

/* TODO: remove on stm32 */
enum Result {
	HAL_FAIL,
	HAL_OK
};

struct UART_HandleTypeDef {
	unsigned int address;
	int value;
};

bool HAL_UART_Transmit_DMA(UART_HandleTypeDef *uart, uint8_t *buffer, unsigned short size) {
	std::cout << __FUNCSIG__ << std::endl;
	return true;
}
bool HAL_UART_Receive_DMA(UART_HandleTypeDef *uart, uint8_t *buffer, unsigned short size) {
	std::cout << __FUNCSIG__ << std::endl;
	return true;
}

#define RX_BUFFER_SIZE 64

class HAL_UART_MsgBroker :
	public MsgBroker
{
public:
	bool assignDevice(void* DevHandle) override;
	bool sendMsg(const recipient_t& _recipient, const std::string& _msg) override;
	bool publishData(const recipient_t& _recipient, std::map<std::string_view, double> *values) override;
	bool publishData(const recipient_t& _recipient, std::map<std::string_view, uint8_t> *values) override;
	double requestData(const recipient_t& _recipient, const std::string_view& _data_addr) override;
private:
	UART_HandleTypeDef *UART_Handle{};
	uint8_t RxBuffer[RX_BUFFER_SIZE]{};
};

