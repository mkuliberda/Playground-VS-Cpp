#include "HAL_UART_MsgBroker.h"
#include <string>

bool HAL_UART_MsgBroker::assignDevice(void * DevHandle)
{
	if (DevHandle == nullptr) return false;
	UART_Handle = (UART_HandleTypeDef*)DevHandle;
	return true;
}

bool HAL_UART_MsgBroker::sendMsg(const recipient_t& _recipient, const std::string& _msg)
{
	std::string str_msg{};
	bool result = false;

	switch (_recipient) {
	case recipient_t::raspberry_pi:
		str_msg = "POST:\\\\rpi\\" + _msg;
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length) == HAL_OK;
		break;
	case recipient_t::google_home:
		str_msg = "POST:\\\\google_home\\" + _msg;
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length) == HAL_OK;
		break;
	default:
		break;
	}
	return result;
}

bool HAL_UART_MsgBroker::publishData(const recipient_t& _recipient, std::map<std::string_view, double>* values)
{
	return false;
}

double HAL_UART_MsgBroker::requestData(const recipient_t& _recipient, const std::string_view& _data_addr)
{

	HAL_UART_Receive_DMA(UART_Handle, RxBuffer, sizeof(RxBuffer));
	return 0.0;
}
