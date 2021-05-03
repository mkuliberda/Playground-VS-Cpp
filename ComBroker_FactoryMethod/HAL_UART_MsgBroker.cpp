#include "HAL_UART_MsgBroker.h"
#include <string>

bool HAL_UART_Transmit_DMA(UART_HandleTypeDef *uart, uint8_t *buffer, unsigned short size) {
	std::string str{};
	for (int i = 0; i < size; ++i) str += buffer[i];
	std::cout << str << std::endl;
	return true;
}
bool HAL_UART_Receive_DMA(UART_HandleTypeDef *uart, uint8_t *buffer, unsigned short size) {
	std::cout << __FUNCSIG__ << std::endl;
	return true;
}

bool HAL_UART_DMA_MsgBroker::assignDevice(void * _dev_handle)
{
	if (_dev_handle == nullptr) return false;
	UART_Handle = (UART_HandleTypeDef*)_dev_handle;
	return true;
}

bool HAL_UART_DMA_MsgBroker::sendMsg(const recipient_t& _recipient, const std::string& _msg)
{
	std::string str_msg{pub_hdr};
	bool result = false;

	switch (_recipient) {
	case recipient_t::raspberry_pi:
		str_msg += "rpi\\{\"Msg\":\"" + _msg + "\"}";
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length()) == HAL_OK;
		break;
	case recipient_t::google_home:
		str_msg += "google_home\\{\"Msg\":\"" + _msg + "\"}";
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length()) == HAL_OK;
		break;
	case recipient_t::broadcast:
		str_msg += "all\\{\"Msg\":\"" + _msg + "\"}";
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length()) == HAL_OK;
		break;
	default:
		break;
	}
	return result;
}

bool HAL_UART_DMA_MsgBroker::publishData(const recipient_t& _recipient, const char* _publisher, const std::map<const char*, double> &_values)
{
	std::string str_msg{ "{\"" };
	str_msg += _publisher;
	str_msg+="\":[";
	bool result = false;
	//std::cout << str_msg << std::endl;

	for (const auto&[key, value] : _values) {
		str_msg += '\"';
		str_msg += key;
		str_msg += '\"';
		str_msg += ':';
		str_msg += std::to_string(value);
		str_msg += ",";
	}
	str_msg.pop_back();
	str_msg += "]}";

	//std::cout <<str_msg<<std::endl;

	switch (_recipient) {
	case recipient_t::raspberry_pi:
		str_msg = (static_cast<std::string>(pub_hdr) + "rpi\\" + str_msg);
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length()) == HAL_OK;
		break;
	case recipient_t::google_home:
		str_msg = (static_cast<std::string>(pub_hdr) + "google_home\\" + str_msg);
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length()) == HAL_OK;
		break;
	case recipient_t::broadcast:
		str_msg = (static_cast<std::string>(pub_hdr) + "all\\" + str_msg);
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length()) == HAL_OK;
		break;
	default:
		break;
	}
	return result;
}

bool HAL_UART_DMA_MsgBroker::requestData(const recipient_t& _recipient, const std::string& _data_key)
{
	std::string str_msg{ get_hdr };
	bool result = false;

	switch (_recipient) {
	case recipient_t::raspberry_pi:
		str_msg += "rpi\\" + _data_key;
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length()) == HAL_OK;
		break;
	case recipient_t::google_home:
		str_msg += "google_home\\" + _data_key;
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length()) == HAL_OK;
		break;
	case recipient_t::ntp_server:
		str_msg += "ntp_server\\" + _data_key;
		result = HAL_UART_Transmit_DMA(UART_Handle, (uint8_t*)str_msg.c_str(), str_msg.length()) == HAL_OK;
		break;
	default:
		break;
	}
	return result;
}

