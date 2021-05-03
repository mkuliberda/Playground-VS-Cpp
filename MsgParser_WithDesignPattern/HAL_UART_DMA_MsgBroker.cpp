#include "HAL_UART_DMA_MsgBroker.h"
#include "utilities.h"
#include <string>
#include <cstring>
#include "cmsis_os.h"

//extern const UBaseType_t xArrayIndex; //TODO: uncomment on STM32

bool HAL_UART_DMA_MsgBroker::assignDevice(void * _dev_handle)
{
	if (_dev_handle == nullptr) return false;
	UART_Handle = (UART_HandleTypeDef*)_dev_handle;
	return devValid = true;
}

bool HAL_UART_DMA_MsgBroker::sendMsg(const ExternalObject& _recipient, const std::string& _msg, const bool& _wait_until_cplt)
{	
	
	std::string str_msg{pub_hdr};
	const std::string msg_start{R"({"Msg":")"};
	const std::string msg_end{"\"}"};
	bool result = false;

	switch (_recipient.object) {
	case ExternalObject_t::raspberry_pi:
		str_msg += address_map[ExternalObject_t::raspberry_pi] + msg_start + _msg + msg_end + msg_ending;
        result = transmit(str_msg, _wait_until_cplt);
		break;
	case ExternalObject_t::google_home:
		str_msg += address_map[ExternalObject_t::google_home] + msg_start + _msg + msg_end + msg_ending;
        result = transmit(str_msg, _wait_until_cplt);
		break;
	case ExternalObject_t::broadcast:
		str_msg += address_map[ExternalObject_t::broadcast] + msg_start + _msg + msg_end + msg_ending;
        result = transmit(str_msg, _wait_until_cplt);
		break;
	case ExternalObject_t::ntp_server:
		break;
	case ExternalObject_t::my_phone:
		break;
	default:
		break;
	}
	return result;
}

bool HAL_UART_DMA_MsgBroker::publishData(const ExternalObject& _recipient, const char* _publisher, std::unordered_map<const char*, int32_t> _values, const bool& _wait_until_cplt)
{
	/*Header hdr{ _recipient };
	DataList data{_values};
	Footer ft {"}\n"};

	std::vector<Element*> msg_items{ &hdr, &data, &ft };
	JsonEncoder json_encoder;

	for (auto &item: msg_items)
	{
		item.accept(&json_encoder);
	}
	*/
	
	
	
	std::string str_msg{ "{\"" };
	str_msg += _publisher;
	str_msg += "\":[";
	bool result = false;

	for (const auto& kv : _values) {
		str_msg += '\"';
		str_msg += kv.first;
		str_msg += '\"';
		str_msg += ':';
		str_msg += patch::to_string(kv.second);
		str_msg += ",";
	}
	str_msg.pop_back(); //remove last comma 
	str_msg += "]}";
	str_msg += msg_ending;

	switch (_recipient.object) {
	case ExternalObject_t::raspberry_pi:
		str_msg = (static_cast<std::string>(pub_hdr) + address_map[ExternalObject_t::raspberry_pi] + str_msg);
        result = transmit(str_msg, _wait_until_cplt);
		break;
	case ExternalObject_t::google_home:
		str_msg = (static_cast<std::string>(pub_hdr) + address_map[ExternalObject_t::google_home] + str_msg);
		result = transmit(str_msg, _wait_until_cplt);
		break;
	case ExternalObject_t::broadcast:
		str_msg = (static_cast<std::string>(pub_hdr) + address_map[ExternalObject_t::broadcast] + str_msg);
		result = transmit(str_msg, _wait_until_cplt);
		break;
	case ExternalObject_t::ntp_server:
		break;
	case ExternalObject_t::my_phone:
		break;
	default:
		break;
	}
	return result;
}

bool HAL_UART_DMA_MsgBroker::requestData(const ExternalObject& _recipient, const std::string& _data_key, const bool& _wait_until_cplt)
{
	std::string str_msg{ get_hdr };
	bool result = false;

	switch (_recipient.object) {
	case ExternalObject_t::raspberry_pi:
		str_msg += address_map[ExternalObject_t::raspberry_pi] + _data_key + msg_ending;
		result = transmit(str_msg, _wait_until_cplt);
		break;
	case ExternalObject_t::google_home:
		str_msg += address_map[ExternalObject_t::google_home] + _data_key + msg_ending;
		result = transmit(str_msg, _wait_until_cplt);
		break;
	case ExternalObject_t::ntp_server:
		str_msg += address_map[ExternalObject_t::ntp_server] + _data_key + msg_ending;
        result = transmit(str_msg, _wait_until_cplt);
		break;
	case ExternalObject_t::my_phone:
		break;
	case ExternalObject_t::broadcast:
		break;
	default:
		break;
	}
	return result;
}

// bool HAL_UART_DMA_MsgBroker::setParser(MsgParser *_parser){
// 	if (_parser != nullptr){
// 		parserInstance = _parser;
// 		return true;
// 	}
// 	return false;
// }
// bool HAL_UART_DMA_MsgBroker::setEncoder(MsgEncoder *_encoder){
// 	return false;
// }

void HAL_UART_DMA_MsgBroker::setAddresses(std::unordered_map<ExternalObject_t, std::string>& _address_map)
{
	address_map = _address_map;
}

bool HAL_UART_DMA_MsgBroker::readData(const size_t& _size, void(*action)(const std::string&)){
	memset(rxBuffer, '\0', BUFFER_SIZE);
	if (devValid){
		std::cout << "HAL_UART_Receive_DMA(UART_Handle, rxBuffer, _size);" << std::endl; //TODO: change on STM32
	}
	std::cout << "ulTaskNotifyTake( xArrayIndex, osWaitForever);" << std::endl; //TODO: change on STM32
	if(rxBuffer[0] == '{' && rxBuffer[_size-1] == '}'){
		const std::string time_str{reinterpret_cast<char*>(rxBuffer)};
		return true;// (parserInstance != nullptr) ? parserInstance->parseString(time_str, action) : false;
	}
	return false;
}

bool HAL_UART_DMA_MsgBroker::readData(const size_t& _size){
	memset(rxBuffer, '\0', BUFFER_SIZE);
	if (devValid){
		std::cout << "HAL_UART_Receive_DMA(UART_Handle, rxBuffer, _size);" << std::endl; //TODO: change on STM32
	}
	std::cout << "ulTaskNotifyTake( xArrayIndex, osWaitForever);" << std::endl; //TODO: change on STM32
	if(rxBuffer[0] == '{' && rxBuffer[_size-1] == '}'){
		const std::string time_str{reinterpret_cast<char*>(rxBuffer)};
		return true; // (parserInstance != nullptr) ? parserInstance->parseString(time_str) : false;
	}
	return false;
}

bool HAL_UART_DMA_MsgBroker::transmit(const std::string& _str, const bool& _blocking_mode){
	int32_t bytes_to_send = _str.length();
	uint32_t pos = 0;
	uint8_t retries = 0;
	bool success = false;
	std::cout << "while(bytes_to_send > 0 if (HAL_UART_Transmit_DMA(UART_Handle, txBuffer, MINIMUM(bytes_to_send, BUFFER_SIZE)) == HAL_OK)" << std::endl; //TODO: change on STM32
	success = true; 		//TODO: change on STM32
	/*while(bytes_to_send > 0){
		std::memset(txBuffer, '\0', BUFFER_SIZE);
		_str.copy((char*)txBuffer, MINIMUM(bytes_to_send, BUFFER_SIZE), pos);
		if (devValid){
			if (HAL_UART_Transmit_DMA(UART_Handle, txBuffer, MINIMUM(bytes_to_send, BUFFER_SIZE)) == HAL_OK){
				if (_blocking_mode)	ulTaskNotifyTake( xArrayIndex, osWaitForever);
				bytes_to_send -= BUFFER_SIZE;
				pos += BUFFER_SIZE;
				success = true;
				retries = 0;
			}
			else{
				retries++;
				success = false;
			}
			if (retries > 20) break;
		}
	}*/
	return success;
} 


