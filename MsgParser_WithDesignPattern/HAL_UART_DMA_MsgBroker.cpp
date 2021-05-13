#include "HAL_UART_DMA_MsgBroker.h"
#include "utilities.h"
#include <string>
#include <cstring>
#include "cmsis_os.h"

//extern const UBaseType_t xArrayIndex; //TODO: uncomment on STM32

bool HAL_UART_DMA_MsgBroker::assignDevice(void *_dev_handle)
{
	if (_dev_handle == nullptr) return false;
	uart_handle = static_cast<UART_HandleTypeDef*>(_dev_handle);
	return dev_valid = true;
}

bool HAL_UART_DMA_MsgBroker::sendMsg(const ExternalObject& _recipient, const InternalObject& _publisher, const std::string& _msg, const bool& _wait_until_cplt, Encoder *_encoder)
{
	_ASSERT(_publisher.id > 999 || _recipient.id > 999); //TODO: change this on STM32

	std::string id_str = std::to_string(_publisher.id); //TODO: patch::to_string
	const std::string pub_id = std::string(3 - id_str.length(), '0') + id_str;
	id_str = std::to_string(_recipient.id); //TODO: patch::to_string
	const std::string rcv_id = std::string(3 - id_str.length(), '0') + id_str;

	const Header hdr{ int_address_map->at(_publisher.object) + pub_id,  ext_address_map->at(_recipient.object) + rcv_id };
	const DataItem msg{ "Info", _msg };
	const Footer end{ "", "" };

	if (_encoder != nullptr) {
		hdr.accept(*_encoder);
		msg.accept(*_encoder);
		end.accept(*_encoder);
		return transmit(_encoder->str(), _wait_until_cplt);
	}
	if (encoder != nullptr)
	{
		hdr.accept(*encoder);
		msg.accept(*encoder);
		end.accept(*encoder);
		return transmit(encoder->str(), _wait_until_cplt);
	}

	return false;
}

bool HAL_UART_DMA_MsgBroker::publishData(const ExternalObject& _recipient, const InternalObject& _publisher, std::unordered_map<std::string, int32_t> _values, const bool& _wait_until_cplt, Encoder *_encoder)
{
	_ASSERT(_publisher.id > 999 || _recipient.id > 999); //TODO: change this on STM32

	std::string id_str = std::to_string(_publisher.id); //TODO: patch::to_string
	const std::string pub_id = std::string(3 - id_str.length(), '0') + id_str;
	id_str = std::to_string(_recipient.id); //TODO: patch::to_string
	const std::string rcv_id = std::string(3 - id_str.length(), '0') + id_str;
	
	Header hdr{ int_address_map->at(_publisher.object) + pub_id,  ext_address_map->at(_recipient.object) + rcv_id};
	Data data_list{};
	Footer end{ "", "" };
	
	for (auto &[key,value]: _values)
	{
		DataItem item{key, std::to_string(value)}; //TODO: patch::to_string
		data_list.emplace_back(item);
	}

	if (std::vector<Element*> msg_items{ &hdr, &data_list, &end }; _encoder != nullptr) {
		for (auto &item : msg_items) item->accept(*_encoder);
		return transmit(_encoder->str(), _wait_until_cplt);
	}
	else if(encoder != nullptr)
	{
		for (auto &item : msg_items) item->accept(*encoder);
		return transmit(encoder->str(), _wait_until_cplt);
	}

	return false;
}

bool HAL_UART_DMA_MsgBroker::requestData(const ExternalObject& _recipient, const InternalObject& _publisher, const std::string& _data_key, const std::string& _data_type, const bool& _wait_until_cplt, Encoder *_encoder)
{
	_ASSERT(_publisher.id > 999 || _recipient.id > 999); //TODO: change this on STM32

	std::string id_str = std::to_string(_publisher.id); //TODO: patch::to_string
	const std::string pub_id = std::string(3 - id_str.length(), '0') + id_str;
	id_str = std::to_string(_recipient.id); //TODO: patch::to_string
	const std::string rcv_id = std::string(3 - id_str.length(), '0') + id_str;

	const Header hdr{ int_address_map->at(_publisher.object) + pub_id,  ext_address_map->at(_recipient.object) + rcv_id };
	const DataItem msg{ _data_key, _data_type };
	const Footer end{ "", "" };

	if (_encoder != nullptr) {
		hdr.accept(*_encoder);
		msg.accept(*_encoder);
		end.accept(*_encoder);
		return transmit(_encoder->str(), _wait_until_cplt);
	}
	if (encoder != nullptr)
	{
		hdr.accept(*encoder);
		msg.accept(*encoder);
		end.accept(*encoder);
		return transmit(encoder->str(), _wait_until_cplt);
	}

	return false;
}

 bool HAL_UART_DMA_MsgBroker::setDefaultParser(MsgParser *_parser){
	if (_parser == nullptr) return false;
	delete this->parser;
 	parser= _parser;
 	return true;
 	
 }
 bool HAL_UART_DMA_MsgBroker::setDefaultEncoder(Encoder*_encoder){
	 if (_encoder == nullptr) return false;
	 delete this->encoder;
	 encoder = _encoder;
	 return true;
 }

void HAL_UART_DMA_MsgBroker::setExternalAddresses(std::unordered_map<ExternalObject_t, std::string> *_addresses)
{
	ext_address_map = _addresses;
}
void HAL_UART_DMA_MsgBroker::setInternalAddresses(std::unordered_map<InternalObject_t, std::string> *_addresses)
{
	int_address_map = _addresses;
}

bool HAL_UART_DMA_MsgBroker::readData(void(*action)(const std::string&)){
	memset(rx_buffer, '\0', buffer_size);
	if (dev_valid){
		std::cout << "HAL_UART_Receive_DMA(UART_Handle, rxBuffer, msg_in_len);" << std::endl; //TODO: change on STM32
	}
	std::cout << "ulTaskNotifyTake( xArrayIndex, osWaitForever);" << std::endl; //TODO: change on STM32
	if(rx_buffer[0] == '{' && rx_buffer[msg_in_len -1] == '}'){
		const std::string time_str{reinterpret_cast<char*>(rx_buffer)};
		return true;// (parserInstance != nullptr) ? parserInstance->parseString(time_str, action) : false;
	}
	return false;
}

bool HAL_UART_DMA_MsgBroker::readData(){
	memset(rx_buffer, '\0', buffer_size);
	if (dev_valid){
		std::cout << "HAL_UART_Receive_DMA(UART_Handle, rx_buffer, msg_in_len);" << std::endl; //TODO: change on STM32
	}
	std::cout << "ulTaskNotifyTake( xArrayIndex, osWaitForever);" << std::endl; //TODO: change on STM32
	return true;
}

bool HAL_UART_DMA_MsgBroker::transmit(const std::string& _str, const bool& _blocking_mode){
	int32_t bytes_to_send = _str.length();
	uint32_t pos = 0;
	uint8_t retries = 0;
	bool success = false;
	std::cout << _str; //TODO: del on STM32
	std::cout << "while(bytes_to_send > 0 if (HAL_UART_Transmit_DMA(UART_Handle, txBuffer, MINIMUM(bytes_to_send, buffer_size)) == HAL_OK)" << std::endl; //TODO: change on STM32
	success = true; 		//TODO: change to false on STM32
	/*while(bytes_to_send > 0){
		std::memset(txBuffer, '\0', buffer_size);
		_str.copy((char*)txBuffer, MINIMUM(bytes_to_send, buffer_size), pos);
		if (devValid){
			if (HAL_UART_Transmit_DMA(UART_Handle, txBuffer, MINIMUM(bytes_to_send, buffer_size)) == HAL_OK){
				if (_blocking_mode)	ulTaskNotifyTake( xArrayIndex, osWaitForever);
				bytes_to_send -= buffer_size;
				pos += buffer_size;
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

IncomingMessage HAL_UART_DMA_MsgBroker::getIncoming(MsgParser *_parser)
{
	if (_parser != nullptr) return _parser->parseIncoming(rx_buffer, msg_in_len);
	else if( parser != nullptr)	return parser->parseIncoming(rx_buffer, msg_in_len);
	return {};
}


